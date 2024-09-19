//
// Created by Raghu Sundar on 09/07/2024.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mock_uart_interface.h>

#include <xmodem/xmodem_tx.h>

TEST(handle_incoming, check_header_c) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemTx test_tx(std::move(p_uart));
    test_tx.set_state(comms::TxState::AWAITING_C);

    constexpr char test = 0x43;
    test_tx.handle_incoming(&test);
    EXPECT_EQ(test_tx.get_state(), comms::TxState::SEND);
}

TEST(handle_incoming, check_header_c_on_ack) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemTx test_tx(std::move(p_uart));
    test_tx.set_state(comms::TxState::WAITING_ACK);

    constexpr char test = 0x43;
    test_tx.handle_incoming(&test);
    EXPECT_EQ(test_tx.get_state(), comms::TxState::SEND_ERROR);
}

