#include "Tdec_tree.h"


Tdec_tree::Tdec_tree(Tconfig *v_config)
{

  config = v_config;
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
    df_test.filter_by_idx_svm(*i);
    vector<vector<string>> _table_svm = df_test.get_all_record_svm();

    Tmy_svm my_svm(config);
    Tconf_metrix svm_conf_metrix;

    string nm_model = config->svm_path + "/svm_model_" + to_string(*i) + ".csv";
    my_svm.load_model(nm_model);

    experiment.start_test_more_detail(*i);

    for (int j = 0; j < _table_svm.size(); ++j)
    {      
      vector<string> vec_str = _table_svm[j];      
      string tmp_label_org = vec_str[vec_str.size()-1];     
      
      string tmp_label_svm = my_svm.guess(_table_svm[j]);      
      
      svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
      total_svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
    }

    svm_conf_metrix.kalkulasi();
    int jml_FN = svm_conf_metrix.get_FN("normal");
    int jml_FP = svm_conf_metrix.get_FP("normal");
    int jml_TN = svm_conf_metrix.get_TN("normal");
    int jml_TP = svm_conf_metrix.get_TP("normal");
    float f1 = svm_conf_metrix.get_F1();

    experiment.hsl_more_detail(jml_FP, jml_FN, jml_TP, jml_TN, f1, *i);

    experiment.end_test_more_detail(*i);

  }

  total_svm_conf_metrix.kalkulasi();
  int jml_FN = total_svm_conf_metrix.get_FN("normal");
  int jml_FP = total_svm_conf_metrix.get_FP("normal");
  int jml_TN = total_svm_conf_metrix.get_TN("normal");
  int jml_TP = total_svm_conf_metrix.get_TP("normal");
  float f1 = total_svm_conf_metrix.get_F1();

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
