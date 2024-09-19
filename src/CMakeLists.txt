#
# Created by Raghu Sundar on 04/07/2024.
#

add_library(xmodem
        ${CMAKE_CURRENT_SOURCE_DIR}/xmodem/xmodem.cpp)
target_include_directories(xmodem PUBLIC "${CMAKE_SOURCE_DIR}/includes/")

add_library(xmodemtx
        ${CMAKE_CURRENT_SOURCE_DIR}/xmodem/xmodem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xmodem/xmodem_tx.cpp)
target_include_directories(xmodemtx PUBLIC "${CMAKE_SOURCE_DIR}/includes/")

add_library(xmodemrx
        ${CMAKE_CURRENT_SOURCE_DIR}/xmodem/xmodem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/xmodem/xmodem_rx.cpp)
target_include_directories(xmodemrx PUBLIC "${CMAKE_SOURCE_DIR}/includes/")
