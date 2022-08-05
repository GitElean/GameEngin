cmake_minimum_required(VERSION 3.16)

project(PONG VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(src/Game)

target_include_directories(${PROJECT_NAME}
  PUBLIC src/Game
)

target_link_directories(${PROJECT_NAME}
  PRIVATE src/Game
)

target_link_libraries(${PROJECT_NAME}
  Game
  SDL2main
  SDL2
  GL
)