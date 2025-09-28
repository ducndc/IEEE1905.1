cmake_minimum_required(VERSION 3.10)
project(ieee1905 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
#pkg_check_modules(UBOX REQUIRED libubox)
find_package(OpenSSL REQUIRED)
pkg_check_modules(NL3 REQUIRED libnl-3.0)
pkg_check_modules(NLROUTE REQUIRED libnl-route-3.0)

add_subdirectory(external/spdlog)
add_subdirectory(src)
add_subdirectory(test)
