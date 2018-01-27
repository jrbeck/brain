#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "ImageBuffer.h"

class SdlApp {
public:
  SdlApp();
  ~SdlApp();

  int init(unsigned width, unsigned height);
  int quit();

  int drawFrame(const ImageBuffer* imageBuffer);

private:
  SDL_Window* mSdlWindow;
  SDL_Renderer* mSdlRenderer;
  SDL_Texture* mSdlTexture;

  void destroyTexture();
};
