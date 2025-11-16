cmake_minimum_required(VERSION 3.15)

project(tiny_http_server
    VERSION 0.1.0
    LANGUAGES CXX
)

# Use C++17 (you can bump to 20 later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Put compiled binaries in a predictable place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the executable
add_executable(tiny_http_server
    src/main.cpp
    src/server.cpp
)

target_include_directories(tiny_http_server 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# On Unix-like systems, enable more warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(tiny_http_server PRIVATE -Wall -Wextra -Wpedantic)
endif()
