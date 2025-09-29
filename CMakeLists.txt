cmake_minimum_required(VERSION 3.10)

project(IEEE1905_1_LIB VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------
# 1. Core library
# -----------------------------
set(CORE_SRCS
    src_cpp/tlv.cpp
    src_cpp/cmdu.cpp
    src_cpp/serializer.cpp
    src_cpp/parser.cpp
    src_cpp/agent.cpp
    src_cpp/interface_manager.cpp
    src_cpp/lldp.cpp
    src_cpp/wsc.cpp
)

add_library(ieee1905_1 SHARED ${CORE_SRCS})
target_compile_features(ieee1905_1 PUBLIC cxx_std_17)

# public headers
target_include_directories(ieee1905_1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# -----------------------------
# 2. Example
# -----------------------------
add_executable(simple_controller examples/simple_controller.cpp)
target_link_libraries(simple_controller PRIVATE ieee1905_1)

# -----------------------------
# 3. Install
# -----------------------------
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX})
install(TARGETS ieee1905_1
    EXPORT ieee1905_1_Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
