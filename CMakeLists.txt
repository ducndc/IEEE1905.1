cmake_minimum_required(VERSION 3.10)
project(ieee1905 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -fPIC")

# include path của dự án và libnl3
include_directories(include /usr/include/libnl3)

# Thêm spdlog vào CMake
add_subdirectory(external/spdlog)

set(SRC_DIR src)
set(SOURCES
    ${SRC_DIR}/timer.cpp
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/cmdu.cpp
    ${SRC_DIR}/cmdu_ackq.cpp
    ${SRC_DIR}/ieee_1905_wsc.cpp
    ${SRC_DIR}/crypt_util.cpp
    ${SRC_DIR}/debug.cpp
)

add_library(ieee1905 SHARED ${SOURCES})

set_target_properties(ieee1905 PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "ieee1905"
)

# Link spdlog header-only + các thư viện khác
target_link_libraries(ieee1905
    spdlog::spdlog_header_only
    crypto
    nl-3
    nl-genl-3
)

install(TARGETS ieee1905 LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
