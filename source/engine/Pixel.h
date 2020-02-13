#pragma once

#include <cmath>

class Pixel {
public:
  int mR, mG, mB;

  Pixel();
  Pixel(const Pixel& other);
  ~Pixel();

  Pixel operator=(const Pixel& rhs);

  void setRgb(unsigned char r, unsigned char g, unsigned char b);
  void fromRgba(unsigned char* rgba);

  void copy(const Pixel& other);
  void add(const Pixel& other);
  void scale(float scalar);

  float pivotRgb(float value) const;
  float pivotXyz(float value) const;

  void toXyz(float* rgb) const;
  void toLab(float* xyz) const;
};
