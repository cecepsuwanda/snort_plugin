#include <iostream>
#include <experimental/filesystem>
#include "Tmy_svdd.h"

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
  config.C = strtod(argv[5], &endptr);

  int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {

    // string str = file.path().filename();
    // string tmp = argv[6];
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
      //config.C = (float) 1/df_train.getjmlrow_svm();

      //cout << "C " << config.C << endl;


      Tmy_svdd my_svdd(&config);
      //my_svdd.load_model(config.svm_path + "/" + str);
      vector<vector<string>> table = df_train.get_all_record_svm();
      my_svdd.train(table);
      my_svdd.test(df_train);

      df_train.clear_memory();
      df_train.close_file();

    // }else{
    //   cout << str << " tidak ada !!!" << endl;
    // }
  

    
  }

  cout << "jml = " << jml << endl; 

  return 0;
}

