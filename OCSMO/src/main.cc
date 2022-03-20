#include <iostream>
#include <experimental/filesystem>
#include "Tmy_svm.h"

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
  char *endptr;
  Tconfig config;
  config.f_datatype = argv[2];
  config.path_model = argv[1];
  config.svm_path = config.path_model + "/" + argv[3];

  config.gamma = strtod(argv[4], &endptr);
  config.V = strtod(argv[5], &endptr);

    int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {

       string str = file.path().filename();       
       if(str=="train_model_2.csv"){  
        Tdataframe df_train(&config);
        df_train.read_data(file.path());
        df_train.read_data_type(config.f_datatype);
        df_train.by_pass_filter_on();
        df_train.info();

        jml = jml + df_train.getjmlrow_svm();
        
        Tmy_svm my_svm(&config);        
        my_svm.train(df_train);
        // my_svm.test(df_train,conf_metrix);

        df_train.clear_memory();
        df_train.close_file(); 
        }   
  }

  cout << "jml = " << jml << endl;

  return 0;
}
