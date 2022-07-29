

cmake_minimum_required(VERSION 3.16.3)

project(Pong VERSION 1.0)

add_executable(Pong src/main.cpp)

add_subdirectory(src/Game)


target_include_directories(Pong
    PUBLIC src/Game
)

target_link_directories(Pong
    PUBLIC src/Game
)

target_link_libraries(Pong
    SDL2
    SDL2main
    Game
)

set (CMAKE_CXX_FLAGS "-I/usr/include/SDL2")
