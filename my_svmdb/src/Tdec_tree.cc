#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{
  
}

Tdec_tree::~Tdec_tree()
{

}


// void Tdec_tree::set_config(Tconfig v_config)
// {
//   config = v_config;
// }


void Tdec_tree::test(Tdataframe &df_test, tb_experiment &experiment)
{

  Tconf_metrix total_svm_conf_metrix;

  vector<int> idx_svm = df_test.get_idx_svm();
  for (auto i = idx_svm.begin(); i != idx_svm.end(); ++i)
  {
    pesan.cetak("---- Test no svm %d \n",*i);
    df_test.filter_by_idx_svm(*i);
    map<string,vector<string>> _table_svm = df_test.get_all_record_svm_map();

    Tmy_svm my_svm;
    Tconf_metrix svm_conf_metrix;

    string nm_model = global_config.svm_path + "/svm_model_" + to_string(*i) + ".csv";
    my_svm.load_model(nm_model);

    experiment.start_test_more_detail(*i);
    time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);

    for (auto j = _table_svm.begin(); j != _table_svm.end(); ++j)
    {      
      vector<string> vec_str = j->second;      
      string tmp_label_org = vec_str[vec_str.size()-1];     
      
      string tmp_label_svm = my_svm.guess(vec_str);
      df_test.set_label_svm(j->first,tmp_label_svm);      
      
      svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
      total_svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
    }

    svm_conf_metrix.kalkulasi();
    int jml_FN = svm_conf_metrix.get_FN("normal");
    int jml_FP = svm_conf_metrix.get_FP("normal");
    int jml_TN = svm_conf_metrix.get_TN("normal");
    int jml_TP = svm_conf_metrix.get_TP("normal");
    float f1 = svm_conf_metrix.get_F1("normal");

    experiment.hsl_more_detail(jml_FP, jml_FN, jml_TP, jml_TN, f1, *i);
    df_test.detail_dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,id_more_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,*i);

    experiment.end_test_more_detail(*i);

  }

  total_svm_conf_metrix.kalkulasi();
  int jml_FN = total_svm_conf_metrix.get_FN("normal");
  int jml_FP = total_svm_conf_metrix.get_FP("normal");
  int jml_TN = total_svm_conf_metrix.get_TN("normal");
  int jml_TP = total_svm_conf_metrix.get_TP("normal");
  float f1 = total_svm_conf_metrix.get_F1("normal");

  experiment.hsl_svm(jml_FP, jml_FN, jml_TP, jml_TN, f1); 

  df_test.dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt);
  
  Tconf_metrix tmp_conf_metrix; 
  df_test.dtsvm_conf_metrix(global_config.id_experiment,global_config.id_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,tmp_conf_metrix);

  jml_FN = tmp_conf_metrix.get_FN("normal");
  jml_FP = tmp_conf_metrix.get_FP("normal");
  jml_TN = tmp_conf_metrix.get_TN("normal");
  jml_TP = tmp_conf_metrix.get_TP("normal");
  f1 = tmp_conf_metrix.get_F1("normal");

  experiment.hsl(jml_FP, jml_FN, jml_TP, jml_TN, f1);

}




void Tdec_tree::clear_worker(size_t limit)
{

  bool pass = limit == 0  ? true : worker.size() == limit ;

  if (pass)
  {
    //cetak("{clear worker}");
    for (std::thread & th : worker)
    {
      // If thread Object is Joinable then Join that thread.
      if (th.joinable())
        th.join();
    }

    worker.clear();
    worker.shrink_to_fit();
  }
}
