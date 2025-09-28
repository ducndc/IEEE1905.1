add_executable(cmdu_test cmdu_test.cpp)

target_link_libraries(cmdu_test
    ieee1905
    spdlog
    ${NL3_LIBRARIES}
    ${NLROUTE_LIBRARIES}
)
