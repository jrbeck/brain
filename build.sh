#!/bin/sh

clear

g++ \
  source/main.cpp \
  source/PriorityQueue.cpp \
  source/engine/SdlApp.cpp \
  source/engine/Vec2.cpp \
  source/engine/ImageBuffer.cpp \
  source/engine/Pixel.cpp \
  source/engine/Painter.cpp \
  source/engine/BasicText.cpp \
  source/engine/PseudoRandom.cpp \
  source/engine/Messaging/MessageBus.cpp \
  source/engine/Messaging/Mailbox.cpp \
  source/engine/Messaging/Message.cpp \
  source/EngineModes/Brain/Controller.cpp \
  source/EngineModes/Brain/State.cpp \
  source/EngineModes/Brain/Renderer.cpp \
  source/lib/lodepng.cpp \
  -lSDL2 -ansi -Wall -Wextra -O3 -std=c++11 -o brain
