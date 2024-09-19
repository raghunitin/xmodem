//
// Created by Raghu Sundar on 05/07/2024.
//

#ifndef XMODEM_RX_H
#define XMODEM_RX_H
#if defined __cplusplus

#include <chrono>
#include <memory>
#include <utility>

#include <xmodem/xmodem.h>
#include <uart/uart_interface.h>

namespace comms {

    enum class RxState : int {
        SEND_C,
        SEND_C_TIMEOUT,
        SEND_ACK,
        SEND_ACK_TIMEOUT,
        SEND_NACK,
        SEND_NACK_TIMEOUT,
        SEND_EOT_ACK,
        SEND_EOT_ACK_TIMEOUT,
        SEND_FINAL_ACK,
        END_RECEIVE,
        RECEIVE_ERROR,
    };

    class XModemRx : public XModem {
        RxState m_state;
        std::unique_ptr<io::UartInterface> m_uart;
        std::string m_file_path;
        std::uint8_t m_expected_packet_number;
        std::chrono::steady_clock::time_point m_last_c_time;
        std::chrono::steady_clock::time_point m_last_ack_time;
        std::chrono::steady_clock::time_point m_last_nack_time;

    public:
        XModemRx() = delete;
        explicit XModemRx(std::unique_ptr<io::UartInterface> uart_interface) : m_uart(std::move(uart_interface)) {
            m_state = RxState::SEND_C;
            const auto init_time = std::chrono::steady_clock::now() - std::chrono::seconds(4);
            m_last_c_time = init_time;
            m_last_ack_time = init_time;
            m_last_nack_time = init_time;
            m_expected_packet_number = 0x01;
        };
        ~XModemRx() = default;

        XModemRx(const XModemRx&) = delete;
        XModemRx(XModemRx&&) = delete;
        XModemRx& operator=(const XModemRx&) = delete;
        XModemRx& operator=(XModemRx&&) = delete;

        void set_state(RxState state) { m_state = state; } // only for testing
        RxState get_state() const { return m_state; } // only for testing
        bool receive_file(std::string file_path);
        void send_ascii_c(std::chrono::steady_clock::time_point time);
        void send_ascii_c();
        void send_ack(std::chrono::steady_clock::time_point time);
        void send_ack();
        void send_nack(std::chrono::steady_clock::time_point time);
        void send_nack();
        bool handle_incoming(const char* stream);
        void handle_outgoing();
    };

} // namespace comms

#endif // __cplusplus
#endif //XMODEM_RX_H
