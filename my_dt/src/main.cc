
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"

using namespace std;

int main(int argc, char *argv[])
{
  char *endptr;
  Tconfig config;

  config.f_datatype = argv[8];
  config.path_model = argv[11];

  config.use_credal = stoi(argv[4]) == 1;
  config.credal_s = strtod(argv[5], &endptr);
  config.limited = stoi(argv[6]) == 1;
  config.threshold = stoi(argv[7]);

  config.depth = stoi(argv[2]);
  config.min_sample = stoi(argv[3]);

  config.f_train = argv[9];
  config.f_test = argv[10];

  config.search_uniqe_val = true;
  Tdataframe df_train(&config);
  df_train.read_data(config.f_train);
  df_train.read_data_type(config.f_datatype);
  
  
  config.search_uniqe_val = false; 
  Tdataframe df_test(&config);
  df_test.read_data(config.f_test);
  df_test.read_data_type(config.f_datatype);
  

for(double k=0.0;k<=2.0;k+=0.5)
{  
  config.use_credal = k != 0.0;
  config.credal_s = k; 
  for (int i = 2; i < 201; i+=2) 
  {    
    config.min_sample = i;
    for (int j = 2; j < 50; ++j)
    {

      config.depth = j;  
      config.search_uniqe_val = true;
      config.prunning = true;

      // if (stoi(argv[1]) == 0)
      // {            
      Tdt_build dec_tree_build(&config);
      string tmp_str = config.path_model + "/dtsvm_model.csv";
      remove(tmp_str.c_str());
      dec_tree_build.build_tree(df_train);
      // } else {

      //     if (stoi(argv[1]) == 1)
      //     {
      
      config.search_uniqe_val = false; 
      Tdec_tree dec_tree_test(&config);
      dec_tree_test.read_tree();
      dec_tree_test.test(df_test);
      //     }
      // }
      
      config.search_uniqe_val = true;
      df_train.stat_tabel();
    }
  }
 } 

  df_train.close_file();
  df_test.close_file();

  return 0;
}
