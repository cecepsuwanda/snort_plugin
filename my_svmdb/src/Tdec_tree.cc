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

Thsl_test Tdec_tree::thread_test_svm(map<string,vector<string>> table, int v_idx_svm,tb_experiment v_experiment)
{
  sleep(2);

  Thsl_test hsl;
  Tglobal_config global_config;
  Tmy_svm my_svm;
  Tconf_metrix svm_conf_metrix;

  // tb_dataset data;
  // data.switch_to_test();

    hsl.idx_svm = v_idx_svm;

    string str_id_experiment_dt = to_string(global_config.id_experiment_dt);
    string str_id_detail_experiment_dt = to_string(global_config.id_detail_experiment_dt);
    string str_id_experiment = to_string(global_config.id_experiment);
    
    string opt_nm_model = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/optimal/svm_model_" + to_string(v_idx_svm) + ".csv";
    
    path v_path(opt_nm_model);
    if (exists(v_path))
    {
      my_svm.load_model(opt_nm_model);     
    }else{
      string nm_model = global_config.svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv";
      my_svm.load_model(nm_model);
    } 

    
    v_experiment.start_test_more_detail(v_idx_svm);
    time_t id_more_detail_experiment = v_experiment.get_id_more_detail_experiment(v_idx_svm);
    hsl.id_more_detail_experiment = id_more_detail_experiment;

    for (auto j = table.begin(); j != table.end(); ++j)
    {      
      vector<string> vec_str = j->second;      
      string tmp_label_org = vec_str[vec_str.size()-1];     
      
      string tmp_label_svm = my_svm.guess(vec_str);
      
      
      //data.set_label_svm(j->first,tmp_label_svm);

       auto itr = hsl.data.find(j->first);
       if (itr == hsl.data.end()) {
          hsl.data.insert({j->first,tmp_label_svm});
       }

      svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
    }



    svm_conf_metrix.kalkulasi();
    int jml_FN = svm_conf_metrix.get_FN("normal");
    int jml_FP = svm_conf_metrix.get_FP("normal");
    int jml_TN = svm_conf_metrix.get_TN("normal");
    int jml_TP = svm_conf_metrix.get_TP("normal");
    float f1 = svm_conf_metrix.get_F1("normal");

    v_experiment.hsl_more_detail(jml_FP, jml_FN, jml_TP, jml_TN, f1, v_idx_svm);
    //data.detail_dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,id_more_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,v_idx_svm);

    v_experiment.end_test_more_detail(v_idx_svm);
    
    sleep(2);

  return hsl;
}


void Tdec_tree::test(Tdataframe &df_test, tb_experiment &experiment)
{

  Tconf_metrix total_svm_conf_metrix;
  vector<future<Thsl_test>> async_worker;

  vector<int> idx_svm = df_test.get_idx_svm();
  for (auto i = idx_svm.begin(); i != idx_svm.end(); ++i)
  {
    pesan.cetak("---- Test no svm %d \n",*i);
    df_test.filter_by_idx_svm(*i);
    map<string,vector<string>> _table_svm = df_test.get_all_record_svm_map();

    async_worker.push_back(async(std::launch::async, &Tdec_tree::thread_test_svm, _table_svm, *i,experiment));

    // _table_svm.clear();
    
    // Tmy_svm my_svm;
    // Tconf_metrix svm_conf_metrix;
    
    // string str_id_experiment_dt = to_string(global_config.id_experiment_dt);
    // string str_id_detail_experiment_dt = to_string(global_config.id_detail_experiment_dt);
    // string str_id_experiment = to_string(global_config.id_experiment);
    
    // string opt_nm_model = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/optimal/svm_model_" + to_string(*i) + ".csv";
    
    // path v_path(opt_nm_model);
    // if (exists(v_path))
    // {
    //   my_svm.load_model(opt_nm_model);     
    // }else{
    //   string nm_model = global_config.svm_path + "/svm_model_" + to_string(*i) + ".csv";
    //   my_svm.load_model(nm_model);
    // } 

    
    // experiment.start_test_more_detail(*i);
    // time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);

    // for (auto j = _table_svm.begin(); j != _table_svm.end(); ++j)
    // {      
    //   vector<string> vec_str = j->second;      
    //   string tmp_label_org = vec_str[vec_str.size()-1];     
      
    //   string tmp_label_svm = my_svm.guess(vec_str);
    //   df_test.set_label_svm(j->first,tmp_label_svm);      
      
    //   svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
    //   total_svm_conf_metrix.add_jml("normal", tmp_label_svm, 1);
      bool pass = (async_worker.size() % 4)==0 ;
      
       if (pass)
        {
          //Thsl_test hsl;
          future_status status;
          for (future<Thsl_test> &th : async_worker)
          {
            status =  th.wait_for(chrono::seconds(2));

            // hsl = th.get();
            
            // for (auto j = hsl.data.begin(); j != hsl.data.end(); ++j)
            // {
            //   df_test.set_label_svm(j->first,j->second);
            //   total_svm_conf_metrix.add_jml("normal", j->second, 1);
            // }

            // df_test.detail_dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,hsl.id_more_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,hsl.idx_svm); 

          }

          // async_worker.clear();
          // async_worker.shrink_to_fit();
        }  

      

    // }

    // svm_conf_metrix.kalkulasi();
    // int jml_FN = svm_conf_metrix.get_FN("normal");
    // int jml_FP = svm_conf_metrix.get_FP("normal");
    // int jml_TN = svm_conf_metrix.get_TN("normal");
    // int jml_TP = svm_conf_metrix.get_TP("normal");
    // float f1 = svm_conf_metrix.get_F1("normal");

    // experiment.hsl_more_detail(jml_FP, jml_FN, jml_TP, jml_TN, f1, *i);
    // df_test.detail_dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,id_more_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,*i);

    // experiment.end_test_more_detail(*i);

  }

       if (async_worker.size()>0)
       { 
          Thsl_test hsl;
          for (future<Thsl_test> &th : async_worker)
          {
            hsl = th.get();
            for (auto j = hsl.data.begin(); j != hsl.data.end(); ++j)
            {
              df_test.set_label_svm(j->first,j->second);
              total_svm_conf_metrix.add_jml("normal", j->second, 1);
            } 
            df_test.detail_dtsvm_stat(global_config.id_experiment,global_config.id_detail_experiment,hsl.id_more_detail_experiment,global_config.id_experiment_dt,global_config.id_detail_experiment_dt,hsl.idx_svm); 
          }

          async_worker.clear();
          async_worker.shrink_to_fit();
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
