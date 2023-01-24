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

  config.id_experiment_dt = 1674264280;
  config.id_detail_experiment_dt = 1674264984;

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


  //int jml = 0;

  // for (const auto & file : directory_iterator(config.path_model + "/train"))
  // {

  //   string train_file = file.path().filename();
  //   string test_file = train_file;
  //   test_file.replace(0, 5, "test");
  //   string svm_model_file = train_file;
  //   svm_model_file.replace(0, 5, "svm");

  //   float gamma_max = 0.0;
  //   float nu_max = 0.0;

  //   float F1_Train_max = -1.0;
  //   float F1_Test_max = -1.0;

  //   float F1_Train = 0.0;
  //   float F1_Test = 0.0;

  //   Tdataframe df_train(&config);
  //   df_train.read_data(file.path());
  //   df_train.read_data_type(config.f_datatype);
  //   //df_train.info();

  //   path v_path(config.path_model + "/test/" + test_file);

  //   Tdataframe df_test(&config);
  //   if (exists(v_path)) {
  //     df_test.read_data(config.path_model + "/test/" + test_file);
  //     df_test.read_data_type(config.f_datatype);
  //     //df_test.info();
  //   }

  //   if (df_train.getjmlrow_svm() >= 1)
  //   {
  //     for (double i = gamma_awal; i <= gamma_akhir; i += gamma_step)
  //     {
  //       if ((df_train.getjmlrow_svm() > 1000) and (F1_Test_max > 0.8))
  //       {
  //         break;
  //       }
  //       config.gamma = i;
  //       for (double j = nu_awal; j <= nu_akhir; j += nu_step)
  //       {
  //         config.nu = j;

  //         Tmy_svm my_svm(&config);
  //         my_svm.train(df_train.get_all_record_svm());

  //         Tconf_metrix conf_metrix_train;
  //         conf_metrix_train.add_konversi_asli("known", "inside");
  //         conf_metrix_train.add_konversi_asli("normal", "inside");
  //         conf_metrix_train.add_konversi_asli("unknown", "outside");

  //         my_svm.test(df_train, conf_metrix_train);
  //         F1_Train = conf_metrix_train.get_F1();

  //         if (exists(v_path)) {

  //           Tconf_metrix conf_metrix_test;
  //           conf_metrix_test.add_konversi_asli("known", "inside");
  //           conf_metrix_test.add_konversi_asli("normal", "inside");
  //           conf_metrix_test.add_konversi_asli("unknown", "outside");

  //           my_svm.test(df_test, conf_metrix_test);
  //           F1_Test = conf_metrix_test.get_F1();
  //         }

  //         if ((F1_Test > F1_Test_max) and (F1_Train > F1_Train_max))
  //         {
  //           gamma_max = i;
  //           nu_max = j;
  //           F1_Train_max = F1_Train;
  //           F1_Test_max = F1_Test;

  //           string tmp_str = config.svm_path + "/" + svm_model_file;
  //           remove(tmp_str.c_str());
  //           my_svm.save_model(tmp_str);

  //           if (df_train.getjmlrow_svm() > 1000)
  //           {
  //             cout <<"jml_data_train="<<df_train.getjmlrow_svm()<<",gamma=" << gamma_max << ",nu=" << nu_max << ",File_Train=" << train_file << ",F1_Train=" << F1_Train_max << ",File_Test=" << test_file << ",F1_Test=" << F1_Test_max << endl;
  //           }
  //         }
  //         // jml = jml + df_train.getjmlrow_svm();

  //         if ((df_train.getjmlrow_svm() > 1000) and (F1_Test_max > 0.8))
  //         {
  //           break;
  //         }
  //       }

  //     }

  //     cout <<"jml_data_train="<<df_train.getjmlrow_svm()<<",gamma=" << gamma_max << ",nu=" << nu_max << ",File_Train=" << train_file << ",F1_Train=" << F1_Train_max << ",File_Test=" << test_file << ",F1_Test=" << F1_Test_max << endl;
  //   }
  //   df_train.clear_memory();
  //   df_train.close_file();

  //   if (exists(v_path)) {
  //     df_test.clear_memory();
  //     df_test.close_file();
  //   }
  // }

  // cout << "jml = " << jml << endl;
  // conf_metrix.kalkulasi();
  // cout << conf_metrix << endl;

  return 0;
}

