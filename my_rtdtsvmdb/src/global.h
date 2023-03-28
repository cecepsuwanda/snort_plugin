#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

#ifndef Included_global_H

#define Included_global_H

static double bulat_nol(double val, double tolerance, int digit)
{
  double tmp = val;

  if (abs(val) < tolerance)
  {
    tmp = 0.0;
  }
  else
  {
    const double multiplier = std::pow(10.0, digit);
    tmp = ceil(val * multiplier) / multiplier;
  }
  // double tmp = val;
  return tmp;
}

#endif