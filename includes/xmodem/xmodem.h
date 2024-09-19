//
// Created by Raghu Sundar on 04/07/2024.
//
#ifndef COMMS_XMODEM_H
#define COMMS_XMODEM_H
#if defined __cplusplus

#include <cstdint>
#include <memory>

#include <config.h>
#include <types/data_packet.h>

namespace comms {

    class XModem {
    protected:
        std::uint8_t kSOH = DATA_SOH;
        std::uint8_t kEOT = DATA_EOT;
        std::uint8_t kETB = DATA_ETB;
        std::uint8_t kCAN = DATA_CAN;

        std::uint8_t kACK = CONTROL_ACK;
        std::uint8_t kASCII_C = CONTROL_ASCII_C;
        std::uint8_t kNACK = CONTROL_NACK;

        XModem() = default;
    public:
        ~XModem() = default;
        XModem(const XModem&) = delete;
        XModem(XModem&&) = delete;
        XModem& operator=(const XModem&) = delete;
        XModem& operator=(XModem&&) = delete;

        static crc_t calculate_crc(const std::uint8_t* data, std::uint8_t data_bytes);
        static bool is_valid_data_packet(const DataPacket& packet, std::uint8_t expected_packet_number);
        static bool is_valid_control_packet(control_packet_t packet);
    };

} // namespace comms
#endif // __cplusplus
#endif // COMMS_XMODEM_H