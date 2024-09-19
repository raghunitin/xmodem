//
// Created by Raghu Sundar on 05/07/2024.
//

#include <cstdint>
#include <iostream>

#include <xmodem/xmodem_rx.h>

#include <types/data_packet.h>
#include <types/xmodem_buffer.h>

bool comms::XModemRx::receive_file(std::string file_path) {
    m_file_path = std::move(file_path);
    if (m_uart) {
        std::cout << "UART interface unavailable" << std::endl;
        return false;
    }
    if (m_uart->init()) {
        std::cout << "UART init failed, quitting" << std::endl;
        return false;
    }
    while(RxState::END_RECEIVE != m_state && RxState::RECEIVE_ERROR != m_state) {
        handle_outgoing();
        handle_incoming(m_uart->check_for_new_messages(CONTROL_LENGTH));
    }
    m_uart->shutdown();
    return RxState::END_RECEIVE == m_state;
}

void comms::XModemRx::send_ascii_c(std::chrono::steady_clock::time_point time) {
    if ((time - m_last_c_time) >= std::chrono::milliseconds(RESEND_CONTROL_TIMEOUT_MILLISECONDS)) {
        m_last_c_time = time;
        send_ascii_c();
    }
}

void comms::XModemRx::send_ascii_c() {
    m_last_c_time = std::chrono::steady_clock::now();
    m_uart->send_packet(std::to_string(CONTROL_ASCII_C).c_str());
}

void comms::XModemRx::send_ack(std::chrono::steady_clock::time_point time) {
    if ((time - m_last_ack_time) >= std::chrono::milliseconds(RESEND_CONTROL_TIMEOUT_MILLISECONDS)) {
        m_last_ack_time = time;
        send_ack();
    }
}

void comms::XModemRx::send_ack() {
    m_last_ack_time = std::chrono::steady_clock::now();
    m_uart->send_packet(std::to_string(CONTROL_ACK).c_str());
}

void comms::XModemRx::send_nack(std::chrono::steady_clock::time_point time) {
    if ((time - m_last_nack_time) >= std::chrono::milliseconds(RESEND_CONTROL_TIMEOUT_MILLISECONDS)) {
        m_last_nack_time = time;
        send_nack();
    }
}

void comms::XModemRx::send_nack() {
    m_last_nack_time = std::chrono::steady_clock::now();
    m_uart->send_packet(std::to_string(CONTROL_NACK).c_str());
}

bool comms::XModemRx::handle_incoming(const char* stream) {
    bool file_received = false;
    if (stream != nullptr) {
        if (DATA_SOH == *stream) {
            DataPacket packet(stream);
            if (is_valid_data_packet(packet, m_expected_packet_number)) {
                switch (m_state) {
                    case RxState::SEND_C_TIMEOUT:
                    case RxState::SEND_ACK_TIMEOUT:
                    case RxState::SEND_NACK_TIMEOUT: {
                        m_state = RxState::SEND_ACK;
                        FILE *f = fopen(m_file_path.c_str(), "ab");
                        XModemBuffer buff(&packet.data[0]);
                        if (fwrite(buff.data, sizeof(buff.data[0]), buff.data_length, f) != buff.data_length) {
                            std::cout << "Exiting, write to file failed: " << m_file_path << std::endl;
                            std::exit(-3);
                        }
                        else {
                            file_received = true;
                        }
                        fclose(f);
                        m_expected_packet_number++;
                        break;
                    }
                    default: {
                        m_state = RxState::RECEIVE_ERROR;
                        break;
                    }
                }
            }
            else {
                m_state = RxState::SEND_NACK;
            }
        }
        else if (DATA_CAN == *stream) {
            m_state = RxState::SEND_C;
        }
        else if (DATA_EOT == *stream) {
            switch (m_state) {
                case RxState::SEND_ACK_TIMEOUT:
                case RxState::SEND_NACK_TIMEOUT: {
                    m_state = RxState::SEND_EOT_ACK;
                    break;
                }
                default: {
                    m_state = RxState::RECEIVE_ERROR;
                    break;
                }
            }
        }
        else if (DATA_ETB == *stream) {
            switch (m_state) {
                case RxState::SEND_EOT_ACK_TIMEOUT: {
                    m_state = RxState::SEND_FINAL_ACK;
                    break;
                }
                default: {
                    m_state = RxState::RECEIVE_ERROR;
                    break;
                }
            }
        }
        else {
            // do nothing
        }
    }
    return file_received;
}

void comms::XModemRx::handle_outgoing() {
    switch (m_state) {
        case RxState::SEND_C_TIMEOUT:
            send_ascii_c(std::chrono::steady_clock::now());
            break;
        case RxState::SEND_C: {
            m_state = RxState::SEND_C_TIMEOUT;
            send_ascii_c();
            break;
        }
        case RxState::SEND_ACK_TIMEOUT:
            send_ack(std::chrono::steady_clock::now());
            break;
        case RxState::SEND_ACK: {
            m_state = RxState::SEND_ACK_TIMEOUT;
            send_ack();
            break;
        }
        case RxState::SEND_NACK_TIMEOUT:
            send_nack(std::chrono::steady_clock::now());
            break;
        case RxState::SEND_NACK: {
            m_state = RxState::SEND_NACK_TIMEOUT;
            send_nack();
            break;
        }
        case RxState::SEND_EOT_ACK_TIMEOUT:
            send_ack(std::chrono::steady_clock::now());
            break;
        case RxState::SEND_EOT_ACK: {
            m_state = RxState::SEND_EOT_ACK_TIMEOUT;
            send_ack();
            break;
        }
        case RxState::SEND_FINAL_ACK: {
            m_state = RxState::END_RECEIVE;
            send_ack();
            break;
        }
        default:
            // do nothing
            break;
    }
}
