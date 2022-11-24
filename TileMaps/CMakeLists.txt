cmake_minimum_required(VERSION 3.16.3)

project(SDLDEMO VERSION 1.0)

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
  SDL2main
  SDL2
  SDL2_image
  Game
  Scene
)