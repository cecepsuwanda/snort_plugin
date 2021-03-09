
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
//#include "Tbase_dataframe.h"
#include "Tdec_tree.h"

using namespace std;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{


  if (stoi(argv[1]) == 0)
  {
    string f_datatype = argv[9];
    string f_train = argv[10];
    //string f_test = argv[7];
    string path_model = argv[11];
    string svm_path = path_model + "/" + argv[12];

    int train_svm = stoi(argv[2]);
    int feature_selection = stoi(argv[3]);
    int normal_only = stoi(argv[4]);

    char *endptr;
    double gamma = strtod(argv[7], &endptr);
    double nu = strtod(argv[8], &endptr);

    int depth = stoi(argv[5]);
    int min_sample = stoi(argv[6]);


    Tdec_tree dec_tree(train_svm, min_sample, depth);
    dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
    dec_tree.set_model_path(path_model);
    dec_tree.set_svm_path(svm_path);
    dec_tree.set_f_train(f_train);
    //dec_tree.set_f_test(f_test);
    dec_tree.set_f_datatype(f_datatype);

    string tmp_str = path_model + "/dtsvm_model.csv";
    remove(tmp_str.c_str());

    for (const auto & file : directory_iterator(path_model + "/train"))
      remove(file.path());

    for (const auto & file : directory_iterator(path_model + "/test"))
      remove(file.path());

    for (const auto & file : directory_iterator(svm_path))
      remove(file.path());

    dec_tree.build_tree();
  } else {
    if (stoi(argv[1]) == 1)
    {
      string f_datatype = argv[9];
      string f_test = argv[10];
      string path_model = argv[11];
      string svm_path = path_model + "/" + argv[12];

      int train_svm = stoi(argv[2]);
      int feature_selection = stoi(argv[3]);
      int normal_only = stoi(argv[4]);

      char *endptr;
      double gamma = strtod(argv[7], &endptr);
      double nu = strtod(argv[8], &endptr);

      int depth = stoi(argv[5]);
      int min_sample = stoi(argv[6]);

      Tdec_tree dec_tree(train_svm, min_sample, depth);
      dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
      dec_tree.set_model_path(path_model);
      dec_tree.set_svm_path(svm_path);
      dec_tree.set_f_test(f_test);
      dec_tree.set_f_datatype(f_datatype);

      for (const auto & file : directory_iterator(path_model + "/test"))
        remove(file.path());

      dec_tree.read_tree();
      dec_tree.test();

    } else {
      if (stoi(argv[1]) == 2)
      {
        string f_datatype = argv[9];
        string f_train = argv[10];
        //string f_test = argv[7];
        string path_model = argv[11];
        string svm_path = path_model + "/" + argv[12];

        int train_svm = stoi(argv[2]);
        int feature_selection = stoi(argv[3]);
        int normal_only = stoi(argv[4]);

        char *endptr;
        double gamma = strtod(argv[7], &endptr);
        double nu = strtod(argv[8], &endptr);

        int depth = stoi(argv[5]);
        int min_sample = stoi(argv[6]);


        Tdec_tree dec_tree(train_svm, min_sample, depth);
        dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
        dec_tree.set_model_path(path_model);
        dec_tree.set_svm_path(svm_path);
        dec_tree.set_f_train(f_train);
        //dec_tree.set_f_test(f_test);
        dec_tree.set_f_datatype(f_datatype);

        for (const auto & file : directory_iterator(path_model + "/train"))
          remove(file.path());

        for (const auto & file : directory_iterator(svm_path))
          remove(file.path());

        dec_tree.read_tree();
        dec_tree.learn_svm();
      }
    }
  }



  return 0;
}

