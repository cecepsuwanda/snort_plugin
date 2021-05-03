
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"

using namespace std;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
  char *endptr;
  Tconfig config;

  config.f_datatype = argv[9];

  config.path_model = argv[11];
  config.svm_path = config.path_model + "/" + argv[12];

  config.save_train = stoi(argv[13]) == 1;
  config.save_test = stoi(argv[14]) == 1;
  config.use_credal = stoi(argv[15]) == 1;
  config.credal_s = strtod(argv[16], &endptr);
  config.limited = stoi(argv[17]) == 1;

  config.train_svm = stoi(argv[2]) == 1;
  config.feature_selection = stoi(argv[3]) == 1;
  config.normal_only = stoi(argv[4]) == 1;


  config.gamma = strtod(argv[7], &endptr);
  config.nu = strtod(argv[8], &endptr);

  config.depth = stoi(argv[5]);
  config.min_sample = stoi(argv[6]);


  if (stoi(argv[1]) == 0)
  {
    config.f_train = argv[10];

    Tdec_tree dec_tree;
    dec_tree.set_config(config);

    string tmp_str = config.path_model + "/dtsvm_model.csv";
    remove(tmp_str.c_str());

    if (config.save_train == 1) {
      for (const auto & file : directory_iterator(config.path_model + "/train"))
        remove(file.path());
    }

    for (const auto & file : directory_iterator(config.svm_path))
      remove(file.path());

    dec_tree.build_tree();
  } else {
    if (stoi(argv[1]) == 1)
    {

      config.f_test = argv[10];
      config.normal_only = false;

      Tdec_tree dec_tree;
      dec_tree.set_config(config);

      if (config.save_test == 1) {
        for (const auto & file : directory_iterator(config.path_model + "/test"))
          remove(file.path());
      }

      dec_tree.read_tree();
      dec_tree.test();

    } else {
      if (stoi(argv[1]) == 2)
      {

        config.f_train = argv[10];

        Tdec_tree dec_tree;
        dec_tree.set_config(config);

        for (const auto & file : directory_iterator(config.svm_path))
          remove(file.path());

        dec_tree.read_tree();
        dec_tree.learn_svm();
      }
    }
  }



  return 0;
}

