cmake_minimum_required(VERSION 3.16.3)

project(Pong VERSION 1.0)

add_executable(Pong src/main.cpp)

add_subdirectory(src/)

target_include_directories(

)

target_link_directories(

)

target_link_libraries(
    Pong
)
