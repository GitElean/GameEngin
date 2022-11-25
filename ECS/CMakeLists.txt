cmake_minimum_required(VERSION 3.16)

project(PONG VERSION 1.0)

add_executable(${PROJECT_NAME} src/main.cpp)


include_directories(src)
add_subdirectory(src/Game)
add_subdirectory(src/Scene)

target_include_directories(${PROJECT_NAME}
  PUBLIC src/Game
  PUBLIC src/Scene
)

target_link_directories(${PROJECT_NAME}
  PRIVATE src/Game
  PRIVATE src/Scene
)

target_link_libraries(${PROJECT_NAME}
  Game
  Scene
  SDL2main
  SDL2
  GL
)

set(CMAKE_CXX_FLAGS"-std=c++17")