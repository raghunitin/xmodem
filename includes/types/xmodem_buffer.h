//
// Created by Raghu Sundar on 09/07/2024.
//

#ifndef XMODEM_BUFFER_H
#define XMODEM_BUFFER_H
#if defined __cplusplus

#include <cstdint>
#include <sstream>
#include <string>

#include <config.h>

// todo: try integrating file IOs here
    struct XModemBuffer {
        std::uint8_t data_length;
        std::uint8_t data[XBUFFER_DATA_LENGTH]{};

        std::string to_string() {
            std::stringstream data_stream;
            data_stream << data;
            return std::string{std::to_string(data_length) + data_stream.str()};
        }
        XModemBuffer() = default;
        ~XModemBuffer() = default;
        explicit XModemBuffer(const std::uint8_t *datastream) {
            data_length = *datastream++;
            for (unsigned char &each_byte : data) {
                each_byte = *datastream++;
            }
        }
    };

#endif // __cplusplus
#endif //XMODEM_BUFFER_H
