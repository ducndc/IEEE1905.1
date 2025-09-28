cmake_minimum_required(VERSION 3.10)
project(ieee1905 CXX)

# Tùy chọn build
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2 -fPIC")

# Thư mục include
include_directories(include /usr/include/libnl3)

# Danh sách source
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

# Tạo thư viện shared (.so)
add_library(ieee1905 SHARED ${SOURCES})

# Thiết lập versioning
set_target_properties(ieee1905 PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    OUTPUT_NAME "ieee1905"
)

# Link với các thư viện ngoài
target_link_libraries(ieee1905
    crypto
    nl-3
    nl-genl-3
    #spdlog
)

# Cài đặt (install)
install(TARGETS ieee1905
    LIBRARY DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
