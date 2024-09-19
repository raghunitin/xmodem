//
// Created by Raghu Sundar on 05/07/2024.
//

#ifndef XMODEM_TX_H
#define XMODEM_TX_H
#if defined __cplusplus

#include <memory>

#include <xmodem/xmodem.h>
#include <uart/uart_interface.h>

namespace comms {

    enum class TxState : int {
        AWAITING_C,
        SEND,
        RESEND,
        WAITING_ACK,
        WAITING_EOT_ACK,
        SEND_ETB,
        WAITING_ETB_ACK,
        END_TRANSMISSION,
        SEND_ERROR
    };

    class XModemTx : public XModem {
        std::string m_file_path;
        std::unique_ptr<io::UartInterface> m_uart;
        TxState m_state;
    protected:
#ifdef TESTS_ENABLED
    public:
#endif


        XModemTx() = delete;
        explicit XModemTx(std::unique_ptr<io::UartInterface> uart_interface) : m_uart(std::move(uart_interface)),
                                                                                m_state(TxState::AWAITING_C) {}
        ~XModemTx() = default;

        XModemTx(const XModemTx&) = delete;
        XModemTx(XModemTx&&) = delete;
        XModemTx& operator=(const XModemTx&) = delete;
        XModemTx& operator=(XModemTx&&) = delete;

        void set_state(TxState state) { m_state = state; } // only for testing
        TxState get_state() const { return m_state; } // only for testing
        void handle_incoming(FILE *file, const char* message);
        bool handle_outgoing(FILE *file, std::uint8_t &packet_number);
        void send_can_packet() const;
        void send_crc_packet(const std::uint8_t* data, std::uint8_t packet_number) const;
        void send_eot_packet() const;
        void send_etb_packet() const;
        bool send_file(std::string file_path);
    };

} // namespace comms

#endif // __cplusplus
#endif //XMODEM_TX_H
