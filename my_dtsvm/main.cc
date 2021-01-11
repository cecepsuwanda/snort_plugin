
#include <iostream>
#include "Tdec_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  Tdataframe df_train, df_test, df_save;
  char *endptr; 

  df_train.read_data(argv[6]);
  df_train.read_data_type(argv[5]);

  df_test.read_data(argv[7]);
  df_test.read_data_type(argv[5]);

  cout << "train " << df_train.getjmlrow() << " test " << df_test.getjmlrow() << endl;

  df_save.read_data(argv[8]);

  Tdec_tree dec_tree;
 
  cout << "Train Decission Tree : " << endl;
  dec_tree.train(df_train, 0, 0, stoi(argv[2]), stoi(argv[1]),strtod(argv[3],&endptr),strtod(argv[4],&endptr));

 
  dec_tree.save_tree(df_save);

  //dec_tree.read_tree(df_save);
  cout << "Depth : " << argv[1] << " Minimum Sample : " << argv[2] << " train : " << argv[6] << " test : " << argv[7] << endl;
  cout << "Test Decission Tree : " << endl;
  dec_tree.test(df_test);

  // Tmy_svm my_svm;

  // char *endptr;
  // double gamma = strtod(argv[2], &endptr);
  // double nu = strtod(argv[3], &endptr);

  // my_svm.train(df_train,gamma,nu);
  // my_svm.save_model(argv[7]);

  // my_svm.load_model(argv[7]);
  // my_svm.test(df_test);

  return 0;
}

