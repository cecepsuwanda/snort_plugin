#include "Tdec_tree.h"


Tdec_tree::Tdec_tree()
{
  idx_svm = 0;
  id_df = 1;


}

Tdec_tree::~Tdec_tree()
{
  tree.clear();
  del_dec_tree(dec_tree);
}


void Tdec_tree::del_dec_tree(tree_node* parent_node)
{
  if (parent_node->left != NULL)
  {
    if (parent_node->left->isLeaf)
    {
      delete parent_node->left;
      parent_node->left = NULL;
    } else {
      del_dec_tree(parent_node->left);
    }

  }

  if (parent_node->right != NULL)
  {
    if (parent_node->right->isLeaf)
    {
      delete parent_node->right;
      parent_node->right = NULL;
    } else {
      del_dec_tree(parent_node->right);
    }
  }

  if ( (parent_node->left == NULL) and (parent_node->right == NULL) )
  {
    delete parent_node;
    parent_node = NULL;
  }
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
  vec_attr.clear();
  int leafNode = dfs(data, 0);

  if (leafNode == -1) {
    return "dfs_failed.";
  } else {
    label = tree[leafNode].label;
  }
  return label;
}

// void Tdec_tree::thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &dt_conf_metrix)
// {
//   // std::mutex v_mutex;

//   // std::lock_guard<std::mutex> lock(v_mutex);

//   for (size_t i = 0; i < table.size(); ++i)
//   {
//     dt_conf_metrix.add_jml(table[i][table[i].size() - 1], label, 1);
//   }

// }

void Tdec_tree::thread_test_attack(map<string, map<string, int>> metrix, Tconf_metrix &dt_conf_metrix)
{
  // std::mutex v_mutex;

  // std::lock_guard<std::mutex> lock(v_mutex);

  for (auto i = metrix.begin(); i != metrix.end(); i++)
  {
    for (auto j = i->second.begin(); j != i->second.end(); j++)
    {

      dt_conf_metrix.add_jml(i->first, j->first, j->second);
    }
  }

}

void Tdec_tree::test_dfs(tree_node* parent_node , Tdataframe &df_test, Tconf_metrix &dt_conf_metrix, int counter)
{
  counter++;

  auto itr = branch_number.find(counter);
  if (itr == branch_number.end()) {
    branch_number.insert({counter, 1});
  } else {
    branch_number[counter] = branch_number[counter] + 1;
  }

  if (parent_node->left != NULL) {

    Tdataframe df_left;
    df_left = df_test;
    df_left.switch_parent_branch();
    df_left.set_branch(counter, 1, branch_number[counter]);
    df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, true, true);

    if (parent_node->left->isLeaf)
    {
      pesan.cetak("-");
      //df_left.ReFilter(false);
      if (df_left.getjmlrow() > 0) {
        clear_worker(2);
        string label = parent_node->left->label;
        df_left.set_label(label);
        map<string, map<string, int>> branch_conf_metrix = df_left.get_conf_metrix();
        pesan.cetak("+");
        //cetak("[%s %d]\n", label.c_str(), df_left.getjmlrow());
        //_table_attack = df_left.get_all_record();
        //worker.push_back(thread(&Tdec_tree::thread_test_attack, label, _table_attack, ref(dt_conf_metrix)));
        worker.push_back(thread(&Tdec_tree::thread_test_attack, branch_conf_metrix, ref(dt_conf_metrix)));
        // _table_attack.clear();
        // _table_attack.shrink_to_fit();
      }

      //df_left.clear_memory();
    } else {
      pesan.cetak(".");
      //cetak("%d", counter - 1);
      //cetak("?|->");
      if (df_left.getjmlrow() > 0) {
        test_dfs(parent_node->left, df_left, dt_conf_metrix, counter);
      }
      //df_left.clear_memory();
    }

  }

  if (counter == 1)
  {
    clear_worker(0);
  }

  branch_number[counter] = branch_number[counter] + 1;

  if (parent_node->right != NULL) {

    Tdataframe df_right;
    df_right = df_test;
    df_right.switch_parent_branch();
    df_right.set_branch(counter, 2, branch_number[counter]);
    df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, true, true);

    if (parent_node->right->isLeaf)
    {
      pesan.cetak("-");
      //df_right.ReFilter(false);
      if (df_right.getjmlrow() > 0) {
        clear_worker(2);
        string label = parent_node->right->label;
        df_right.set_label(label);
        map<string, map<string, int>> branch_conf_metrix = df_right.get_conf_metrix();
        pesan.cetak("+");
        //cetak("[%s %d]\n", label.c_str(), df_right.getjmlrow());
        //_table_attack = df_right.get_all_record();
        //worker.push_back(thread(&Tdec_tree::thread_test_attack, label, _table_attack, ref(dt_conf_metrix)));
        worker.push_back(thread(&Tdec_tree::thread_test_attack, branch_conf_metrix, ref(dt_conf_metrix)));
        // _table_attack.clear();
        // _table_attack.shrink_to_fit();
      }
      //df_right.clear_memory();
    } else {
      pesan.cetak(".");
      //cetak("%d", counter - 1);
      //cetak("?|<-");
      if (df_right.getjmlrow() > 0) {
        test_dfs(parent_node->right, df_right, dt_conf_metrix, counter);
      }
      //df_right.clear_memory();
    }


  }

  if (counter == 1)
  {
    clear_worker(0);
  }
}



void Tdec_tree::test(Tdataframe &df_test, Tconf_metrix &dt_conf_metrix)
{

  // Tdataframe df(config);
  // df.set_dataset(config->id_dt_test, config->jns_dt_test, config->partition_test);
  // df.read_header_type();
  // df.set_parent(0, 0, 0);
  // df.set_branch(0, 0, 0);
  // df.clone_dataset();
  // df.stat_tabel(false, false, false);

  //df.info();

  //Tconf_metrix dt_conf_metrix;
  dt_conf_metrix.add_konversi_asli("unknown", "known");

  {

    test_dfs(dec_tree, df_test, dt_conf_metrix, 0);
    //test_dfs(0, df, dt_conf_metrix, 0);

  }

  //df.close_file();

  dt_conf_metrix.kalkulasi();
  pesan.cetak("\n Depth=%d,Minimum_Sample=%d,credal=%.4f,threshold=%.4f,FP=%d,FN=%d,F1=%.4f \n", global_config.depth, global_config.min_sample, global_config.credal_s, global_config.threshold, dt_conf_metrix.get_FP("normal"), dt_conf_metrix.get_FN("normal"), dt_conf_metrix.get_F1("normal"));

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
  tb_tree dbtree;
  dbtree.baca_tree(global_config.id_experiment, global_config.id_detail_experiment);

  dbtree.reset_file();
  while (!dbtree.is_eof()) {

    tmp_data = dbtree.get_record();

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

    //   if (idx_svm < newnode.idx_svm)
    //   {
    //     idx_svm = newnode.idx_svm;
    //   }

    tree.push_back(newnode);

    dbtree.next_record();
  }

  dbtree.close_file();

  dec_tree = vec_tree_to_dec_tree(0);

  tree.clear();

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
