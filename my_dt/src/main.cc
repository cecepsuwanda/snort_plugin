
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

  config.f_datatype = argv[13];
  config.path_model = argv[16];

  double credal_s_awal  = strtod(argv[7], &endptr);
  double credal_s_akhir = strtod(argv[8], &endptr);
  double credal_s_step  = strtod(argv[9], &endptr);

  int depth_awal = stoi(argv[1]);
  int depth_akhir = stoi(argv[2]);
  int depth_step = stoi(argv[3]);

  int min_sample_awal  = stoi(argv[4]);
  int min_sample_akhir = stoi(argv[5]);
  int min_sample_step  = stoi(argv[6]);

  int threshold_awal  = stoi(argv[10]);
  int threshold_akhir = stoi(argv[11]);
  int threshold_step  = stoi(argv[12]);

  config.f_train = argv[14];
  config.f_test = argv[15];

  config.search_uniqe_val = true;
  Tdataframe df_train(&config);
  df_train.read_data(config.f_train);
  df_train.read_data_type(config.f_datatype);

  config.search_uniqe_val = false;
  Tdataframe df_test(&config);
  df_test.read_data(config.f_test);
  df_test.read_data_type(config.f_datatype);

  for (int l = threshold_awal; l <= threshold_akhir; l += threshold_step)
  {
    config.limited = l != 0;
    config.threshold = l;
    for (int i = min_sample_awal; i <= min_sample_akhir; i += min_sample_step)
    {
      config.min_sample = i;
      for (double k = credal_s_awal; k <= credal_s_akhir; k += credal_s_step)
      {
        config.use_credal = k != 0.0;
        config.credal_s = k;

        int prev_jml_FP = 0;
        int prev_jml_FN = 0;
        int jml_sama = 0;

        for (int j = depth_awal; j <= depth_akhir; j += depth_step)
        {
          config.depth = j;
          config.search_uniqe_val = true;
          config.prunning = true;


          Tdt_build dec_tree_build(&config);

          string tmp_str = config.path_model + "/dtsvm_model_" + to_string(config.depth) + "_" + to_string(config.min_sample) + "_" + to_string(config.threshold) + ".csv";
          remove(tmp_str.c_str());

          if (j > depth_awal) {
            tmp_str = config.path_model + "/dtsvm_model_" + to_string(config.depth - 1) + "_" + to_string(config.min_sample) + "_" + to_string(config.threshold) + ".csv";
          }

          path v_path(tmp_str);

          df_train.set_config(&config);

          if ((j > depth_awal) and exists(v_path))
          {
            dec_tree_build.read_tree(tmp_str);
            dec_tree_build.build_from_prev_tree(df_train, j - 1);
          } else {
            dec_tree_build.build_tree(df_train);
          }

          df_test.set_config(&config);
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
        df_train.set_config(&config);
        df_train.stat_tabel();

      }
    }
  }

  df_train.close_file();
  df_test.close_file();

  return 0;
}
