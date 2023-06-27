#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "Tmy_dttype.h"

using namespace std;

#ifndef Included_global_H

#define Included_global_H

struct Tconfig
{

  int jns_dt_train = -1;
  int id_dt_train = -1;
  string partition_train;

  int jns_dt_test = -1;
  int id_dt_test = -1;
  string partition_test;

  string path_model = "";
  string svm_path = "";

  bool use_credal = false;
  double credal_s = 0.0;
  bool limited = false;
  bool prunning = false;

  bool normal_only = false;

  int depth = 0;
  int min_sample = 0;
  double threshold = 0;

  time_t id_experiment;
  time_t id_detail_experiment;

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

  bool is_pure;
  bool is_min_sample;
  bool is_depth_limit;
  bool is_same_label;
  bool is_pruning;
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

    is_pure = false;
    is_min_sample = false;
    is_depth_limit = false;
    is_same_label = false;
    is_pruning = false;
    is_not_split = false;
    is_lanjut = true;

    left = NULL;
    right = NULL;

  }

};

struct Tposisi_cabang
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

  void set_parent(int depth, int branch, int branch_number)
  {
    parent_depth = depth ;
    parent_branch = branch ;
    parent_branch_number = branch_number ;
  }

  void switch_parent_branch()
  {
    parent_depth = child_depth;
    parent_branch = child_branch;
    parent_branch_number = child_branch_number;
  }

  string to_query(int idx)
  {
    string tmp = "";

    if ((idx == 0) or (idx == 1))
    {

      tmp += "((child_depth=" + to_string(child_depth) + ") and ";
      tmp += "(child_branch=" + to_string(child_branch) + ") and ";
      tmp += "(child_branch_number=" + to_string(child_branch_number) + "))";

    }

    if (idx == 0)
    {
      tmp += " and ";
    }

    if ((idx == 0) or (idx == 2))
    {
      tmp += "((parent_depth=" + to_string(parent_depth) + ") and ";
      tmp += "(parent_branch=" + to_string(parent_branch) + ") and ";
      tmp += "(parent_branch_number=" + to_string(parent_branch_number) + "))";
    }


    if (idx == 3)
    {
      tmp += "((child_depth=" + to_string(parent_depth) + ") and ";
      tmp += "(child_branch=" + to_string(parent_branch) + ") and ";
      tmp += "(child_branch_number=" + to_string(parent_branch_number) + "))";
    }


    if (idx == 4)
    {
      tmp += "((parent_depth=" + to_string(child_depth) + ") and ";
      tmp += "(parent_branch=" + to_string(child_branch) + ") and ";
      tmp += "(parent_branch_number=" + to_string(child_branch_number) + "))";
    }

    if ((idx == 5) or (idx == 6))
    {
      tmp += "child_depth = " + to_string(parent_depth) + ",";
      tmp += "child_branch = " + to_string(parent_branch) + ",";
      tmp += "child_branch_number = " + to_string(parent_branch_number) + ",";

    }

    if ((idx == 5) or (idx == 7))
    {
      tmp += "parent_depth = " + to_string(parent_depth) + ",";
      tmp += "parent_branch = " + to_string(parent_branch) + ",";
      tmp += "parent_branch_number = " + to_string(parent_branch_number);
    }


    return tmp;
  }
};

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



#endif