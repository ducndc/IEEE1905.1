file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(ieee1905 SHARED ${SRC_FILES})

target_include_directories(ieee1905 PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${UBOX_INCLUDE_DIRS}
    ${NL3_INCLUDE_DIRS}
    ${NLROUTE_INCLUDE_DIRS}
)

target_link_libraries(ieee1905
    spdlog
    OpenSSL::Crypto
    ${UBOX_LIBRARIES}
    ${NL3_LIBRARIES}
    ${NLROUTE_LIBRARIES}
)
