cmake_minimum_required(VERSION 3.15)
project(ResourceLoader)

set(CMAKE_CXX_STANDARD 17)

# Для Windows указываем расширение .asi
if(WIN32)
    set(CMAKE_SHARED_LIBRARY_SUFFIX ".asi")
endif()

# Добавляем путь к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(ResourceLoader SHARED main.cpp)
