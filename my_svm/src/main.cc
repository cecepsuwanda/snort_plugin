#include <iostream>
#include <experimental/filesystem>
#include "Tconf_metrix.h"
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
  config.nu = strtod(argv[5], &endptr);

  Tconf_metrix conf_metrix;
  conf_metrix.add_konversi_asli("known","inside");
  conf_metrix.add_konversi_asli("normal","inside");
  conf_metrix.add_konversi_asli("unknown","outside");

  int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {

    string str = file.path().filename();
    string tmp = argv[6];
    // if ( tmp == "train")
    // {
    //   str.replace(0, 5, "svm");
    // } else {
    //   str.replace(0, 4, "svm");
    // }

    // path v_path(config.svm_path + "/" + str);
    
    // if (exists(v_path)) {
       
        Tdataframe df_train(&config);
        df_train.read_data(file.path());
        df_train.read_data_type(config.f_datatype);
        df_train.info();

        jml = jml + df_train.getjmlrow_svm();

        Tmy_svm my_svm(&config);
        //my_svm.load_model(config.svm_path + "/" + str);
        my_svm.train(df_train.get_all_record_svm());
        my_svm.test(df_train,conf_metrix);

        df_train.clear_memory();
        df_train.close_file();
        
    // }else{
    //   cout << str << " tidak ada !!!" << endl;
    // }    
  }

  cout << "jml = " << jml << endl;
  conf_metrix.kalkulasi();
  cout << conf_metrix << endl;  

  return 0;
}

