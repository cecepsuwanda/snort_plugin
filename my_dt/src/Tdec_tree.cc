#include "Tdec_tree.h"


Tdec_tree::Tdec_tree(Tconfig *v_config)
{
  idx_svm = 0;
  id_df = 1;

  config = v_config;
}

Tdec_tree::~Tdec_tree()
{

}

// void Tdec_tree::set_config(Tconfig v_config)
// {
//   config = v_config;
// }

bool Tdec_tree::is_pass(int opt, string value1, string value2)
{
  bool pass = false;

  Tmy_dttype tmp1(value1, ((opt == 0) or (opt == 1)));
  Tmy_dttype tmp2(value2, ((opt == 0) or (opt == 1)));

  switch (opt)
  {
  case 0 : {
    //cout << value1 << " " << value2 << endl;
    pass = tmp2 <= tmp1;
    break;
  }
  case 1 : {
    //cout << value1 << " " << value2 << endl;
    pass = tmp1 < tmp2;
    break;
  }
  case 2 : {
    pass = tmp1 == tmp2;
    break;
  }
  case 3 : {
    pass = tmp1 != tmp2;
    break;
  }
  }

  return pass;
}

int Tdec_tree::dfs(vector<string> &data, int treeIndex)
{
  if (tree[treeIndex].isLeaf)
  {
    auto it = vec_attr.find(tree[treeIndex].criteriaAttrIndex);
    if (it == vec_attr.end())
    {
      vec_attr.insert(pair<int, int>(tree[treeIndex].criteriaAttrIndex, 1));
    } else {
      it->second += 1;
    }

    return treeIndex;
  }

  int criteriaAttrIndex = tree[treeIndex].criteriaAttrIndex;

  string tmp_str = "";
  for (size_t i = 0; i < tree[treeIndex].children.size(); i++) {
    int next = tree[treeIndex].children[i];
    tmp_str += "{" + to_string(tree[next].opt) + "," + to_string(criteriaAttrIndex) + "," + tree[next].attrValue + "," + data[criteriaAttrIndex] + "},";
    // if (tree[next].isLeaf)
    // {
    //   return next;
    // } else {
    if (is_pass(tree[next].opt, tree[next].attrValue, data[criteriaAttrIndex]) ) {

      auto it = vec_attr.find(criteriaAttrIndex);
      if (it == vec_attr.end())
      {
        vec_attr.insert(pair<int, int>(criteriaAttrIndex, 1));
      } else {
        it->second += 1;
      }

      return dfs(data, next);
    }
    // }

  }

  cetak("\n");
  cetak(tmp_str.c_str());
  cetak("\n");

  return -1;
}

string Tdec_tree::guess(vector<string> &data)
{
  string label = "failed.";
  vec_attr.clear();
  int leafNode = dfs(data, 0);

  if (leafNode == -1) {
    return "dfs_failed.";
  } else {
    label = tree[leafNode].label;
    if (config->train_svm)
    {
      if ((label == "normal") and (tree[leafNode].idx_svm != -1))
      {
        Twrite_file tmp_wf;

        if (config->save_test) {
          tmp_wf.setnm_f(config->path_model + "/test/test_model_" + to_string(tree[leafNode].idx_svm) + ".csv");
        }
        //cetak("{v {model ");
        Tmy_svm my_svm(config);
        string nm_model = config->svm_path + "/svm_model_" + to_string(tree[leafNode].idx_svm) + ".csv";
        my_svm.load_model(nm_model);
        //cetak("save_model_");
        //cetak(to_string(tree[leafNode].idx_svm).c_str());
        //cetak(".csv} ");

        vector<string> tmp_data;
        string tmp_str = "";
        if (config->feature_selection)
        {

          for (auto itr = vec_attr.begin(); itr != vec_attr.end(); ++itr)
          {
            if (itr->first != -1) {
              tmp_data.push_back(data[itr->first]);
              tmp_str = tmp_str + data[itr->first] + ",";
            }
          }

        } else {
          for (size_t i = 0; i < (data.size() - 1); ++i)
          {
            tmp_data.push_back(data[i]);
            tmp_str = tmp_str + data[i] + ",";
          }
        }

        tmp_str = tmp_str + data[data.size() - 1];

        if (config->save_test)
        {
          tmp_wf.write_file(tmp_str);
          tmp_wf.close_file();
        }
        //cetak(" {guess ");
        label = my_svm.guess(tmp_data);
        //cetak("}");

        tmp_data.clear();
        tmp_data.shrink_to_fit();

      }
    }

  }
  return label;
}

void Tdec_tree::thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &dt_svm_conf_metrix)
{
  // std::mutex v_mutex;

  // std::lock_guard<std::mutex> lock(v_mutex);

  for (int i = 0; i < table.size(); ++i)
  {
    dt_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
    dt_svm_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
    conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
  }


}


