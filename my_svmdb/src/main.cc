#include <iostream>
#include "Tdec_tree.h"
#include "Tconf_metrix.h"
#include "tb_experiment.h"
#include "Tdt_learn_svm.h"
#include "Tmy_svm.h"

using namespace std;

string Tglobal_config::f_datatype;
string Tglobal_config::f_train;
string Tglobal_config::f_test;

string Tglobal_config::path_model;
string Tglobal_config::svm_path;

time_t Tglobal_config::id_experiment_dt;
time_t Tglobal_config::id_detail_experiment_dt;

time_t Tglobal_config::id_experiment;
time_t Tglobal_config::id_detail_experiment;

bool Tglobal_config::save_train;
bool Tglobal_config::save_test;

bool Tglobal_config::train_svm;
bool Tglobal_config::normal_only;

double Tglobal_config::gamma;
double Tglobal_config::nu;

time_t tb_experiment::_id_experiment;
time_t tb_experiment::_id_detail_experiment;


int main(int argc, char *argv[])
{
  char *endptr;
  Tpesan pesan;
  Tglobal_config global_config;
  global_config.init();

  // config.f_datatype = argv[2];
  // config.path_model = argv[1];

  // config.svm_path = config.path_model + "/" + argv[9];

  int id_dt_train  = stoi(argv[1]);
  int jns_dt_train = stoi(argv[2]);
  string partition_train = argv[3];
  int id_dt_test  = stoi(argv[4]);
  int jns_dt_test = stoi(argv[5]);
  string partition_test = argv[6];

  int depth  = stoi(argv[7]);
  int sample = stoi(argv[8]);
  double threshold = strtod(argv[9], &endptr);
  double credal = strtod(argv[10], &endptr);

  double gamma_awal  = strtod(argv[11], &endptr);
  double gamma_akhir = strtod(argv[12], &endptr);
  double gamma_step  = strtod(argv[13], &endptr);

  double nu_awal = strtod(argv[14], &endptr);
  double nu_akhir = strtod(argv[15], &endptr);
  double nu_step = strtod(argv[16], &endptr);

  bool is_experiment_svm = stoi(argv[17]) == 1;
  time_t id_experiment_svm = (time_t) atoll(argv[17]);
  bool is_detail_experiment_svm = stoi(argv[18]) == 1;
  time_t id_detail_experiment_svm = (time_t) atoll(argv[18]);

  // global_config.id_experiment_dt = (time_t) atoll(argv[1]);
  // global_config.id_detail_experiment_dt = (time_t) atoll(argv[2]);

  tb_experiment experiment;
  // train_test_data train_test = experiment.get_train_test_data_old(global_config.id_experiment_dt);
  train_test_data train_test = experiment.get_train_test_data(id_dt_train,jns_dt_train,partition_train,id_dt_test,jns_dt_test,partition_test,depth,sample,threshold,credal);
  
  global_config.id_experiment_dt = train_test.id_experiment_dt;
  global_config.id_detail_experiment_dt = train_test.id_detail_experiment_dt;

  pesan.cetak("Menyiapkan Data Latih :\n");

  Tdataframe df_train;
  df_train.set_dataset(train_test.id_dt_train, train_test.jns_dt_train, train_test.partition_train);
  df_train.read_header_type();
  df_train.set_parent(0, 0, 0);
  df_train.set_branch(0, 0, 0);
  df_train.clone_dataset();
  df_train.stat_tabel(true, true, true);
  df_train.setjmltotalrow();

  pesan.cetak("Menyiapkan Data Test :\n");

  Tdataframe df_test;
  df_test.set_dataset(train_test.id_dt_test, train_test.jns_dt_test, train_test.partition_test);
  df_test.read_header_type();
  df_test.set_parent(0, 0, 0);
  df_test.set_branch(0, 0, 0);
  df_test.switch_to_test();
  df_test.clone_dataset();
  df_test.stat_tabel(false, false, false);

  pesan.cetak("Baca Tree :\n");

  Tdt_learn_svm learn_svm;
  learn_svm.read_tree();

  pesan.cetak("Memberi nomor svm :\n");

  learn_svm.set_svm_dataset(df_train);
  learn_svm.set_svm_dataset(df_test);

  Tdec_tree dec_tree;

  if(is_experiment_svm)
  {
    experiment.set_id_experiment(id_experiment_svm);
  }
  else
  { 
    experiment.insert_experiment(global_config.id_experiment_dt, global_config.id_detail_experiment_dt, gamma_awal, gamma_akhir, gamma_step, nu_awal, nu_akhir, nu_step);
  }
  
  global_config.id_experiment = experiment.get_id_experiment();

  string str_id_experiment_dt = to_string(global_config.id_experiment_dt);
  string str_id_detail_experiment_dt = to_string(global_config.id_detail_experiment_dt);
  string str_id_experiment = to_string(global_config.id_experiment);

  string tmp_str = "hsl/" + str_id_experiment_dt;
  mkdir(tmp_str.c_str(), 0777);
  tmp_str = tmp_str + "/" + str_id_detail_experiment_dt;
  mkdir(tmp_str.c_str(), 0777);
  tmp_str = tmp_str + "/" + str_id_experiment;
  mkdir(tmp_str.c_str(), 0777);

  for (double i = gamma_awal; i <= gamma_akhir; i += gamma_step)
  {

    global_config.gamma = i;
    for (double j = nu_awal; j <= nu_akhir; j += nu_step)
    {
      global_config.nu = j;

      pesan.cetak("gamma = %f nu = %f \n", i, j);

      if(is_detail_experiment_svm)
      {
         experiment.set_id_detail_experiment(id_detail_experiment_svm);
      }
      else{
         experiment.insert_detail_experiment(global_config.id_experiment_dt, global_config.id_detail_experiment_dt, global_config.gamma, global_config.nu);
      }

      global_config.id_detail_experiment = experiment.get_id_detail_experiment();

      string tmp_str1 = tmp_str + "/" + to_string(global_config.id_detail_experiment);
      mkdir(tmp_str1.c_str(), 0777);

      global_config.svm_path = tmp_str1;

      learn_svm.learn_svm(df_train);

      experiment.end_train_start_test();

      dec_tree.test(df_test, experiment);

      experiment.end_test();

    }
  }

  experiment.end_experiment();
  df_train.close_file();
  df_test.close_file();

  return 0;
}

