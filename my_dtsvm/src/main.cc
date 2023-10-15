
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"
#include "Tdt_learn_svm.h"


using namespace std;
using std::experimental::filesystem::directory_iterator;


string Tglobal_config::f_datatype;
string Tglobal_config::f_train;
string Tglobal_config::f_test;
string Tglobal_config::path_model;
string Tglobal_config::svm_path;

bool Tglobal_config::save_train;
bool Tglobal_config::save_test;

bool Tglobal_config::use_credal;
double Tglobal_config::credal_s;
bool Tglobal_config::limited;
bool Tglobal_config::prunning;

bool Tglobal_config::train_svm;
bool Tglobal_config::feature_selection;
bool Tglobal_config::normal_only;

double Tglobal_config::gamma;
double Tglobal_config::nu;

int Tglobal_config::depth;
int Tglobal_config::min_sample;
int Tglobal_config::threshold;

bool Tglobal_config::search_uniqe_val;

bool Tglobal_config::gunakan_rata2gain;
bool Tglobal_config::find_other_attr;
double Tglobal_config::skala_pruning;
int Tglobal_config::jml_thread;

int main(int argc, char *argv[])
{
  char *endptr;
  Tglobal_config global_config;
  global_config.init();

  global_config.f_datatype = argv[9];

  global_config.path_model = argv[11];
  global_config.svm_path = global_config.path_model + "/" + argv[12];

  global_config.save_train = stoi(argv[13]) == 1;
  global_config.save_test = stoi(argv[14]) == 1;
  global_config.use_credal = stoi(argv[15]) == 1;
  global_config.credal_s = strtod(argv[16], &endptr);
  global_config.limited = stoi(argv[17]) == 1;
  global_config.threshold = stoi(argv[18]);

  global_config.train_svm = stoi(argv[2]) == 1;
  global_config.feature_selection = stoi(argv[3]) == 1;
  global_config.normal_only = true;//stoi(argv[4]) == 1;


  global_config.gamma = strtod(argv[7], &endptr);
  global_config.nu = strtod(argv[8], &endptr);

  global_config.depth = stoi(argv[5]);
  global_config.min_sample = stoi(argv[6]);


  if (stoi(argv[1]) == 0)
  {
    global_config.f_train = argv[10];
    global_config.prunning = true;

    Tdt_build dec_tree;

    string tmp_str = global_config.path_model + "/dtsvm_model.csv";
    remove(tmp_str.c_str());

    if (global_config.save_train == 1) {
      for (const auto & file : directory_iterator(global_config.path_model + "/train"))
        remove(file.path());
    }

    for (const auto & file : directory_iterator(global_config.svm_path))
      remove(file.path());

    dec_tree.build_tree();
  } else {
    if (stoi(argv[1]) == 1)
    {

      global_config.f_test = argv[10];
      global_config.normal_only = false;

      Tdec_tree dec_tree;

      if (global_config.save_test == 1) {
        for (const auto & file : directory_iterator(global_config.path_model + "/test"))
          remove(file.path());
      }

      dec_tree.read_tree();
      dec_tree.test();

    } else {
      if (stoi(argv[1]) == 2)
      {

        global_config.f_train = argv[10];

        Tdt_learn_svm dec_tree;

        for (const auto & file : directory_iterator(global_config.svm_path))
          remove(file.path());

        dec_tree.read_tree();
        dec_tree.learn_svm();
      }
    }
  }



  return 0;
}

