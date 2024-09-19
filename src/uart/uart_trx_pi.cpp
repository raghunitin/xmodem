//
// Created by Raghu Sundar on 07/07/2024.
//

#include <uart/uart_trx.h>

#include <cstdio>
#include <iostream>
#include <string>

#include <wiringPi.h>
#include <wiringSerial.h>

#define UART_TTY "/dev/ttyS0"
#define BAUD_RATE 115200

bool io::UartTrx::init() {
    m_serial_fd = serialOpen(UART_TTY, BAUD_RATE);
    if (m_serial_fd < 0) {
        std::cout << "Unable to open serial port" << std::endl;
        return false;
    }
    return true;
}

void io::UartTrx::shutdown() {
    serialClose(m_serial_fd);
}

void io::UartTrx::send_packet(const char* data_stream) {
    serialPuts(m_serial_fd, data_stream);
    serialFlush(m_serial_fd);
}

const char* io::UartTrx::check_for_new_messages(int packet_length) {
    int bytes_available = serialDataAvail(m_serial_fd);
    if (bytes_available >= packet_length) {
        char result[bytes_available]{};
        for (auto i=0; i < bytes_available; i++) {
            auto tmp = serialGetchar(m_serial_fd);
            result[i] = static_cast<char>(tmp);
        }
        return &result[0];
    }
    return nullptr;
}

/*
#include "hardware/uart.h"

#define UART_ID uart0



#define UART_TX_PIN 0

#define UART_RX_PIN 1

    // arg parse - commandline argument -> --file -f file_path
    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_puts(UART_ID, stream);
}
*/
