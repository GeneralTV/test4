cmake_minimum_required(VERSION 3.15)
project(ResourceLoader)

set(CMAKE_CXX_STANDARD 17)

add_library(ResourceLoader SHARED main.cpp)
target_link_libraries(ResourceLoader plugin-sdk)
