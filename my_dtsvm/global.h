
using namespace std;

#ifndef Included_global_H

#define Included_global_H

  struct Tconfig
  {  
    string f_datatype = "";
    string f_train = "";
    string f_test = "";
    string path_model = "";
    string svm_path = "";

    bool save_train = false;
    bool save_test = false;
    bool use_credal = false;
    double credal_s = 0.0;
    bool limited = false; 

    bool train_svm = false;
    bool feature_selection = false;
    bool normal_only = false;

    
    double gamma = 0.0;
    double nu = 0.0;

    int depth = 0;
    int min_sample = 0;
    int threshold = 0;

    bool search_uniqe_val = false;

  };

#endif