cmake_minimum_required(VERSION 3.10)

project(IEEE1905_1_LIB
    VERSION 0.1
    DESCRIPTION "IEEE 1905.1 Core Library"
    HOMEPAGE_URL "https://github.com/yourname/ieee1905"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -----------------------------
# 1. External dependencies (spdlog via submodule)
# -----------------------------
add_subdirectory(external/spdlog)

# Export spdlog khi install
install(TARGETS spdlog
    EXPORT ieee1905_1_Targets
)


option(BUILD_EXAMPLES "Build example programs" ON)

# -----------------------------
# 2. Core Library
# -----------------------------
set(CORE_SRCS
    src/tlv.cpp
    src/cmdu.cpp
    src/serializer.cpp
    src/parser.cpp
    src/agent.cpp
    src/interface_manager.cpp
    src/lldp.cpp
    src/wsc.cpp
    src/logger.cpp
)

add_library(ieee1905_1 SHARED ${CORE_SRCS})

target_include_directories(ieee1905_1 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(ieee1905_1 PUBLIC cxx_std_17)

# link to submodule
target_link_libraries(ieee1905_1 PUBLIC spdlog::spdlog)

# -----------------------------
# 3. Examples (optional)
# -----------------------------
if (BUILD_EXAMPLES)
    add_executable(simple_controller examples/simple_controller.cpp)
    target_link_libraries(simple_controller PRIVATE ieee1905_1 spdlog::spdlog)
endif()

# -----------------------------
# 4. Install
# -----------------------------
install(TARGETS ieee1905_1
    EXPORT ieee1905_1_Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT ieee1905_1_Targets
    FILE ieee1905_1Targets.cmake
    NAMESPACE ieee1905_1::
    DESTINATION lib/cmake/ieee1905_1
)
