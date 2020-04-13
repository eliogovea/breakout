project(simple_triangle)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 14)

# set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS         "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(PkgConfig REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
find_package(GLEW REQUIRED STATIC)
include_directories(${GLEW_INCLUDE_DIRS})

# GLFW
find_package(glfw3 REQUIRED)

find_package(Threads)

set(
  SOURCE_FILES
  src/client.cpp
  include/breakout/graphics/window.cpp
  include/vendor/tiny_obj_loader.cpp
  include/vendor/stb_image.cpp
)

add_executable(client ${SOURCE_FILES})
target_link_libraries(
  client
  OpenGL::GL 
  GLEW::GLEW
  glfw
  ${CMAKE_THREAD_LIBS_INIT}
) 

add_executable(server
  src/server.cpp
)