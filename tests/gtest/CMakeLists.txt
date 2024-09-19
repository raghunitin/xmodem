
enable_testing()

add_executable(xmodem_test xmodem_test.cpp)
target_link_libraries(xmodem_test xmodem GTest::gtest_main)

add_executable(xmodem_tx_test
                xmodem_tx_test.cpp
                ${CMAKE_SOURCE_DIR}/tests/mocks/mock_uart_interface.h)
target_include_directories(xmodem_tx_test PUBLIC ${CMAKE_SOURCE_DIR}/tests/mocks)
target_link_libraries(xmodem_tx_test xmodemtx GTest::gtest_main GTest::gmock_main)

add_executable(xmodem_rx_test
                xmodem_rx_test.cpp
                ${CMAKE_SOURCE_DIR}/tests/mocks/mock_uart_interface.h)
target_include_directories(xmodem_rx_test PUBLIC ${CMAKE_SOURCE_DIR}/tests/mocks)
target_link_libraries(xmodem_rx_test xmodemrx GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(xmodem_test)
gtest_discover_tests(xmodem_tx_test)
gtest_discover_tests(xmodem_rx_test)
