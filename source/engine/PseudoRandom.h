// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// * PseudoRandom
// *
// * this is a linear congruential generator designed to produce the same output
// * as the msvc++ generator, but with some extra built-in utility.
// *
// *  the numbers are generated as:
// *  X[0] = seed value
// *  X[n + 1] = (a * X[n]) + c
// *
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#pragma once

#include <cmath>

class PseudoRandom {
public:
  PseudoRandom();
  PseudoRandom(unsigned seed);

  void setSeed(unsigned seed);
  unsigned getSeed() const;

  int nextInt();
  int nextInt(int low, int high);

  double nextDouble();
  double nextDouble(double low, double high);

  static const unsigned mDefaultSeed = 0;
  static const unsigned mA = 214013;
  static const unsigned mC = 2531011;
  static const unsigned mRandMax = 32767;
  static const unsigned mRandMaxMod = mRandMax + 1;

private:
  unsigned mSeed;
  unsigned mX;
};
