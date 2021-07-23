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
  config.nu = strtod(argv[5], &endptr);

  int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {

    string str = file.path().filename();
    string tmp = argv[6];
    if ( tmp == "train")
    {
      str.replace(0, 5, "svm");
    } else {
      str.replace(0, 4, "svm");
    }

    path v_path(config.svm_path + "/" + str);

    // df_train.set_min_sample(2);

    //cout << str << endl;

    // if( (df_train.getjmlrow()>10000) ) //(!df_train.is_single_label()) and
    // {
    // df_train.info();
    if (exists(v_path)) {

      Tdataframe df_train(&config);
      df_train.read_data(file.path());
      df_train.read_data_type(config.f_datatype);
      df_train.info();

      jml = jml + df_train.getjmlrow_svm();

      Tmy_svm my_svm(&config);
      my_svm.load_model(config.svm_path + "/" + str);
      // my_svm.train(df_train,gamma,nu);
      my_svm.test(df_train);

      df_train.clear_memory();
      df_train.close_file();

    }else{
      cout << str << " tidak ada !!!" << endl;
    }
    // }

    
  }

  cout << "jml = " << jml << endl;

  /*Tdataframe df_train, df_test, df_save;
  df_train.read_data(argv[4]);
  df_train.read_data_type(argv[3]);

  df_test.read_data(argv[5]);
  df_test.read_data_type(argv[3]);

  cout << "train " << df_train.getjmlrow() << " test " << df_test.getjmlrow() << endl;

  Tmy_svm my_svm;

  char *endptr;
  double gamma = strtod(argv[1], &endptr);
  double nu = strtod(argv[2], &endptr);

  my_svm.train(df_train,gamma,nu);
  my_svm.save_model(argv[6]);

  my_svm.load_model(argv[6]);
  my_svm.test(df_test);*/

  return 0;
}