void Tdec_tree::thread_test_svm(int idx_svm, string label, vector<vector<string>> table, Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &svm_conf_metrix, Tconf_metrix &dt_svm_conf_metrix)
{
  // std::mutex v_mutex;
  // std::lock_guard<std::mutex> lock(v_mutex);

  Twrite_file tmp_wf;

  if (config->save_test) {
    tmp_wf.setnm_f(config->path_model + "/test/test_model_" + to_string(idx_svm) + ".csv");
  }

  Tmy_svm my_svm(config);

  string nm_model = config->svm_path + "/svm_model_" + to_string(idx_svm) + ".csv";
  my_svm.load_model(nm_model);

  vector<string> tmp_data;
  string tmp_str;
  for (int i = 0; i < table.size(); ++i)
  {
    tmp_str = "";
    for (int j = 0; j < (table[i].size() - 1); ++j)
    {
      tmp_data.push_back(table[i][j]);
      tmp_str = tmp_str + table[i][j] + ",";
    }

    string tmp_label = label;
    tmp_label = my_svm.guess(tmp_data);

    //table[i].push_back(tmp_label);

    //v_mutex.lock();
    svm_conf_metrix.add_jml(table[i][table[i].size() - 1], tmp_label, 1);
    dt_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
    dt_svm_conf_metrix.add_jml(table[i][table[i].size() - 1], tmp_label, 1);

    conf_metrix.add_jml(table[i][table[i].size() - 1], tmp_label, 1);
    //v_mutex.unlock();

    tmp_str = tmp_str + table[i][table[i].size() - 1] + "," + tmp_label;

    if (config->save_test)
    {
      tmp_wf.write_file(tmp_str);
    }

    tmp_data.clear();
    tmp_data.shrink_to_fit();
  }

  my_svm.destroy_model();

  if (config->save_test)
  {
    tmp_wf.close_file();
  }

  //std::lock_guard<std::mutex> lock(v_mutex);

  // for (int i = 0; i < table.size(); ++i)
  // {

// v_mutex.lock();
  // svm_conf_metrix.add_jml(table[i][table[i].size() - 2], table[i][table[i].size() - 1], 1);
  // dt_conf_metrix.add_jml(table[i][table[i].size() - 2], label, 1);
  // dt_svm_conf_metrix.add_jml(table[i][table[i].size() - 2], table[i][table[i].size() - 1], 1);

  // conf_metrix.add_jml(table[i][table[i].size() - 2], table[i][table[i].size() - 1], 1);
  // v_mutex.unlock();

  // }



}

void Tdec_tree::test_dfs(int node_index , Tdataframe &df_test, Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &svm_conf_metrix, Tconf_metrix &dt_svm_conf_metrix)
{
  //cetak("%d", node_index);
  if (tree[node_index].isLeaf)
  {

    string label = tree[node_index].label;

    if ((config->train_svm) and (label == "normal") and (tree[node_index].idx_svm != -1))
    {
      //clear_worker(1);
      //cetak("+{v { %d %d }", tree[node_index].idx_svm, df_test.getjmlrow_svm());
      _table_svm = df_test.get_all_record_svm();
      thread_test_svm(tree[node_index].idx_svm, label, _table_svm, conf_metrix, dt_conf_metrix, svm_conf_metrix, dt_svm_conf_metrix);
      //worker.push_back(thread(&Tdec_tree::thread_test_svm, tree[node_index].idx_svm, ref(config), label, _table_svm, ref(conf_metrix), ref(dt_conf_metrix), ref(svm_conf_metrix), ref(dt_svm_conf_metrix)));
      //cetak("}");
      _table_svm.clear();
      _table_svm.shrink_to_fit();
    } else {
      //clear_worker(1);
      //cetak("+{A { %d }}", df_test.getjmlrow());
      _table_attack = df_test.get_all_record();
      worker.push_back(thread(&Tdec_tree::thread_test_attack, label, _table_attack, ref(conf_metrix), ref(dt_conf_metrix), ref(dt_svm_conf_metrix)));
      _table_attack.clear();
      _table_attack.shrink_to_fit();
    }
    df_test.clear_memory();
    //cetak("\n");
  } else {
    //cetak("|?");

    int left = tree[node_index].children[0];
    int right = tree[node_index].children[1];

    vector<string> opt;
    opt.push_back("<=");
    opt.push_back("<");
    opt.push_back("==");
    opt.push_back("!=");

    Tdataframe df_left, df_right;
    df_left = df_test;
    df_right = df_test;
    df_test.clear_memory();

    //clear_worker(0);

    if (left != -1) {

      //cetak("->");
      //cetak("[%d %d %s %s ", node_index , tree[node_index].criteriaAttrIndex, opt[tree[left].opt].c_str(), tree[left].attrValue.c_str());
      df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);

      //cetak("%d ]", df_left.getjmlrow());
      if (df_left.getjmlrow() > 0) {
        //cetak("->");
        //cetak("[%d %d %s %s]", node_index , tree[node_index].criteriaAttrIndex, opt[tree[left].opt].c_str(), tree[left].attrValue.c_str());
        test_dfs(left, df_left, conf_metrix, dt_conf_metrix, svm_conf_metrix, dt_svm_conf_metrix);
      } else {
        //cetak("\n");
      }
      df_left.clear_memory();
    }

    //clear_worker(0);

    if (right != -1) {
      //cetak("<-");
      //cetak("[%d %d %s %s ", node_index, tree[node_index].criteriaAttrIndex, opt[tree[right].opt].c_str(), tree[right].attrValue.c_str());
      df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
      //cetak("%d ]", df_right.getjmlrow());
      if (df_right.getjmlrow() > 0) {
        //cetak("<-");
        //cetak("[%d %d %s %s]", node_index, tree[node_index].criteriaAttrIndex, opt[tree[right].opt].c_str(), tree[right].attrValue.c_str());
        test_dfs(right, df_right, conf_metrix, dt_conf_metrix, svm_conf_metrix, dt_svm_conf_metrix);
      } else {
        //cetak("\n");
      }
      df_right.clear_memory();
    }

    //clear_worker(0);

  }

  if (node_index == 0)
  {
    clear_worker(0);
  }

}

