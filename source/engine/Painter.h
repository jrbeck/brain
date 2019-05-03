#pragma once

#include "ImageBuffer.h"
#include "Vec2.h"
#include "v2d_t.h" // DEPRECATED
#include "BasicText.h"

#define MY_PI (3.14159265358979323846)
#define DEG2RAD (0.01745329251994329577)

#define DRAWSTRINGF_MAX_LENGTH (4096)

class RgbFloat {
public:
  RgbFloat() : r(0), g(0), b(0) {};
  RgbFloat(float red, float green, float blue) : r(red), g(green), b(blue) {};

  RgbFloat scale(const float a) const {
    return RgbFloat(r * a, g * a, b * a);
  }

  void scale(const RgbFloat& other, float a) {
    r = other.r * a;
    g = other.g * a;
    b = other.b * a;
  }

  float r;
  float g;
  float b;
};

class Painter {
public:
  Painter(ImageBuffer* imageBuffer);
  ~Painter();

  void clear(unsigned char red, unsigned char green, unsigned char blue);

  bool isOnScreen(int x, int y) const;

  void setRgb(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
  void setRgb(int x, int y, RgbFloat color);
  void setRgb(const v2di_t& a, unsigned char red, unsigned char green, unsigned char blue);
  void setRgb(const Vec2& a, unsigned char red, unsigned char green, unsigned char blue);

  void drawLine(const Vec2& a, const Vec2& b, unsigned char red, unsigned char green, unsigned char blue);
  void drawLine(const Vec2& a, const Vec2& b, const RgbFloat& color);
  void drawOnScreenLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue);

  void drawRing(VEC2_DATA_TYPE radius, const Vec2& center, const RgbFloat& color);
  void drawHex(VEC2_DATA_TYPE radius, const Vec2& center, const RgbFloat& color);

  void drawString(int x, int y, const char* text);
  void drawFormattedString(int x, int y, const char* format, ...);

private:
  bool clipTest(VEC2_DATA_TYPE p, VEC2_DATA_TYPE q, VEC2_DATA_TYPE& t1, VEC2_DATA_TYPE& t2);
  void clipLine(int x1, int y1, int x2, int y2, unsigned char red, unsigned char green, unsigned char blue);

  ImageBuffer* mImageBuffer;
  BasicText* mBasicText;
  int mWidth, mHeight;
  char mTextBuffer[DRAWSTRINGF_MAX_LENGTH];
};
