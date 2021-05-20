
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"

using namespace std;
using std::experimental::filesystem::directory_iterator;

// void coba(Tdataframe &df_tmp, int idx, float & overall_metric, string & split_value)
// {

//   //df.set_shared_memory_on();
//   // Tdataframe df_tmp(&config);
//   // df_tmp.read_data(config.f_train);
//   // df_tmp.read_data_type(config.f_datatype);
//   //df_tmp = df;

//   string current_split_value = "-1";
//   float current_overall_metric = -1;

//   //df_tmp.get_col_pot_split(idx);
//   df_tmp.calculate_overall_metric(idx, current_overall_metric, current_split_value);

//   overall_metric = current_overall_metric;
//   split_value = current_split_value;

//   //df_tmp.clear_memory();
//   //df_tmp.close_file();
// }

int main(int argc, char *argv[])
{
  char *endptr;
  Tconfig config;

  config.f_datatype = argv[9];

  config.path_model = argv[11];
  config.svm_path = config.path_model + "/" + argv[12];

  config.save_train = stoi(argv[13]) == 1;
  config.save_test = stoi(argv[14]) == 1;
  config.use_credal = stoi(argv[15]) == 1;
  config.credal_s = strtod(argv[16], &endptr);
  config.limited = stoi(argv[17]) == 1;
  config.threshold = stoi(argv[18]);

  config.train_svm = stoi(argv[2]) == 1;
  config.feature_selection = stoi(argv[3]) == 1;
  config.normal_only = stoi(argv[4]) == 1;


  config.gamma = strtod(argv[7], &endptr);
  config.nu = strtod(argv[8], &endptr);

  config.depth = stoi(argv[5]);
  config.min_sample = stoi(argv[6]);


  if (stoi(argv[1]) == 0)
  {
    config.f_train = argv[10];

    vector<thread> worker;

    // config.search_uniqe_val = true;

    // Tdataframe df_train(&config);
    // df_train.read_data(config.f_train);
    // df_train.read_data_type(config.f_datatype);
    // df_train.info();

    // float overall_metric[42];
    // string split_value[42];

    // for (int i = 0; i < df_train.get_jml_valid_attr(); ++i)
    // {
    //   cout << i << endl;
    //   worker.push_back(thread(&coba, ref(df_train), df_train.get_valid_attr(i), ref(overall_metric[i]), ref(split_value[i])));
    // }

    // if (worker.size() > 0)
    // {
    //   cout << "Join" << endl;
    //   int i = 0;
    //   for (std::thread & th : worker)
    //   {
    //     // If thread Object is Joinable then Join that thread.
    //     if (th.joinable()) {
    //       th.join();
    //       cout << overall_metric[i] << " " << split_value[i] << endl;
    //     }
    //     i++;
    //   }

    //   worker.clear();
    //   worker.shrink_to_fit();

    // }


    // vector<vector<string>> table = df_train.get_all_record_svm();
    // cout << table.size() << endl;
    // table = df_train.get_all_record();
    // cout << table.size() << endl;
    // table = df_train.get_all_record_svm();
    // cout << table.size() << endl;
    // table = df_train.get_all_record();
    // cout << table.size() << endl;
    // table = df_train.get_all_record_svm();
    // cout << table.size() << endl;

    

    // df_train.clear_memory();
    // df_train.close_file();


    Tdec_tree dec_tree(&config);

    string tmp_str = config.path_model + "/dtsvm_model.csv";
    remove(tmp_str.c_str());

    if (config.save_train == 1) {
      for (const auto & file : directory_iterator(config.path_model + "/train"))
        remove(file.path());
    }

    for (const auto & file : directory_iterator(config.svm_path))
      remove(file.path());

    dec_tree.build_tree();
  } else {
    if (stoi(argv[1]) == 1)
    {

      config.f_test = argv[10];
      config.normal_only = false;

      Tdec_tree dec_tree(&config);

      if (config.save_test == 1) {
        for (const auto & file : directory_iterator(config.path_model + "/test"))
          remove(file.path());
      }

      dec_tree.read_tree();
      dec_tree.test();

    } else {
      if (stoi(argv[1]) == 2)
      {

        config.f_train = argv[10];

        Tdec_tree dec_tree(&config);

        for (const auto & file : directory_iterator(config.svm_path))
          remove(file.path());

        dec_tree.read_tree();
        dec_tree.learn_svm();
      }
    }
  }



  return 0;
}

