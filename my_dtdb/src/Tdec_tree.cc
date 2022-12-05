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
          //tmp_wf.setnm_f(config->path_model + "/test/test_model_" + to_string(tree[leafNode].idx_svm) + ".csv");
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

        for (size_t i = 0; i < (data.size() - 1); ++i)
        {
          tmp_data.push_back(data[i]);
          tmp_str = tmp_str + data[i] + ",";
        }


        tmp_str = tmp_str + data[data.size() - 1];

        if (config->save_test)
        {
          // tmp_wf.write_file(tmp_str);
          // tmp_wf.close_file();
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

void Tdec_tree::thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &dt_conf_metrix)
{
  // std::mutex v_mutex;

  // std::lock_guard<std::mutex> lock(v_mutex);

  for (size_t i = 0; i < table.size(); ++i)
  {
    dt_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
  }

}



void Tdec_tree::test_dfs(int node_index , Tdataframe &df_test, Tconf_metrix &dt_conf_metrix, int counter)
{
  if (tree[node_index].isLeaf)
  {

    string label = tree[node_index].label;
    cetak("[%s %d]\n", label.c_str(), df_test.getjmlrow());
    //clear_worker(1);
    _table_attack = df_test.get_all_record();
    worker.push_back(thread(&Tdec_tree::thread_test_attack, label, _table_attack, ref(dt_conf_metrix)));
    _table_attack.clear();
    _table_attack.shrink_to_fit();
    //cetak("\n");
    df_test.clear_memory();
  } else {
    cetak("%d|?", counter);

    counter++;

    int left = tree[node_index].children[0];
    int right = tree[node_index].children[1];

    Tdataframe df_left, df_right;
    df_left = df_test;
    df_left.switch_parent_branch();
    df_left.set_branch(counter, 1);
    df_right = df_test;
    df_right.switch_parent_branch();
    df_right.set_branch(counter, 2);


    //clear_worker(0);

    if (left != -1) {
      df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
      if (df_left.getjmlrow() > 0) {
        test_dfs(left, df_left, dt_conf_metrix, counter);
      }
      df_left.clear_memory();
    }

    //clear_worker(0);
    if (right != -1) {
      df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
      if (df_right.getjmlrow() > 0) {
        test_dfs(right, df_right, dt_conf_metrix, counter);
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

void Tdec_tree::test(Tconf_metrix &dt_conf_metrix)
{

  Tdataframe df(config);
  df.set_dataset(config->id_dt_test, config->jns_dt_test, config->partition_test);
  df.read_header_type();
  df.set_parent(0, 0);
  df.set_branch(0, 0);
  df.clone_dataset();
  df.hit_label_stat_onoff();
  df.stat_tabel();

  //df.info();

  //Tconf_metrix dt_conf_metrix;
  dt_conf_metrix.add_konversi_asli("unknown", "known");

  {
    Timer timer;
    test_dfs(0, df, dt_conf_metrix, 0);
    //double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
  }

  df.close_file();

  dt_conf_metrix.kalkulasi();
  cetak("Depth=%d,Minimum_Sample=%d,credal=%.4f,threshold=%d,FP=%d,FN=%d,F1=%.4f \n", config->depth, config->min_sample, config->credal_s, config->threshold, dt_conf_metrix.get_FP("known"), dt_conf_metrix.get_FN("known"), dt_conf_metrix.get_F1());

  // dt_conf_metrix.save(config->path_model + "/dt_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);


}

void Tdec_tree::read_tree()
{
  vector<string> tmp_data;
  tb_tree dbtree;
  dbtree.baca_tree(config->id_experiment,config->id_detail_experiment);

  dbtree.reset_file();
  while (!dbtree.is_eof()) {

    tmp_data = dbtree.get_record();

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

    //   if (idx_svm < newnode.idx_svm)
    //   {
    //     idx_svm = newnode.idx_svm;
    //   }

    tree.push_back(newnode);

    dbtree.next_record();
  }  

  dbtree.close_file();
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
