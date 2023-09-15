#include <stdarg.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <ctime>

#include "Tmy_dttype.h"

using namespace std;

#ifndef Included_global_H

#define Included_global_H

class Tglobal_config
{
public:
  static int jns_dt_train;
  static int id_dt_train;
  static string partition_train;

  static int jns_dt_test;
  static int id_dt_test;
  static string partition_test;

  static bool use_credal;
  static double credal_s;
  static bool prunning;

  static int depth;
  static int min_sample;

  static bool limited;
  static double threshold;
  static bool find_other_attr;

  static time_t id_experiment;
  static time_t id_detail_experiment;

  static double ratio_valid_attr;
  static double skala_pruning;
  static bool continue_attr_only;  
  static bool buat_kombinasi;
  static bool gunakan_rata2gain;
  
  static bool one_agains_many_only;
  

  static bool cetak_credal;

  static void init()
  {
    jns_dt_train = -1;
    id_dt_train = -1;
    partition_train = "-1";

    jns_dt_test = -1;
    id_dt_test = -1;
    partition_test = "-1";

    use_credal = false;
    credal_s = 0.0;

    prunning = true;
    depth = 0;
    min_sample = 0;

    limited = true;
    threshold = 0;
    find_other_attr = true;

    ratio_valid_attr = 0.3;

    skala_pruning = 0.5;

    continue_attr_only = false;    
    buat_kombinasi = true;
    gunakan_rata2gain = true;    
    one_agains_many_only = false;   

    //cetak_credal = false;

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

struct Tposisi_cabang
{
  int child_depth = -1 ;
  int child_branch = -1 ;
  int child_branch_number = -1 ;
  int parent_depth = -1 ;
  int parent_branch = -1 ;
  int parent_branch_number = -1 ;
  string id_branch = "-1";

  void reset()
  {
    child_depth = 0 ;
    child_branch = 0 ;
    child_branch_number = 0 ;
    parent_depth = 0 ;
    parent_branch = 0 ;
    parent_branch_number = 0 ;
    id_branch = "000000";
  }

  string get_id_branch()
  {
    return id_branch;
  }

  void set_child(int depth, int branch, int branch_number)
  {
    child_depth = depth ;
    child_branch = branch ;
    child_branch_number = branch_number ;
    id_branch = to_string(child_depth) + to_string(child_branch) + to_string(child_branch_number) + to_string(parent_depth) + to_string(parent_branch) + to_string(parent_branch_number);
  }

  void set_parent(int depth, int branch, int branch_number)
  {
    parent_depth = depth ;
    parent_branch = branch ;
    parent_branch_number = branch_number ;
    id_branch = to_string(child_depth) + to_string(child_branch) + to_string(child_branch_number) + to_string(parent_depth) + to_string(parent_branch) + to_string(parent_branch_number);
  }

  void switch_parent_branch()
  {
    parent_depth = child_depth;
    parent_branch = child_branch;
    parent_branch_number = child_branch_number;
    id_branch = to_string(child_depth) + to_string(child_branch) + to_string(child_branch_number) + to_string(parent_depth) + to_string(parent_branch) + to_string(parent_branch_number);
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

    if (idx == 8)
    {
      tmp += "'" + id_branch + "',";
      tmp += to_string(child_depth) + ",";
      tmp += to_string(child_branch) + ",";
      tmp += to_string(child_branch_number) + ",";
      tmp += to_string(parent_depth) + ",";
      tmp += to_string(parent_branch) + ",";
      tmp += to_string(parent_branch_number);
    }


    return tmp;
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

#endif