#pragma once

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include "../lib/lodepng.h"

#include "Pixel.h"

#define RAW_PIXEL_SIZE (4)
#define SDL_BIT_DEPTH  (32)

class ImageBuffer {
public:
  ImageBuffer();
  ImageBuffer(const ImageBuffer& other);
  ~ImageBuffer();

  void copy(const ImageBuffer& other);

  unsigned resize(unsigned width, unsigned height);
  void clear(unsigned char r, unsigned char g, unsigned char b);

  unsigned loadPng(const char* filename);
  unsigned savePng(const char* filename);

  unsigned getWidth() const;
  unsigned getHeight() const;
  unsigned softResize(unsigned width, unsigned height);

  Pixel* getPixel(int x, int y);
  void setPixel(int x, int y, const Pixel& pixel);
  void setRgb(int x, int y, unsigned char r, unsigned char g, unsigned char b);

  void convertToRgba(std::vector<unsigned char>& rgbaVector) const;
  SDL_Surface* toSdlSurface() const;

private:
  unsigned mWidth, mHeight;
  Pixel* mPixels;

  // copy constructor guard
  // ImageBuffer(const ImageBuffer& imageBuffer) { }
  // assignment operator guard
  // ImageBuffer& operator=(const ImageBuffer& imageBuffer) { return *this; }

  unsigned destroyPixels();

  unsigned getOffset(int x, int y);
  void buildFromRgbaVector(std::vector<unsigned char>& rgbaVector);
};
