//
// Created by Raghu Sundar on 05/07/2024.
//

#ifndef UART_TRX_H
#define UART_TRX_H
#if defined __cplusplus


#include <uart/uart_interface.h>

namespace io {

class UartTrx : public UartInterface {
    int m_serial_fd;
public:
    bool init() override;
    const char* check_for_new_messages(int packet_length) override;
    void send_packet(const char* data_stream) override;
    void shutdown() override;

    UartTrx() = default;
    UartTrx(const UartTrx&) = delete;
    UartTrx(UartTrx&&) = delete;
    UartTrx& operator=(const UartTrx&) = delete;
    UartTrx& operator=(UartTrx&&) = delete;

};

} // namespace io
#endif // __cplusplus
#endif //UART_TRX_H
