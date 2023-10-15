#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <ctime>

#include "Tmy_dttype.h"

using namespace std;

#ifndef Included_global_H

#define Included_global_H

struct Tglobal_config
{
  static string f_datatype;
  static string f_train;
  static string f_test;

  static string path_model;
  static string svm_path;

  static time_t id_experiment_dt;
  static time_t id_detail_experiment_dt;

  static time_t id_experiment;
  static time_t id_detail_experiment;

  static bool save_train;
  static bool save_test;

  static bool train_svm;
  static bool normal_only;

  static double gamma;
  static double nu;

  static void init()
  {
    f_datatype = "";
    f_train = "";
    f_test = "";

    path_model = "";
    svm_path = "";

    save_train = false;
    save_test = false;

    train_svm = false;
    normal_only = true;

    gamma = 0.0;
    nu = 0.0;
  }

};


struct Tmetric_split_value
{
  int idx = 0;
  float overall_metric = -1;
  string split_value = "-1";
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

    is_pure = false;
    is_min_sample = false;
    is_depth_limit = false;
    is_same_label = false;
    is_pruning = false;
    is_pruning_1 = false;
    is_not_split = false;
    is_lanjut = true;

    left = NULL;
    right = NULL;

  }

};

struct posisi_cabang
{
  int child_depth = -1 ;
  int child_branch = -1 ;
  int child_branch_number = -1 ;
  int parent_depth = -1 ;
  int parent_branch = -1 ;
  int parent_branch_number = -1 ;

  void reset()
  {
    child_depth = 0 ;
    child_branch = 0 ;
    child_branch_number = 0 ;
    parent_depth = 0 ;
    parent_branch = 0 ;
    parent_branch_number = 0 ;
  }

  void set_child(int depth, int branch, int branch_number)
  {
    child_depth = depth ;
    child_branch = branch ;
    child_branch_number = branch_number ;
  }

  void switch_parent_branch()
  {
    parent_depth = child_depth;
    parent_branch = child_branch;
    parent_branch_number = child_branch_number;
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