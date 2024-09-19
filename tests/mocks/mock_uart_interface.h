//
// Created by Raghu Sundar on 10/07/2024.
//
#ifndef MOCK_UART_INTERFACE_H
#define MOCK_UART_INTERFACE_H

#include <gmock/gmock.h>

#include <uart/uart_interface.h>

class MockUartInterface final : public io::UartInterface {
public:
    MOCK_METHOD(bool, init, (), (override));
    MOCK_METHOD(void, send_packet, (const char* data_stream), (override));
    MOCK_METHOD(const char*, check_for_new_messages, (int packet_length), (override));
    MOCK_METHOD(void, shutdown, (), (override));
};
#endif //MOCK_UART_INTERFACE_H