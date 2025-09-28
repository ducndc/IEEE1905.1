cmake_minimum_required(VERSION 3.10)
project(ieee1905 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -fPIC")

# Include path của dự án và libnl3
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /usr/include/libnl3
)

# Thêm spdlog từ submodule
# git submodule add https://github.com/gabime/spdlog external/spdlog
add_subdirectory(external/spdlog)

# Nguồn code
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCES
    ${SRC_DIR}/timer.cpp
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/cmdu.cpp
    ${SRC_DIR}/cmdu_ackq.cpp
    ${SRC_DIR}/ieee_1905_wsc.cpp
    ${SRC_DIR}/crypt_util.cpp
    ${SRC_DIR}/debug.cpp
)

# Tạo shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Đặt thuộc tính cho thư viện
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "ieee1905"
)

# Link spdlog và libnl
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        spdlog::spdlog_header_only
        crypto
        nl-3
        nl-genl-3
)

# Include cho target này
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Cài đặt
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

# Custom clean target
add_custom_target(distclean
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/*
    COMMENT "Removing all build files"
)
