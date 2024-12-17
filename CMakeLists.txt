cmake_minimum_required(VERSION 3.10)
project(NetworkScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(network_scanner 
    src/main.cpp
    src/scanner.cpp
    src/scanner.hpp
)

target_include_directories(network_scanner PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(UNIX)
    target_link_libraries(network_scanner PRIVATE pthread)
endif()
