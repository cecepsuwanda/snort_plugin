
#include <iostream>
#include "Tmy_svm.h"

using namespace std;

int main(int argc, char *argv[])
{
  Tdataframe df_train, df_test, df_save;
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
  my_svm.test(df_test);

  return 0;
}

