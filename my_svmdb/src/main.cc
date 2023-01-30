#include <iostream>
#include <experimental/filesystem>
#include "Tdec_tree.h"
#include "Tconf_metrix.h"
#include "tb_experiment.h"
#include "Tdt_learn_svm.h"
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

  config.svm_path = config.path_model + "/" + argv[9];

  double gamma_awal  = strtod(argv[3], &endptr);
  double gamma_akhir = strtod(argv[4], &endptr);
  double gamma_step  = strtod(argv[5], &endptr);

  double nu_awal = strtod(argv[6], &endptr);
  double nu_akhir = strtod(argv[7], &endptr);
  double nu_step = strtod(argv[8], &endptr);

  config.id_experiment_dt = 1674635983;
  config.id_detail_experiment_dt = 1674637223;

  tb_experiment experiment;
  train_test_data train_test = experiment.get_train_test_data(config.id_experiment_dt);


  Tdataframe df_train(&config);
  df_train.set_dataset(train_test.id_dt_train, train_test.jns_dt_train, train_test.partition_train);
  df_train.read_header_type();
  df_train.set_parent(0, 0, 0);
  df_train.set_branch(0, 0, 0);
  df_train.clone_dataset();
  df_train.stat_tabel(true, true, true);
  df_train.setjmltotalrow();

  Tdataframe df_test(&config);
  df_test.set_dataset(train_test.id_dt_test, train_test.jns_dt_test, train_test.partition_test);
  df_test.read_header_type();
  df_test.set_parent(0, 0, 0);
  df_test.set_branch(0, 0, 0);
  df_test.switch_to_test();
  df_test.clone_dataset();
  df_test.stat_tabel(false, false, false);

  Tdt_learn_svm learn_svm(&config);
  learn_svm.read_tree();

  learn_svm.set_svm_dataset(df_train);
  learn_svm.set_svm_dataset(df_test);

  Tdec_tree dec_tree(&config);

  experiment.insert_experiment(config.id_experiment_dt, config.id_detail_experiment_dt, gamma_awal, gamma_akhir, gamma_step, nu_awal, nu_akhir, nu_step);
  config.id_experiment = experiment.get_id_experiment();

  string str_id_experiment_dt = to_string(config.id_experiment_dt);
  string str_id_detail_experiment_dt = to_string(config.id_detail_experiment_dt);
  string str_id_experiment = to_string(config.id_experiment);
  
  string tmp_str = "hsl/"+str_id_experiment_dt;
  mkdir(tmp_str.c_str(), 0777);
  tmp_str = tmp_str+"/"+str_id_detail_experiment_dt;
  mkdir(tmp_str.c_str(), 0777);
  tmp_str = tmp_str+"/"+str_id_experiment;
  mkdir(tmp_str.c_str(), 0777);  

  for (double i = gamma_awal; i <= gamma_akhir; i += gamma_step)
  {

    config.gamma = i;
    for (double j = nu_awal; j <= nu_akhir; j += nu_step)
    {
      config.nu = j;

      experiment.insert_detail_experiment(config.id_experiment_dt, config.id_detail_experiment_dt, config.gamma, config.nu);
      config.id_detail_experiment = experiment.get_id_detail_experiment();

      string tmp_str1 = tmp_str + "/"+to_string(config.id_detail_experiment);
      mkdir(tmp_str1.c_str(), 0777);   

      config.svm_path = tmp_str1;

      learn_svm.learn_svm(df_train, experiment);

      experiment.end_train_start_test();

      dec_tree.test(df_test,experiment);

      experiment.end_test();

    }
  }

  experiment.end_experiment();
  df_train.close_file();
  df_test.close_file();  

  return 0;
}

