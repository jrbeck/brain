#ifndef MATH_UTIL_H_
#define MATH_UTIL_H_

#include <math.h>


// prototypes * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
double r_num(double low, double high);
int r_numi(int low, int high);
double sign(double num);


// functions * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// returns a double in [low, high) // FIXME: hopefully [low
double r_num(double low, double high) {
  return (((double)rand() / (double)(RAND_MAX + 1.0f)) * (high - low)) + low;
}


// returns an int in [low, high)
int r_numi(int low, int high) {
  low--;
  return (int)((((double)rand() / (double)(RAND_MAX + 1.0f)) * (high - low)) + low);
}


// returns the sign of a number 1 if zero or positive, -1 if negative
double sign(double num) {
  if (num < 0.0f) {
    return -1.0f;
  }
  return 1.0f;
}

#endif
