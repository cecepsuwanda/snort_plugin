
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
    char *endptr;

    string f_datatype = argv[9];
    string f_train = argv[10];
    //string f_test = argv[7];
    string path_model = argv[11];
    string svm_path = path_model + "/" + argv[12];

    int save_train = stoi(argv[13]);
    int save_test = stoi(argv[14]);
    int use_credal = stoi(argv[15]);
    double credal_s = strtod(argv[16], &endptr);
    int limited = stoi(argv[17]); 

    int train_svm = stoi(argv[2]);
    int feature_selection = stoi(argv[3]);
    int normal_only = stoi(argv[4]);

    
    double gamma = strtod(argv[7], &endptr);
    double nu = strtod(argv[8], &endptr);

    int depth = stoi(argv[5]);
    int min_sample = stoi(argv[6]);


    Tdec_tree dec_tree(train_svm, min_sample, depth, save_train, save_test,use_credal,credal_s,limited);
    dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
    dec_tree.set_model_path(path_model);
    dec_tree.set_svm_path(svm_path);
    dec_tree.set_f_train(f_train);
    //dec_tree.set_f_test(f_test);
    dec_tree.set_f_datatype(f_datatype);

    string tmp_str = path_model + "/dtsvm_model.csv";
    remove(tmp_str.c_str());

    if (save_train == 1) {
      for (const auto & file : directory_iterator(path_model + "/train"))
        remove(file.path());
    }

    for (const auto & file : directory_iterator(svm_path))
      remove(file.path());

    dec_tree.build_tree();
  } else {
    if (stoi(argv[1]) == 1)
    {
      char *endptr;

      string f_datatype = argv[9];
      string f_test = argv[10];
      string path_model = argv[11];
      string svm_path = path_model + "/" + argv[12];

      int save_train = stoi(argv[13]);
      int save_test = stoi(argv[14]);
      int use_credal = stoi(argv[15]);
      double credal_s = strtod(argv[16], &endptr);
      int limited = stoi(argv[17]);

      int train_svm = stoi(argv[2]);
      int feature_selection = stoi(argv[3]);
      int normal_only = stoi(argv[4]);

      
      double gamma = strtod(argv[7], &endptr);
      double nu = strtod(argv[8], &endptr);

      int depth = stoi(argv[5]);
      int min_sample = stoi(argv[6]);

      Tdec_tree dec_tree(train_svm, min_sample, depth, save_train, save_test,use_credal,credal_s,limited);
      dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
      dec_tree.set_model_path(path_model);
      dec_tree.set_svm_path(svm_path);
      dec_tree.set_f_test(f_test);
      dec_tree.set_f_datatype(f_datatype);

      if(save_test==1){
      for (const auto & file : directory_iterator(path_model + "/test"))
        remove(file.path());
      }

      dec_tree.read_tree();
      dec_tree.test();

    } else {
      if (stoi(argv[1]) == 2)
      {
        char *endptr;

        string f_datatype = argv[9];
        string f_train = argv[10];
        //string f_test = argv[7];
        string path_model = argv[11];
        string svm_path = path_model + "/" + argv[12];

        int save_train = stoi(argv[13]);
        int save_test = stoi(argv[14]);
        int use_credal = stoi(argv[15]);
        double credal_s = strtod(argv[16], &endptr);
        int limited = stoi(argv[17]);

        int train_svm = stoi(argv[2]);
        int feature_selection = stoi(argv[3]);
        int normal_only = stoi(argv[4]);

        
        double gamma = strtod(argv[7], &endptr);
        double nu = strtod(argv[8], &endptr);

        int depth = stoi(argv[5]);
        int min_sample = stoi(argv[6]);


        Tdec_tree dec_tree(train_svm, min_sample, depth, save_train, save_test,use_credal,credal_s,limited);
        dec_tree.set_svm_param(feature_selection, normal_only, gamma , nu);
        dec_tree.set_model_path(path_model);
        dec_tree.set_svm_path(svm_path);
        dec_tree.set_f_train(f_train);
        //dec_tree.set_f_test(f_test);
        dec_tree.set_f_datatype(f_datatype);

        for (const auto & file : directory_iterator(svm_path))
          remove(file.path());

        dec_tree.read_tree();
        dec_tree.learn_svm();
      }
    }
  }



  return 0;
}

