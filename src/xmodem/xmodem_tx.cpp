//
// Created by Raghu Sundar on 05/07/2024.
//

#include <cstdint>
#include <iostream>
#include <utility>

#include <xmodem/xmodem_tx.h>

#include <types/data_packet.h>
#include <types/xmodem_buffer.h>

void comms::XModemTx::send_crc_packet(const std::uint8_t* data, const std::uint8_t packet_number) const {
    DataPacket packet;
    packet.header = DATA_SOH;
    packet.packet_up = packet_number;
    packet.packet_down = PACKET_CHECKSUM - packet_number;
    for (unsigned char &i : packet.data) {
        i = *data++;
    }
    packet.crc = calculate_crc(data, DATA_LENGTH);
    m_uart->send_packet(packet.to_string().c_str());
}

void comms::XModemTx::send_eot_packet() const {
    m_uart->send_packet(std::to_string(DATA_EOT).c_str());
}

void comms::XModemTx::send_etb_packet() const {
    m_uart->send_packet(std::to_string(DATA_ETB).c_str());
}

void comms::XModemTx::send_can_packet() const {
    m_uart->send_packet(std::to_string(DATA_CAN).c_str());
}

bool comms::XModemTx::send_file(std::string file_path) {
    m_file_path = std::move(file_path);
    if (m_uart) {
        std::cout << "UART interface unavailable" << std::endl;
        return false;
    }
    if (m_uart->init()) {
        std::cout << "UART init failed, quitting" << std::endl;
        return false;
    }
    std::uint8_t packet_number{0x00};
    FILE *fp = fopen(file_path.c_str(), "rb");
    fseek(fp, 0, SEEK_END);
    long int file_size = ftell(fp);
    fclose(fp);
    int percent;
    int sent_times = 0;
    FILE *f = fopen(file_path.c_str(), "rb");
    while (TxState::END_TRANSMISSION != m_state && TxState::SEND_ERROR != m_state) {
        handle_incoming(f, m_uart->check_for_new_messages(CONTROL_LENGTH));
        bool is_file_sent = handle_outgoing(f, packet_number);
        if (is_file_sent) {
            ++sent_times;
            percent = ((sent_times * XBUFFER_DATA_LENGTH) / file_size) * 100;
        }
        // todo: show progress bar with percent value, every 2 seconds?.
    }
    fclose(f);
    m_uart->shutdown();
    return TxState::END_TRANSMISSION == m_state;
}

void comms::XModemTx::handle_incoming(FILE *file, const char* message) {
    if (message != nullptr) {
        if (CONTROL_ASCII_C == *message) {
            switch (m_state) {
                case TxState::AWAITING_C: {
                    m_state = TxState::SEND;
                    break;
                }
                default: {
                    m_state = TxState::SEND_ERROR;
                    break;
                }
            }
        }
        else if (CONTROL_ACK == *message) {
            switch (m_state) {
                case TxState::AWAITING_C: {
                    m_state = TxState::SEND_ERROR;
                    break;
                }
                case TxState::WAITING_ACK: {
                    fseek(file, XBUFFER_DATA_LENGTH, SEEK_CUR);
                    m_state = TxState::SEND;
                    break;
                }
                case TxState::WAITING_EOT_ACK: {
                    m_state = TxState::SEND_ETB;
                    break;
                }
                case TxState::WAITING_ETB_ACK: {
                    m_state = TxState::END_TRANSMISSION;
                    break;
                }
                default:
                    // do nothing
                        break;
            }
        }
        else if (CONTROL_NACK == *message) {
            switch (m_state) {
                case TxState::AWAITING_C: {
                    m_state = TxState::SEND_ERROR;
                    break;
                }
                case TxState::WAITING_ACK:
                case TxState::WAITING_EOT_ACK:
                case TxState::WAITING_ETB_ACK: {
                    m_state = TxState::RESEND;
                    break;
                }
                default:
                    // do nothing
                        break;
            }
        }
        else {
            // ignoring junk
        }
    }
}

bool comms::XModemTx::handle_outgoing(FILE *file, std::uint8_t &packet_number) {
    bool file_sent = false;
    XModemBuffer x_buffer{};
    switch (m_state) {
        case TxState::SEND: {
            packet_number++;
            x_buffer.data_length = fread(x_buffer.data, sizeof(std::uint8_t), XBUFFER_DATA_LENGTH, file);
        }
        case TxState::RESEND: {
            if (x_buffer.data_length != 0) {
                std::string data_to_send = x_buffer.to_string();
                m_state = TxState::WAITING_ACK;
                send_crc_packet(reinterpret_cast<std::uint8_t*>(data_to_send[0]), packet_number);
                file_sent = true;
            }
            else {
                m_state = TxState::WAITING_EOT_ACK;
                send_eot_packet();
            }
            break;
        }
        case TxState::SEND_ETB: {
            m_state = TxState::WAITING_ETB_ACK;
            send_etb_packet();
            break;
        }
        default:
            // do nothing
                break;
    }
    return file_sent;
}
