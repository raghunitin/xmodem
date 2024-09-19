#
# Created by Raghu Sundar on 04/07/2024.
#
project(FileTransmitter)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(UART_FILE "${CMAKE_SOURCE_DIR}/src/uart/uart_trx_pi.cpp")
else ()
    set(UART_FILE "${CMAKE_SOURCE_DIR}/src/uart/uart_trx.cpp")
endif ()

add_executable(FileTransmitter
        ${CMAKE_CURRENT_SOURCE_DIR}/file_transmitter.cpp
        ${UART_FILE}
        ${CMAKE_SOURCE_DIR}/src/xmodem/xmodem.cpp
        ${CMAKE_SOURCE_DIR}/src/xmodem/xmodem_tx.cpp)
target_include_directories(FileTransmitter PUBLIC "${CMAKE_SOURCE_DIR}/includes/")


project(FileReceiver)
add_executable(FileReceiver
        ${CMAKE_CURRENT_SOURCE_DIR}/file_receiver.cpp
        ${UART_FILE}
        ${CMAKE_SOURCE_DIR}/src/xmodem/xmodem.cpp
        ${CMAKE_SOURCE_DIR}/src/xmodem/xmodem_rx.cpp)
target_include_directories(FileReceiver PUBLIC "${CMAKE_SOURCE_DIR}/includes/")

# Linking wiringPi
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
    target_link_libraries(FileTransmitter ${WIRINGPI_LIBRARIES})
    target_link_libraries(FileReceiver ${WIRINGPI_LIBRARIES})
endif ()

install(TARGETS FileTransmitter FileReceiver DESTINATION bin)
