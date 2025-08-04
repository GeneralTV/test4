cmake_minimum_required(VERSION 3.15)
project(ResourceLoader)

set(CMAKE_CXX_STANDARD 17)

# Копируем uiresources в папку сборки
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/uiresources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(ResourceLoader SHARED main.cpp)
target_link_libraries(ResourceLoader plugin-sdk)

# После сборки копируем .asi и uiresources в /dist/
add_custom_command(
    TARGET ResourceLoader POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_BINARY_DIR}/uiresources
        ${CMAKE_CURRENT_BINARY_DIR}/dist/uiresources
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/Release/ResourceLoader.asi
        ${CMAKE_CURRENT_BINARY_DIR}/dist/
)
