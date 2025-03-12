cmake_minimum_required(VERSION 3.10.0)
project(vt_bike VERSION 0.1.0 LANGUAGES C)
find_package(OpenSSL REQUIRED)

add_executable(vt_bike runKAT.c rng.c)
target_link_libraries(vt_bike OpenSSL::SSL)