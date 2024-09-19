//
// Created by Raghu Sundar on 05/07/2024.
//

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>

#include <getopt.h>

#include <uart/uart_trx.h>
#include <xmodem/xmodem_rx.h>


int main(int argc, char* argv[]) {
    opterr = 0;
    int c;

    static constexpr size_t kOptionSize = 2;
    static constexpr std::array<option, kOptionSize> kLongOptions {
                {
                    {"file", required_argument, nullptr, 'f'},
                    {nullptr, 0, nullptr, 0}
                }
    };
    static constexpr const char* kShortOptions{"f:"};

    std::string output_file{};
    bool getopt_result{false};

    while ((c = getopt_long(argc, argv, kShortOptions, kLongOptions.data(), nullptr)) != -1) {
        switch (c) {
            case 'f': {
                output_file = static_cast<std::string>(optarg);
                getopt_result = true;
                break;
            }
            default:
                std::cout << "Unknown argument received: " << c << " : " << optarg << std::endl;
        }
    }

    if (getopt_result) {
        // check if destination valid
        FILE *f = fopen(output_file.c_str(), "wb");
        if (f == nullptr) {
            std::cout << "Exiting, bad destination: " << output_file << std::endl;
            std::exit(-2);
        }
        fclose(f);
        std::cout << "Receiving: " << output_file << std::endl;
        std::unique_ptr<io::UartTrx> p_uart_trx = std::make_unique<io::UartTrx>();
        comms::XModemRx rx(std::move(p_uart_trx));
        auto receive_status = rx.receive_file(output_file);
        std::cout << "file receive " << (receive_status ? "complete" : "failed") << std::endl;
    }
    else {
        std::cout << "Exiting, input parameter '-f' missing" << std::endl;
        std::exit(-1);
    }
    std::exit(0);
}
