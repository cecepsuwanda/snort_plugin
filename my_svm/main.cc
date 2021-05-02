#include <iostream>
#include <experimental/filesystem>
#include "Tmy_svm.h"

using namespace std;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
  string path_model = argv[1];

  double gamma = 0.0005;
  double nu = 0.5;

  for (const auto & file : directory_iterator(path_model + "/train"))
  {
    Tdataframe df_train(false,0.0);
    df_train.read_data(file.path());
    df_train.read_data_type(argv[2]);
    df_train.setjmltotalrow();
    df_train.set_min_sample(2);
    
    // if( (df_train.getjmlrow()>10000) ) //(!df_train.is_single_label()) and
    // {
      df_train.info();
      
      Tmy_svm my_svm(false,true,0,"","",false);
      my_svm.train(df_train,gamma,nu);
      my_svm.test(df_train);

    // }

    df_train.clear_memory();
    df_train.close_file();
  }

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

