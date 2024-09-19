//
// Created by Raghu Sundar on 07/07/2024.
//

#include <uart/uart_trx.h>

#include <cstdio>
#include <string>

#include <wiringPi.h>
#include <wiringSerial.h>


/*
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200


#define UART_TX_PIN 0

#define UART_RX_PIN 1

    // arg parse - commandline argument -> --file -f file_path
    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_puts(UART_ID, stream);
}
*/
