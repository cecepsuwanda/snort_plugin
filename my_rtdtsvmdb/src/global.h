#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

#ifndef Included_global_H

#define Included_global_H

class Node {
public:
  int criteriaAttrIndex;
  string attrValue;
  string label;

  int treeIndex;
  bool isLeaf;
  int opt;
  int idx_svm;

  vector<int > children;

  Node() {
    criteriaAttrIndex = -1;
    attrValue = "-1";
    label = "-1";
    treeIndex = -1;
    isLeaf = false;
    opt = -1;
    idx_svm = -1;
  }
};

// static double bulat_nol(double val, double tolerance, int digit)
// {
//   double tmp = val;

//   if (abs(val) < tolerance)
//   {
//     tmp = 0.0;
//   }
//   else
//   {
//     const double multiplier = std::pow(10.0, digit);
//     tmp = ceil(val * multiplier) / multiplier;
//   }
//   // double tmp = val;
//   return tmp;
// }

#endif