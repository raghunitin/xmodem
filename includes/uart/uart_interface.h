//
// Created by Raghu Sundar on 05/07/2024.
//

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H
#if defined __cplusplus

namespace io {

class UartInterface {
public:
    virtual ~UartInterface() = default;
    virtual bool init() = 0;
    virtual void send_packet(const char* data_stream) = 0;
    virtual const char* check_for_new_messages(int packet_length) = 0;
    virtual void shutdown() = 0;
};

} // namespace io
#endif // __cplusplus
#endif //UART_INTERFACE_H
