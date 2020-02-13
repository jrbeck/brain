cmake_minimum_required(VERSION 3.9)
project(Brain)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lpthread -ansi -Wall -Werror -Wextra -pedantic -pedantic-errors -O3 -g")

set(SOURCE_FILES
  source/main.cpp
  source/PriorityQueue.cpp
  source/engine/SdlApp.cpp
  source/engine/v2d_t.cpp
  source/engine/Vec2.cpp
  source/engine/Complex.cpp
  source/engine/ImageBuffer.cpp
  source/engine/Pixel.cpp
  source/engine/Painter.cpp
  source/engine/BasicText.cpp
  source/engine/PseudoRandom.cpp
  source/engine/Viewport.cpp
  source/engine/Messaging/MessageBus.cpp
  source/engine/Messaging/Mailbox.cpp
  source/engine/Messaging/Message.cpp
  source/EngineModes/Brain/Controller.cpp
  source/EngineModes/Brain/State.cpp
  source/EngineModes/Brain/Renderer.cpp
  source/EngineModes/Mandelbrot/Controller.cpp
  source/EngineModes/Mandelbrot/State.cpp
  source/EngineModes/Mandelbrot/Renderer.cpp
  source/lib/lodepng.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(brain ${SOURCE_FILES})
