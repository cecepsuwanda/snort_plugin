
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;

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


  for (int i = 100; i <= 100; i += 2)
  {
    config.min_sample = i;
    for (double k = 0.5; k <= 0.5; k += 0.5)
    {
      config.use_credal = k != 0.0;
      config.credal_s = k;

      int prev_jml_FP = 0;
      int prev_jml_FN = 0;
      int jml_sama = 0;

      for (int j = 9; j <= 9; ++j)
      {
        config.depth = j;
        config.search_uniqe_val = true;
        config.prunning = true;


        Tdt_build dec_tree_build(&config);

        string tmp_str = config.path_model + "/dtsvm_model_" + to_string(config.depth) + "_" + to_string(config.min_sample) + "_" + to_string(config.threshold) + ".csv";
        remove(tmp_str.c_str());

        // if (j > 2) {
        //   tmp_str = config.path_model + "/dtsvm_model_" + to_string(config.depth - 1) + "_" + to_string(config.min_sample) + "_" + to_string(config.threshold) + ".csv";
        // }

        // path v_path(tmp_str);

        // if ((j > 2) and exists(v_path))
        // {
        //   dec_tree_build.read_tree(tmp_str);
        //   dec_tree_build.build_from_prev_tree(df_train, j - 1);
        // } else {
          dec_tree_build.build_tree(df_train);
        // }

        config.search_uniqe_val = false;
        Tdec_tree dec_tree_test(&config);
        dec_tree_test.read_tree();
        Tconf_metrix dt_conf_metrix;
        dec_tree_test.test(df_test, dt_conf_metrix);


        int jml_FN = dt_conf_metrix.get_FN("known");
        int jml_FP = dt_conf_metrix.get_FP("known");

        jml_sama++;
        if ((prev_jml_FN != jml_FN) or (prev_jml_FP != jml_FP))
        {
          jml_sama = 0;
          prev_jml_FP = jml_FP;
          prev_jml_FN = jml_FN;
        }

        if (jml_sama >= 5)
        {
          break;
        }

      }

      config.search_uniqe_val = true;
      df_train.stat_tabel();

    }
  }

  df_train.close_file();
  df_test.close_file();

  return 0;
}
