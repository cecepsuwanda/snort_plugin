#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>

#include "Tmy_dttype.h"

using namespace std;

#ifndef Included_global_H

#define Included_global_H

struct Tglobal_config
{
public:

  static string f_datatype;
  static string f_train;
  static string f_test;
  static string path_model;
  static string svm_path;

  static bool save_train;
  static bool save_test;

  static bool use_credal;
  static double credal_s;
  static bool limited;
  static bool prunning;

  static bool train_svm;
  static bool feature_selection;
  static bool normal_only;

  static double gamma;
  static double nu;

  static int depth;
  static int min_sample;
  static int threshold;

  static bool search_uniqe_val;

  static bool gunakan_rata2gain;
  static bool find_other_attr;
  static double skala_pruning;
  static int jml_thread;

  static void init()
  {
    f_datatype = "";
    f_train = "";
    f_test = "";
    path_model = "";
    svm_path = "";

    save_train = false;
    save_test = false;

    use_credal = false;
    credal_s = 0.0;
    limited = false;
    prunning = false;

    train_svm = false;
    feature_selection = false;
    normal_only = false;


    gamma = 0.0;
    nu = 0.0;

    depth = 0;
    min_sample = 0;
    threshold = 0;

    search_uniqe_val = false;

    gunakan_rata2gain = true;
    find_other_attr = true;
    skala_pruning = 0.5;
    jml_thread = 2;
  }

};




class Node {
public:
  int criteriaAttrIndex;
  Tmy_dttype attrValue;
  string label;

  int treeIndex;
  bool isLeaf;
  int opt;
  int idx_svm;

  vector<int > children;

  Node() {
    criteriaAttrIndex = -1;
    attrValue.set_value("-1", true);
    label = "-1";
    treeIndex = -1;
    isLeaf = false;
    opt = -1;
    idx_svm = -1;
  }
};

struct tree_node
{
  int criteriaAttrIndex;
  Tmy_dttype attrValue;
  string label;

  int treeIndex;
  bool isLeaf;
  int opt;
  int idx_svm;

  int depth;
  int branch;
  int branch_number;

  int jml_normal;
  int jml_known;

  bool is_pure;
  bool is_min_sample;
  bool is_depth_limit;
  bool is_same_label;
  bool is_pruning;
  bool is_pruning_1;
  bool is_not_split;
  bool is_lanjut;

  tree_node *left;
  tree_node *right;

  tree_node()
  {
    criteriaAttrIndex = -1;
    attrValue.set_value("-1", true);
    label = "-1";

    treeIndex = -1;
    isLeaf = false;
    opt = -1;
    idx_svm = -1;

    depth = 0;
    branch = 0;
    branch_number = 0;

    jml_normal = 0;
    jml_known = 0;  

    left = NULL;
    right = NULL;

  }

};

class Tpesan
{
public:
  static void cetak_stdout(const char *s)
  {
    fputs(s, stdout);
    fflush(stdout);
  }

  static void cetak ( const char * format, ... )
  {
    char buffer[256];
    va_list args;
    va_start (args, format);
    vsprintf (buffer, format, args);
    //perror (buffer);
    va_end (args);
    cetak_stdout(buffer);
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