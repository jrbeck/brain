#include "Complex.h"

Complex::Complex() :
  mValue(0, 0)
{}

Complex::Complex(VEC2_DATA_TYPE r, VEC2_DATA_TYPE i) :
  mValue(r, i)
{}

Complex::Complex(Vec2 value) :
  mValue(value)
{}

Complex Complex::operator*(const VEC2_DATA_TYPE scalar) const {
  return Complex(mValue * scalar);
}

Complex Complex::operator+(const Complex& other) const {
  return Complex(mValue + other.mValue);
}

Complex Complex::operator*(const Complex& other) const {
  VEC2_DATA_TYPE r = (mValue.x * other.mValue.x) - (mValue.y * other.mValue.y);
  VEC2_DATA_TYPE i = (mValue.x * other.mValue.y) + (mValue.y * other.mValue.x);

  return Complex(r, i);
}

void Complex::squareAndAdd(const Complex& other) {
  VEC2_DATA_TYPE xTemp = mValue.x;
  mValue.x = ((xTemp * xTemp) - (mValue.y * mValue.y)) + other.mValue.x;
  mValue.y = (2.0 * (xTemp * mValue.y)) + other.mValue.y;
}

VEC2_DATA_TYPE Complex::length() const {
  return mValue.length();
}
