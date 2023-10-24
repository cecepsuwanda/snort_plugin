#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{
  idx_svm = 0;
  id_df = 1;

  
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
    tmp_str += "{" + to_string(tree[next].opt) + "," + to_string(criteriaAttrIndex) + "," + tree[next].attrValue.get_string() + "," + data[criteriaAttrIndex] + "},";
    // if (tree[next].isLeaf)
    // {
    //   return next;
    // } else {
    if (is_pass(tree[next].opt, tree[next].attrValue.get_string(), data[criteriaAttrIndex]) ) {

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

  pesan.cetak("\n");
  pesan.cetak(tmp_str.c_str());
  pesan.cetak("\n");

  return -1;
}

string Tdec_tree::guess(vector<string> &data)
{
  string label = "failed.";
  // vec_attr.clear();
  // int leafNode = dfs(data, 0);

  // if (leafNode == -1) {
  //   return "dfs_failed.";
  // } else {
  //   label = tree[leafNode].label;
  //   if (global_config.train_svm)
  //   {
  //     if ((label == "normal") and (tree[leafNode].idx_svm != -1))
  //     {
  //       Twrite_file tmp_wf;

  //       if (global_config.save_test) {
  //         tmp_wf.setnm_f(global_config.path_model + "/test/test_model_" + to_string(tree[leafNode].idx_svm) + ".csv");
  //       }
  //       //cetak("{v {model ");
  //       Tmy_svm my_svm;
  //       string nm_model = global_config.svm_path + "/svm_model_" + to_string(tree[leafNode].idx_svm) + ".csv";
  //       my_svm.load_model(nm_model);
  //       //cetak("save_model_");
  //       //cetak(to_string(tree[leafNode].idx_svm).c_str());
  //       //cetak(".csv} ");

  //       vector<string> tmp_data;
  //       string tmp_str = "";
  //       if (global_config.feature_selection)
  //       {

  //         for (auto itr = vec_attr.begin(); itr != vec_attr.end(); ++itr)
  //         {
  //           if (itr->first != -1) {
  //             tmp_data.push_back(data[itr->first]);
  //             tmp_str = tmp_str + data[itr->first] + ",";
  //           }
  //         }

  //       } else {
  //         for (size_t i = 0; i < (data.size() - 1); ++i)
  //         {
  //           tmp_data.push_back(data[i]);
  //           tmp_str = tmp_str + data[i] + ",";
  //         }
  //       }

  //       tmp_str = tmp_str + data[data.size() - 1];

  //       if (global_config.save_test)
  //       {
  //         tmp_wf.write_file(tmp_str);
  //         tmp_wf.close_file();
  //       }
  //       //cetak(" {guess ");
  //       label = my_svm.guess(tmp_data);
  //       //cetak("}");

  //       tmp_data.clear();
  //       tmp_data.shrink_to_fit();

  //     }
  //   }

  // }
  return label;
}

void Tdec_tree::thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &dt_conf_metrix)
{
  // std::mutex v_mutex;

  // std::lock_guard<std::mutex> lock(v_mutex);

  for (int i = 0; i < table.size(); ++i)
  {
    dt_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);    
  }

}



void Tdec_tree::test_dfs(int node_index , Tdataframe &df_test, Tconf_metrix &dt_conf_metrix)
{
  //cetak("%d", node_index);
  if (tree[node_index].isLeaf)
  {

    string label = tree[node_index].label;
      //clear_worker(1);
      //cetak("+{A { %d }}", df_test.getjmlrow());
      _table_attack = df_test.get_all_record();
      worker.push_back(thread(&Tdec_tree::thread_test_attack, label, _table_attack, ref(dt_conf_metrix)));
      _table_attack.clear();
      _table_attack.shrink_to_fit();
    
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
        test_dfs(left, df_left, dt_conf_metrix);
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
        test_dfs(right, df_right, dt_conf_metrix);
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

void Tdec_tree::test(Tdataframe &df,Tconf_metrix &dt_conf_metrix)
{
  // Tdataframe df(config);
  // df.read_data(config->f_test);
  // df.read_data_type(config->f_datatype);
  //df.info();  

  //Tconf_metrix dt_conf_metrix;  
  dt_conf_metrix.add_konversi_asli("unknown", "known");
  
  {
    Timer timer;
    test_dfs(0, df, dt_conf_metrix);
    //double elapsed_time = double(std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
  }
  
  //df.close_file();  
  
  dt_conf_metrix.kalkulasi();
  pesan.cetak("Depth=%d,Minimum_Sample=%d,credal=%.4f,threshold=%d,FP=%d,FN=%d,F1=%.4f \n", global_config.depth, global_config.min_sample, global_config.credal_s,global_config.threshold,dt_conf_metrix.get_FP("known"),dt_conf_metrix.get_FN("known"),dt_conf_metrix.get_F1());
  
  // dt_conf_metrix.save(config->path_model + "/dt_metrik.csv", config->f_test, config->depth, config->min_sample, config->gamma, config->nu, config->credal_s);


}

tree_node* Tdec_tree::vec_tree_to_dec_tree(int node_index)
{
  tree_node* parent_node = new tree_node;

  if (tree[node_index].isLeaf)
  {
    parent_node->isLeaf = tree[node_index].isLeaf;
    parent_node->label = tree[node_index].label;
    parent_node->idx_svm = tree[node_index].idx_svm;
  } else {

    parent_node->criteriaAttrIndex = tree[node_index].criteriaAttrIndex;

    int left = tree[node_index].children[0];
    int right = tree[node_index].children[1];

    if (left != -1) {
      tree_node* left_node = vec_tree_to_dec_tree(left);
      left_node->opt = tree[left].opt;
      left_node->attrValue.set_value(tree[left].attrValue.get_string(), (left_node->opt == 0) or (left_node->opt == 1));
      parent_node->left = left_node;
    }

    if (right != -1) {
      tree_node* right_node = vec_tree_to_dec_tree(right);
      right_node->opt = tree[right].opt;
      right_node->attrValue.set_value(tree[right].attrValue.get_string(), (right_node->opt == 0) or (right_node->opt == 1));
      parent_node->right = right_node;
    }
  }

  return parent_node;
}

void Tdec_tree::read_tree()
{
  vector<string> tmp_data;
  Tdataframe df;
  df.read_data(global_config.path_model + "/dtsvm_model_"+to_string(global_config.depth)+"_"+to_string(global_config.min_sample)+"_"+to_string(global_config.threshold)+".csv");

  df.reset_file();
  while (!df.is_eof()) {

    tmp_data = df.get_record();

    //cout << tmp_data.size() << endl;

    Node newnode;
    //cout << tmp_data[0] << endl;
    newnode.criteriaAttrIndex = tmp_data[0] == "-1" ?  -1 : stoi(tmp_data[0]);
    newnode.attrValue.set_value(tmp_data[1], false);
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

  dec_tree = vec_tree_to_dec_tree(0);

  tree.clear();

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
