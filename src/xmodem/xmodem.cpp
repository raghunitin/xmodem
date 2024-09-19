//
// Created by Raghu Sundar on 05/07/2024.
//

#include <cstdint>

#include <types/data_packet.h>
#include <xmodem/xmodem.h>

crc_t comms::XModem::calculate_crc(const std::uint8_t* data, std::uint8_t data_bytes) {
    crc_t crc_result{0x0};
    while (0 < data_bytes--) {
        crc_result = crc_result ^ static_cast<std::uint16_t>(*data << 8);
        data += 1;

        unsigned char i = 8;
        do {
            if (crc_result & 0x8000) {
                crc_result = crc_result << 1 ^ 0x1021;
            }
            else {
                crc_result = crc_result << 1;
            }
        } while (0 < --i);
    }
    return crc_result;
}

bool comms::XModem::is_valid_data_packet(const DataPacket &packet, const std::uint8_t expected_packet_number) {
    if (DATA_SOH != packet.header) {
        return false;
    }
    if (PACKET_CHECKSUM != packet.packet_up + packet.packet_down) {
        return false;
    }
    if (expected_packet_number != packet.packet_up) {
        return false;
    }
    if (packet.crc != calculate_crc(&packet.data[0], DATA_LENGTH)) {
        return false;
    }
    return true;
}

bool comms::XModem::is_valid_control_packet(const control_packet_t packet) {
    if (packet != CONTROL_ASCII_C && packet != CONTROL_ACK && packet != CONTROL_NACK) {
        return false;
    }
    return true;
}
