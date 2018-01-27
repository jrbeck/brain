#include "PseudoRandom.h"

PseudoRandom::PseudoRandom() {
  setSeed(mDefaultSeed);
}

PseudoRandom::PseudoRandom(unsigned seed) {
  setSeed(seed);
}

void PseudoRandom::setSeed(unsigned seed) {
  mX = mSeed = seed;
}

unsigned PseudoRandom::getSeed() const {
  return mSeed;
}

int PseudoRandom::nextInt() {
  // unsigned value = (int)((mX = ((mA * mX) + mC)) >> 16u);

  // FIXME: is there a faster way to do this?
  // perhaps value = (value % mRandMaxMod)...
  // while (value > mRandMax) {
  //   value -= mRandMaxMod;
  // }

  return (int)((mX = ((mA * mX) + mC)) >> 16u) % mRandMaxMod;
  // return value;
}

// returns int in [low, high)
int PseudoRandom::nextInt(int low, int high) {
  double val = (double)nextInt ();
  return (int)floor(((val / (double)(mRandMax + 1)) * (double)(high - low)) + (double)low);
}

double PseudoRandom::nextDouble() {
  return (double)nextInt () / ((double)mRandMax + 1.0);
}

double PseudoRandom::nextDouble(double low, double high) {
  double nextInteger = (double)nextInt ();
  return (((nextInteger / (double)(mRandMaxMod)) * (high - low)) + low);
}
