#pragma once

#include "ImageBuffer.h"
#include "Vec2.h"
#include "v2d_t.h" // DEPRECATED
#include "BasicText.h"

#define DRAWSTRINGF_MAX_LENGTH (4096)

class Painter {
public:
  Painter(ImageBuffer* imageBuffer);
  ~Painter();

  void clear(unsigned char red, unsigned char green, unsigned char blue);

  bool isOnScreen(int x, int y) const;

  void setRgb(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
  void setRgb(const v2di_t& a, unsigned char red, unsigned char green, unsigned char blue);
  void setRgb(const Vec2& a, unsigned char red, unsigned char green, unsigned char blue);
  void drawLine(const Vec2& a, const Vec2& b, unsigned char red, unsigned char green, unsigned char blue);

  void drawString(int x, int y, const char* text);
  void drawFormattedString(int x, int y, const char* format, ...);

private:
  ImageBuffer* mImageBuffer;
  BasicText* mBasicText;
  int mWidth, mHeight;
  char mTextBuffer[DRAWSTRINGF_MAX_LENGTH];
};
