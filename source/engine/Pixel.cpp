#include "Pixel.h"

Pixel::Pixel() :
  mR(0),
  mG(0),
  mB(0)
{}

// Pixel(const Pixel &other) :
//   mR(other.mR),
//   mG(other.mG),
//   mB(other.mB)
// {}

Pixel Pixel::operator=(const Pixel& rhs) {
  Pixel lhs;
  lhs.mR = rhs.mR;
  lhs.mG = rhs.mG;
  lhs.mB = rhs.mB;
  return lhs;
}

Pixel::~Pixel() {}

void Pixel::setRgb(unsigned char r, unsigned char g, unsigned char b) {
  mR = r;
  mG = g;
  mB = b;
}

void Pixel::fromRgba(unsigned char* rgba) {
  mR = rgba[0];
  mG = rgba[1];
  mB = rgba[2];
}

void Pixel::copy(const Pixel& other) {
  mR = other.mR;
  mG = other.mG;
  mB = other.mB;
}

void Pixel::add(const Pixel& other) {
  mR += other.mR;
  mG += other.mG;
  mB += other.mB;
}

void Pixel::scale(float scalar) {
  mR = (int)((float)mR * scalar);
  mG = (int)((float)mG * scalar);
  mB = (int)((float)mB * scalar);
}

float Pixel::pivotRgb(float value) const {
    if (value > 0.04045) {
      return pow(((value + 0.055) / 1.055), 2.4);
    }
    return value / 12.92;
}

float Pixel::pivotXyz(float value) const {
    // epsilon = 0.008856  # 216 / 24389
    // kappa = 903.3       # 24389 / 27
    if (value > 0.008856) {
      return pow(value, (1.0 / 3.0));
    }
    // (7.787 * value) + (16.0 / 116.0)
    return (903.3 * value + 16) / 116.0;
}

void Pixel::toXyz(float* xyz) const {
  float rFloat = (float)mR / 255.0;
  float gFloat = (float)mG / 255.0;
  float bFloat = (float)mB / 255.0;

  float pR = pivotRgb(rFloat) * 100.0;
  float pG = pivotRgb(gFloat) * 100.0;
  float pB = pivotRgb(bFloat) * 100.0;

  // Observer = 2deg, Illuminant = D65
  xyz[0] = pR * 0.4124 + pG * 0.3576 + pB * 0.1805;
  xyz[1] = pR * 0.2126 + pG * 0.7152 + pB * 0.0722;
  xyz[2] = pR * 0.0193 + pG * 0.1192 + pB * 0.9505;
}

void Pixel::toLab(float* lab) const {
  float xyz[3];
  toXyz(xyz);

  float pX = pivotXyz(xyz[0] / 95.047);  // ref_X =  95.047
  float pY = pivotXyz(xyz[1] / 100.000); // ref_Y = 100.000
  float pZ = pivotXyz(xyz[2] / 108.883); // ref_Z = 108.883

  lab[0] = (116.0 * pY) - 16.0;
  lab[1] = 500.0 * (pX - pY);
  lab[2] = 200.0 * (pY - pZ);
}
