
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
#include "Twrite_file.h"

using namespace std;

#ifndef Included_Tconf_metrix_H

#define Included_Tconf_metrix_H

struct Tdata
{
  int TP, FP, TN, FN, jml;
  float accuracy, recall, specificity;
};

class Tconf_metrix
{
private:
  map<string, map<string, int>> matrik;
  map<string, Tdata> matrik1;
  float accuracy, precision, recall;
  int failed, tepat, tdk_tepat, jml_data;
  map<string, int> guess;
public:
  Tconf_metrix();
  ~Tconf_metrix();
  void add_jml(string asli, string tebakan, int jml);
  void kalkulasi();
  int get_TP(string kelas);
  int get_TN(string kelas);
  int get_FP(string kelas);
  int get_FN(string kelas);
  void save(string nm_file, string param_nm_file, int param_depth, int param_min_sample, double param_gamma, double param_nu, double param_credal_s);

  friend ostream & operator << (ostream &out, const Tconf_metrix &tc);
};

#endif