#
# Created by Raghu Sundar on 04/07/2024.
#

cmake_minimum_required(VERSION 3.12.0)
project(XModem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "turn on or off tests" OFF)

add_subdirectory(src)

add_subdirectory(scripts)

if (ENABLE_TESTS)
	add_subdirectory(tests)
endif()
