
#include <chrono>
#include <iostream>
#include "Tdec_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
  Tdataframe df_train, df_test, df_save;
  Tdec_tree dec_tree;
  char *endptr;

  df_save.read_data(argv[8]);

  if (stoi(argv[9]) == 1)
  {
    df_train.read_data(argv[6]);
    df_train.read_data_type(argv[5]);

    cout << "Train : Jumlah Baris : " << df_train.getjmlrow() << " Jumlah Kolom : " << df_train.getjmlcol() << endl;
    cout << "Depth : " << argv[1] << " Minimum Sample : " << argv[2] << " gamma : " << argv[3] << " nu : " << argv[4] << " train : " << argv[6] << " test : " << argv[7] << endl;
    cout << "Start Train Decission Tree : " << endl;
    auto start = std::chrono::steady_clock::now();
    dec_tree.train(df_train, 0, 0, stoi(argv[2]), stoi(argv[1]), strtod(argv[3], &endptr), strtod(argv[4], &endptr));
    auto end = std::chrono::steady_clock::now();
    double elapsed_time = double(std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count()); 
    cout << "Lama Training : " << elapsed_time/1e9 <<endl;
    cout << "End Train Decission Tree : " << endl;

    dec_tree.save_tree(df_save);
  }

  df_test.read_data(argv[7]);
  df_test.read_data_type(argv[5]);

  cout << "Test : Jumlah Baris : " << df_test.getjmlrow() << " Jumlah Kolom : " << df_test.getjmlcol() << endl;

  if (stoi(argv[9]) == 0) {
    dec_tree.read_tree(df_save);
  }

  cout << "Depth : " << argv[1] << " Minimum Sample : " << argv[2] << " gamma : " << argv[3] << " nu : " << argv[4] << " train : " << argv[6] << " test : " << argv[7] << endl;
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