void Tdec_tree::test()
{
  Tdataframe df(config);
  df.read_data(config->f_test);
  df.read_data_type(config->f_datatype);
  //df.info();


  //cetak("Test : Jumlah Baris : %d Jumlah Kolom : %d \n", df.getjmlrow(), df.getjmlcol());
  //cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.4f credal : %.4f test : %s \n", config->depth, config->min_sample, config->gamma, config->nu, config->credal_s , config->f_test.c_str());
  //cetak("Test Decission Tree : \n");

  Tconf_metrix conf_metrix, dt_conf_metrix, svm_conf_metrix, dt_svm_conv_metrix;

  svm_conf_metrix.add_konversi_asli("known", "normal");
  dt_conf_metrix.add_konversi_asli("unknown", "known");
  dt_svm_conv_metrix.add_konversi_asli("unknown", "known");
  dt_svm_conv_metrix.add_konversi_tebakan("unknown", "known");


  {
    Timer timer;
    test_dfs(0, df, conf_metrix, dt_conf_metrix, svm_conf_metrix, dt_svm_conv_metrix);
    //double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
  }

  //cetak("End Test Decission Tree : \n");

  df.close_file();

  //cout << " Jumlah Data : " << jml_data << " Prediksi Tepat : " << tepat << " Failed : " << failed << " Prosentase : " << ((tepat / (double) jml_data) * 100) << endl;
  // cout << "All Metrix : " << endl;
  // conf_metrix.kalkulasi();
  // cout << conf_metrix << endl << endl;
  // conf_metrix.save(config->path_model + "/all_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);

  // cout << "Dession Tree Metrix : " << endl;
  dt_conf_metrix.kalkulasi();
  cetak("Depth : %d Minimum Sample : %d credal : %.4f FP : %d FN : %d \n", config->depth, config->min_sample, config->credal_s,dt_conf_metrix.get_FN("known"),dt_conf_metrix.get_FP("known"));
  //cout << dt_conf_metrix << endl << endl;
  // dt_conf_metrix.save(config->path_model + "/dt_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);

  if (config->train_svm)
  {
    cout << "SVM Metrix : " << endl;
    svm_conf_metrix.kalkulasi();
    cout << svm_conf_metrix << endl << endl;
    svm_conf_metrix.save(config->path_model + "/svm_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);
  }

  // cout << "SVM & Dession Tree Metrix : " << endl;
  // dt_svm_conv_metrix.kalkulasi();
  // cout << dt_svm_conv_metrix << endl << endl;
  // dt_svm_conv_metrix.save(config->path_model + "/dtsvm_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);


}

void Tdec_tree::read_tree()
{
  vector<string> tmp_data;
  Tdataframe df;
  df.read_data(config->path_model + "/dtsvm_model.csv");

  df.reset_file();
  while (!df.is_eof()) {

    tmp_data = df.get_record();

    //cout << tmp_data.size() << endl;

    Node newnode;
    //cout << tmp_data[0] << endl;
    newnode.criteriaAttrIndex = tmp_data[0] == "-1" ?  -1 : stoi(tmp_data[0]);
    newnode.attrValue = tmp_data[1];
    newnode.label = tmp_data[2];
    //cout << tmp_data[2] << endl;
    newnode.treeIndex = tmp_data[3] == "-1" ? -1 : stoi(tmp_data[3]);
    newnode.isLeaf = tmp_data[4] == "1";
    //cout << tmp_data[4] << endl;
    newnode.opt = tmp_data[5] == "-1" ? -1 : stoi(tmp_data[5]);
    //cout << tmp_data[5] << endl;
    newnode.children.push_back(tmp_data[6] == "-1" ? -1 :  stoi(tmp_data[6]));
    //cout << tmp_data[6] << endl;
    newnode.children.push_back(tmp_data[7] == "-1" ? -1 :  stoi(tmp_data[7]));

    newnode.idx_svm = tmp_data[8] == "-1" ? -1 : stoi(tmp_data[8]);

    if (idx_svm < newnode.idx_svm)
    {
      idx_svm = newnode.idx_svm;
    }

    tree.push_back(newnode);

    df.next_record();
  }

  df.close_file();

}

void Tdec_tree::clear_worker(int limit)
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
