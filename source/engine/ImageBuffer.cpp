#include "ImageBuffer.h"

ImageBuffer::ImageBuffer() :
  mWidth(0),
  mHeight(0),
  mPixels(nullptr)
{}

ImageBuffer::ImageBuffer(const ImageBuffer& other) :
  mWidth(0),
  mHeight(0),
  mPixels(nullptr)
{
  copy(other);
}

ImageBuffer::ImageBuffer(uint width, uint height) :
  mPixels(nullptr)
{
  resize(width, height);
}

ImageBuffer::~ImageBuffer() {
  destroyPixels();
}

void ImageBuffer::copy(const ImageBuffer& other) {
  if (mWidth == other.getWidth() && mHeight == other.getHeight()) {
    uint totalPixels = mWidth * mHeight;
    memcpy((void*)mPixels, (void*)other.mPixels, totalPixels * sizeof(Pixel));
    return;
  }

  mWidth = other.getWidth();
  mHeight = other.getHeight();
  std::vector<unsigned char> rgbaVector;
  other.convertToRgba(rgbaVector);
  buildFromRgbaVector(rgbaVector);
}

uint ImageBuffer::resize(uint width, uint height) {
  destroyPixels();

  mWidth = width;
  mHeight = height;

  uint totalPixels = mWidth * mHeight;
  mPixels = new Pixel[totalPixels];
  for (uint offset = 0; offset < totalPixels; ++offset) {
    mPixels[offset].setRgb(0, 0, 0);
  }

  return totalPixels;
}

void ImageBuffer::clear(unsigned char r, unsigned char g, unsigned char b) {
  uint totalPixels = mWidth * mHeight;
  for (uint offset = 0; offset < totalPixels; ++offset) {
    mPixels[offset].setRgb(r, g, b);
  }
}

uint ImageBuffer::loadPng(const char* filename) {
  std::vector<unsigned char> rgbaVector;
  uint error = lodepng::decode(rgbaVector, mWidth, mHeight, filename);
  if (error) {
    std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
    return error;
  }
  buildFromRgbaVector(rgbaVector);
  return 0;
}

uint ImageBuffer::savePng(const char* filename) {
  std::vector<unsigned char> rgbaVector;
  convertToRgba(rgbaVector);
  uint error = lodepng::encode(filename, rgbaVector, mWidth, mHeight);
  if (error) {
    std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
    return error;
  }
  return 0;
}

uint ImageBuffer::getWidth() const {
  return mWidth;
}

uint ImageBuffer::getHeight() const {
  return mHeight;
}

uint ImageBuffer::softResize(uint width, uint height) {
  if (width * height != mWidth * mHeight) {
    return 1;
  }
  mWidth = width;
  mHeight = height;
  return 0;
}

Pixel *ImageBuffer::getPixel(int x, int y) {
  uint offset = getOffset(x, y);
  return &mPixels[offset];
}

void ImageBuffer::setPixel(int x, int y, const Pixel& pixel) {
  uint offset = getOffset(x, y);
  mPixels[offset].setRgb(pixel.mR, pixel.mG, pixel.mB);
}

void ImageBuffer::setRgb(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
  uint offset = getOffset(x, y);
  mPixels[offset].setRgb(r, g, b);
}

void ImageBuffer::convertToRgba(std::vector<unsigned char>& rgbaVector) const {
  rgbaVector.resize(mWidth * mHeight * RAW_PIXEL_SIZE);

  uint totalPixels = mWidth * mHeight;
  for (uint offset = 0; offset < totalPixels; ++offset) {
    rgbaVector[(offset * RAW_PIXEL_SIZE) + 0] = mPixels[offset].mR;
    rgbaVector[(offset * RAW_PIXEL_SIZE) + 1] = mPixels[offset].mG;
    rgbaVector[(offset * RAW_PIXEL_SIZE) + 2] = mPixels[offset].mB;
    rgbaVector[(offset * RAW_PIXEL_SIZE) + 3] = 255;
  }
}

SDL_Surface* ImageBuffer::toSdlSurface() const {
  SDL_Surface* surface = SDL_CreateRGBSurface(
    0,
    mWidth,
    mHeight,
    SDL_BIT_DEPTH,
    0x000000ff,
    0x0000ff00,
    0x00ff0000,
    0xff000000
  );

  if (surface == nullptr) {
    return nullptr;
  }

  std::vector<unsigned char> rgbaVector;
  convertToRgba(rgbaVector);
  memcpy(surface->pixels, &rgbaVector[0], mWidth * mHeight * RAW_PIXEL_SIZE);
  return surface;
}

void ImageBuffer::destroyPixels() {
  if (mPixels != nullptr) {
    delete [] mPixels;
    mPixels = nullptr;
  }

  mWidth = 0;
  mHeight = 0;
}

uint ImageBuffer::getOffset(int x, int y) {
  return x + (y * mWidth);
}

void ImageBuffer::buildFromRgbaVector(std::vector<unsigned char>& rgbaVector) {
  destroyPixels();
  uint totalPixels = mWidth * mHeight;
  mPixels = new Pixel[totalPixels];
  for (uint offset = 0; offset < totalPixels; ++offset) {
    mPixels[offset].fromRgba(&rgbaVector[offset * RAW_PIXEL_SIZE]);
  }
}
