//
// Created by Raghu Sundar on 09/07/2024.
//

#ifndef DATA_PACKET_H
#define DATA_PACKET_H
#if defined __cplusplus

#include <cstdint>
#include <sstream>
#include <string>

#include <config.h>

typedef std::uint16_t crc_t;

typedef std::uint8_t control_packet_t;

struct DataPacket {
    // implement type safety for members
    std::uint8_t header;
    std::uint8_t packet_up;
    std::uint8_t packet_down;
    // TODO: implement type-safety struct for data and define [] operator
    std::uint8_t data[DATA_LENGTH]{};
    crc_t crc;

    std::string to_string() {
        std::stringstream data_stream;
        data_stream << data;
        return std::string{
            std::to_string(header) +
            std::to_string(packet_up) +
            std::to_string(packet_down) +
            data_stream.str() +
            std::to_string(crc)};
    }

    DataPacket() = default;
    ~DataPacket() = default;
    explicit DataPacket(const char *stream) {
        header = *stream++;
        packet_up = *stream++;
        packet_down = *stream++;
        for (unsigned char &each_byte : data) {
            each_byte = *stream++;
        }
        crc = static_cast<std::uint8_t>(*stream++);
        crc = (crc << 8) + *stream;
    }
    DataPacket(const DataPacket&) = delete;
    DataPacket(DataPacket&&) = delete;
    DataPacket& operator=(const DataPacket&) = delete;
    DataPacket& operator=(DataPacket&&) = delete;
};

#endif // __cplusplus
#endif //DATA_PACKET_H
