#include "Tdt_learn_svm.h"

Tdt_learn_svm::Tdt_learn_svm()
{
  idx_svm = 0;
  id_df = 1;

}

Tdt_learn_svm::~Tdt_learn_svm()
{
  tree.clear();
  del_dec_tree(dec_tree);
}

void Tdt_learn_svm::del_dec_tree(tree_node* parent_node)
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


void Tdt_learn_svm::svm_datatset_dfs(tree_node* parent_node , Tdataframe &df, int counter)
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
    df_left = df;
    df_left.switch_parent_branch();
    df_left.set_branch(counter, 1, branch_number[counter]);
    df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, false, false);

    if (parent_node->left->isLeaf)
    {
      df_left.ReFilter(false);
      if (df_left.getjmlrow() > 0) {
        string label = parent_node->left->label;
        df_left.set_label_idx_svm(parent_node->left->idx_svm, label);
      }
    } else {
      svm_datatset_dfs(parent_node->left, df_left, counter);
    }

  }


  branch_number[counter] = branch_number[counter] + 1;

  if (parent_node->right != NULL) {

    Tdataframe df_right;
    df_right = df;
    df_right.switch_parent_branch();
    df_right.set_branch(counter, 2, branch_number[counter]);
    df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, false, false);

    if (parent_node->right->isLeaf)
    {
      df_right.ReFilter(false);
      if (df_right.getjmlrow() > 0) {
        string label = parent_node->right->label;
        df_right.set_label_idx_svm(parent_node->right->idx_svm, label);
      }

    } else {
      svm_datatset_dfs(parent_node->right, df_right, counter);
    }

  }

}

void Tdt_learn_svm::set_svm_dataset(Tdataframe &df)
{
  branch_number.clear();
  svm_datatset_dfs(dec_tree, df, 0);
}


tree_node* Tdt_learn_svm::vec_tree_to_dec_tree(int node_index)
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

void Tdt_learn_svm::read_tree()
{
  vector<string> tmp_data;
  tb_tree dbtree;
  dbtree.baca_tree(global_config.id_experiment_dt, global_config.id_detail_experiment_dt);

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

    if (idx_svm < newnode.idx_svm)
    {
      idx_svm = newnode.idx_svm;
    }

    tree.push_back(newnode);

    dbtree.next_record();
  }

  dbtree.close_file();
  dec_tree = vec_tree_to_dec_tree(0);

}

void Tdt_learn_svm::thread_save_train(vector<vector<string>> table, int v_idx_svm)
{
  Tglobal_config global_config;
   
  Twrite_file tmp_wf;
  tmp_wf.setnm_f(global_config.path_model + "/train/train_model_" + to_string(v_idx_svm) + ".csv");


  for (int i = 0; i < table.size(); ++i)
  {
    string tmp_str = "";
    for (int j = 0; j < (table[i].size() - 1); ++j)
    {
      tmp_str = tmp_str + table[i][j] + ",";
    }

    tmp_str = tmp_str + table[i][table[i].size() - 1];
    tmp_wf.write_file(tmp_str);

  }

  tmp_wf.close_file();

}

