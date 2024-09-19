//
// Created by Raghu Sundar on 05/07/2024.
//

#include <array>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>


#include <getopt.h>

#include <uart/uart_trx.h>
#include <xmodem/xmodem_tx.h>

// todo: check header includes on all files

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

    std::string file_path;
    bool getopt_result{false};

    while ((c = getopt_long(argc, argv, kShortOptions, kLongOptions.data(), nullptr)) != -1) {
        switch (c) {
            case 'f': {
                file_path = static_cast<std::string>(optarg);
                getopt_result = true;
                break;
            }
            default:
                std::cout << "Unknown argument received: " << c << " : " << optarg << std::endl;
        }
    }

    if (getopt_result) {
        FILE *f = fopen(file_path.c_str(), "rb");
        if (f == nullptr) {
            std::cout << "Exiting, bad file: " << file_path << std::endl;
            std::exit(-2);
        }
        fclose(f);
        std::cout << "Transferring: " << file_path << std::endl;
        std::unique_ptr<io::UartTrx> p_uart_trx = std::make_unique<io::UartTrx>();
        comms::XModemTx tx(std::move(p_uart_trx));
        auto send_status = tx.send_file(file_path);
        std::cout << "file transfer " << (send_status ? "complete" : "failed") << std::endl;
    }
    else {
        std::cout << "Exiting, input parameter '-f' missing" << std::endl;
        std::exit(-1);
    }
    std::exit(0);
}
