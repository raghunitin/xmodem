//
// Created by Raghu Sundar on 09/07/2024.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mock_uart_interface.h>

#include <xmodem/xmodem_rx.h>

TEST(send_ascii_c, check_c_on_uart) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    EXPECT_CALL(*p_uart, send_packet(testing::_)).Times(testing::Exactly(1));

    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.send_ascii_c();
}

TEST(send_ack, check_ack_on_uart) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    EXPECT_CALL(*p_uart, send_packet(testing::_)).Times(testing::Exactly(1));

    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.send_ack();
}

TEST(send_nack, check_nack_on_uart) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    EXPECT_CALL(*p_uart, send_packet(testing::_)).Times(testing::Exactly(1));

    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.send_nack();
}

TEST(handle_incoming, check_header_can) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    EXPECT_EQ(test_rx.get_state(), comms::RxState::SEND_C);

    constexpr char test = 0x18;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::SEND_C);
}

TEST(handle_incoming, check_header_eot_on_c) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    EXPECT_EQ(test_rx.get_state(), comms::RxState::SEND_C);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);

    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_c_timeout) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_C_TIMEOUT);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_ack) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_ACK);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_nack) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_NACK);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_eot_ack) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_EOT_ACK);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_eot_ack_timeout) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_EOT_ACK_TIMEOUT);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::RECEIVE_ERROR);
}

TEST(handle_incoming, check_header_eot_on_ack_timeout) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_ACK_TIMEOUT);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::SEND_EOT_ACK);
}

TEST(handle_incoming, check_header_eot_on_nack_timeout) {
    std::unique_ptr<MockUartInterface> p_uart = std::make_unique<MockUartInterface>();
    comms::XModemRx test_rx(std::move(p_uart));
    test_rx.set_state(comms::RxState::SEND_NACK_TIMEOUT);

    constexpr char test = 0x04;
    test_rx.handle_incoming(&test);
    EXPECT_EQ(test_rx.get_state(), comms::RxState::SEND_EOT_ACK);
}