void Tdt_learn_svm::thread_train_svm(vector<vector<string>> table, int v_idx_svm)
{
  sleep(10);
  Tglobal_config global_config;
  tb_experiment experiment;

  experiment.insert_more_detail_experiment(global_config.id_experiment_dt, global_config.id_detail_experiment_dt, v_idx_svm, global_config.gamma, global_config.nu);
  Tmy_svm my_svm;
  my_svm.train(table);
  my_svm.save_model(global_config.svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
  table.clear();
  sleep(5);
  // time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(v_idx_svm);
  experiment.end_train_more_detail();
}

void Tdt_learn_svm::f_train_svm(Tdataframe &df, int v_idx_svm)
{
  vector<vector<string>> table = df.get_all_record_svm();

  // if (config->save_train) {

  //   worker.push_back(thread(&Tdt_learn_svm::thread_save_train, ref(config), table, v_idx_svm));
  //   //thread_save_test(config,table,v_idx_svm);

  // }
  worker.push_back(thread(&Tdt_learn_svm::thread_train_svm, table, v_idx_svm));
  table.clear();
}

void Tdt_learn_svm::clear_worker(int limit)
{

  bool pass = limit == 0  ? true : worker.size() == limit ;

  if (pass)
  {
    if (limit == 0) {
      pesan.cetak("{clear worker} \n");
    }
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

void Tdt_learn_svm::svm_dfs(int depth , int node_index , Tdataframe & df_train)
{
  // cetak("%d?", depth);

  // if (tree[node_index].isLeaf)
  // {
  //   cetak("*");

  //   string label = tree[node_index].label;

  //   if ((label == "normal") and (tree[node_index].idx_svm != -1))
  //   {

  //     clear_worker(20);

  //     cetak("{v {j %d %d}", tree[node_index].idx_svm, df_train.getjmlrow_svm());
  //     f_train_svm(df_train, tree[node_index].idx_svm);
  //     cetak("}");
  //   } else {
  //     cetak("{A}");
  //   }

  //   cetak("\n");

  // } else {

  //   cetak("|");

  //   int left = tree[node_index].children[0];
  //   int right = tree[node_index].children[1];

  //   Tdataframe df_left, df_right;
  //   df_left = df_train;
  //   df_right = df_train;
  //   //df_train.clear_memory();


  //   if (left != -1) {
  //     df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
  //     cetak("->");
  //     svm_dfs(depth + 1, left, df_left);
  //   }

  //   if (right != -1) {
  //     df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
  //     cetak("<-");
  //     svm_dfs(depth + 1, right, df_right);
  //   }

  //   clear_worker(20);
  // }

  // if (node_index == 0)
  // {
  //   clear_worker(0);
  // }

}

void Tdt_learn_svm::learn_svm(Tdataframe &df)
{
   
  tb_experiment experiment;

  time_t id_detail_experiment = 0, id_experiment = 0, id_detail_experiment_dt = 0, id_experiment_dt = 0;
  bool ada = experiment.cari_detail_experiment(global_config.gamma, global_config.nu, id_detail_experiment, id_experiment, id_detail_experiment_dt, id_experiment_dt);


  vector<int> idx_svm = df.get_idx_svm();
  for (auto i = idx_svm.begin(); i != idx_svm.end(); ++i)
  {
    pesan.cetak("---- Train no svm %d \n", *i);
    if (ada)
    {
      string str_id_experiment_dt = to_string(id_experiment_dt);
      string str_id_detail_experiment_dt = to_string(id_detail_experiment_dt);
      string str_id_experiment = to_string(id_experiment);
      string str_id_detail_experiment = to_string(id_detail_experiment);

      string tmp_str = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/optimal/svm_model_" + to_string(*i) + ".csv";

      path v_path(tmp_str);
      if (exists(v_path))
      {
        str_id_experiment_dt = to_string(global_config.id_experiment_dt);
        str_id_detail_experiment_dt = to_string(global_config.id_detail_experiment_dt);
        str_id_experiment = to_string(global_config.id_experiment);
        str_id_detail_experiment = to_string(global_config.id_detail_experiment);

        string tmp_str1 = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";
        
        sleep(10);
        experiment.insert_more_detail_experiment(global_config.id_experiment_dt, global_config.id_detail_experiment_dt, *i, global_config.gamma, global_config.nu);
        fs::copy(tmp_str, tmp_str1);
        //time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);
        experiment.end_train_more_detail();

      } else {

        string tmp_str = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";

        str_id_experiment_dt = to_string(global_config.id_experiment_dt);
        str_id_detail_experiment_dt = to_string(global_config.id_detail_experiment_dt);
        str_id_experiment = to_string(global_config.id_experiment);
        str_id_detail_experiment = to_string(global_config.id_detail_experiment);

        string tmp_str1 = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";

        path v_path(tmp_str);
        if (exists(v_path)) {
          sleep(10);  
          experiment.insert_more_detail_experiment(global_config.id_experiment_dt, global_config.id_detail_experiment_dt, *i, global_config.gamma, global_config.nu);
          fs::copy(tmp_str, tmp_str1);
          //time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);
          experiment.end_train_more_detail();
        } else {
          pesan.cetak(" tidak ada \n");
        }
      }

    } else {
      df.filter_by_idx_svm(*i);
      f_train_svm(df, *i);
      clear_worker(2);
    }
  }
  clear_worker(0);
}