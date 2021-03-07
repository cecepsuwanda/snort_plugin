
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
//#include "Tbase_dataframe.h"
#include "Tdec_tree.h"

using namespace std;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
  Tdataframe df_train, df_test,df_tree;
  Tdec_tree dec_tree(stoi(argv[10]), stoi(argv[11]), stoi(argv[12]));
  char *endptr;

  string s(argv[8]);
  dec_tree.set_model_path(argv[8]);

  if (stoi(argv[9]) == 1)
  {
    for (const auto & file : directory_iterator(s))
      remove(file.path());

    df_train.read_data(argv[6]);
    df_train.read_data_type(argv[5]);
    df_train.set_id(0);
    df_train.info();

    cout << "Train : Jumlah Baris : " << df_train.getjmlrow() << " Jumlah Kolom : " << df_train.getjmlcol() << endl;
    cout << "Depth : " << argv[1] << " Minimum Sample : " << argv[2] << " gamma : " << argv[3] << " nu : " << argv[4] << " train : " << argv[6] << " test : " << argv[7] << endl;
    cout << "Start Train Decission Tree : " << endl;
    auto start = std::chrono::steady_clock::now();
    dec_tree.train(df_train, 0, 0, stoi(argv[2]), stoi(argv[1]), strtod(argv[3], &endptr), strtod(argv[4], &endptr));
    auto end = std::chrono::steady_clock::now();
    double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
    cout << "\nLama Training : " << elapsed_time / 60 << endl;
    cout << "End Train Decission Tree : " << endl;

    cout << "Start Prunning Decission Tree : " << endl;
    dec_tree.post_pruning(df_train, strtod(argv[3], &endptr), strtod(argv[4], &endptr));
    cout << "\nEnd Prunning Decission Tree : " << endl;

    dec_tree.save_tree();

    df_train.close_file();
  }

  df_test.read_data(argv[7]);
  df_test.read_data_type(argv[5]);

  cout << "Test : Jumlah Baris : " << df_test.getjmlrow() << " Jumlah Kolom : " << df_test.getjmlcol() << endl;

  if (stoi(argv[9]) == 0) {
    df_tree.read_data(s+ "/dtsvm_model.csv");
    dec_tree.read_tree(df_tree);
  }



  cout << "Depth : " << argv[1] << " Minimum Sample : " << argv[2] << " gamma : " << argv[3] << " nu : " << argv[4] << " train : " << argv[6] << " test : " << argv[7] << endl;
  cout << "Test Decission Tree : " << endl;
  dec_tree.test(df_test);

  return 0;
}

