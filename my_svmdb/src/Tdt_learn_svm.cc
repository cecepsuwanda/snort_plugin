#include "Tdt_learn_svm.h"

Tdt_learn_svm::Tdt_learn_svm(Tconfig *v_config)
{
  idx_svm = 0;
  id_df = 1;

  config = v_config;
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
      left_node->attrValue = tree[left].attrValue;
      left_node->opt = tree[left].opt;
      parent_node->left = left_node;
    }

    if (right != -1) {
      tree_node* right_node = vec_tree_to_dec_tree(right);
      right_node->attrValue = tree[right].attrValue;
      right_node->opt = tree[right].opt;
      parent_node->right = right_node;
    }
  }

  return parent_node;
}

void Tdt_learn_svm::read_tree()
{
  vector<string> tmp_data;
  tb_tree dbtree;
  dbtree.baca_tree(config->id_experiment_dt, config->id_detail_experiment_dt);

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

void Tdt_learn_svm::thread_save_train(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm)
{
  Twrite_file tmp_wf;
  tmp_wf.setnm_f(v_config->path_model + "/train/train_model_" + to_string(v_idx_svm) + ".csv");


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

void Tdt_learn_svm::thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm, tb_experiment &experiment)
{
  experiment.insert_more_detail_experiment(v_config->id_experiment_dt, v_config->id_detail_experiment_dt, v_idx_svm, v_config->gamma, v_config->nu);
  Tmy_svm my_svm(v_config);
  my_svm.train(table);
  my_svm.save_model(v_config->svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
  table.clear();
  // sleep(5);
  // time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(v_idx_svm);
  // experiment.end_train_more_detail(id_more_detail_experiment, v_idx_svm);
}

void Tdt_learn_svm::f_train_svm(Tdataframe &df, int v_idx_svm, tb_experiment &experiment)
{
  vector<vector<string>> table = df.get_all_record_svm();

  // if (config->save_train) {

  //   worker.push_back(thread(&Tdt_learn_svm::thread_save_train, ref(config), table, v_idx_svm));
  //   //thread_save_test(config,table,v_idx_svm);

  // }
  worker.push_back(thread(&Tdt_learn_svm::thread_train_svm, ref(config), table, v_idx_svm, ref(experiment)));
  table.clear();
}

void Tdt_learn_svm::clear_worker(int limit)
{

  bool pass = limit == 0  ? true : worker.size() == limit ;

  if (pass)
  {
    if (limit == 0) {
      cetak("{clear worker} \n");
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

void Tdt_learn_svm::learn_svm(Tdataframe &df, tb_experiment &experiment)
{

  time_t id_detail_experiment = 0, id_experiment = 0, id_detail_experiment_dt = 0, id_experiment_dt = 0;
  bool ada = experiment.cari_detail_experiment(config->gamma, config->nu, id_detail_experiment, id_experiment, id_detail_experiment_dt, id_experiment_dt);


  vector<int> idx_svm = df.get_idx_svm();
  for (auto i = idx_svm.begin(); i != idx_svm.end(); ++i)
  {
    cetak("---- Train no svm %d \n", *i);
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
        str_id_experiment_dt = to_string(config->id_experiment_dt);
        str_id_detail_experiment_dt = to_string(config->id_detail_experiment_dt);
        str_id_experiment = to_string(config->id_experiment);
        str_id_detail_experiment = to_string(config->id_detail_experiment);

        string tmp_str1 = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";

        experiment.insert_more_detail_experiment(config->id_experiment_dt, config->id_detail_experiment_dt, *i, config->gamma, config->nu);
        fs::copy(tmp_str, tmp_str1);
        time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);
        experiment.end_train_more_detail(id_more_detail_experiment, *i);

      } else {

        string tmp_str = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";

        str_id_experiment_dt = to_string(config->id_experiment_dt);
        str_id_detail_experiment_dt = to_string(config->id_detail_experiment_dt);
        str_id_experiment = to_string(config->id_experiment);
        str_id_detail_experiment = to_string(config->id_detail_experiment);

        string tmp_str1 = "hsl/" + str_id_experiment_dt + "/" + str_id_detail_experiment_dt + "/" + str_id_experiment + "/" + str_id_detail_experiment + "/svm_model_" + to_string(*i) + ".csv";

        path v_path(tmp_str);
        if (exists(v_path)) {
          experiment.insert_more_detail_experiment(config->id_experiment_dt, config->id_detail_experiment_dt, *i, config->gamma, config->nu);
          fs::copy(tmp_str, tmp_str1);
          time_t id_more_detail_experiment = experiment.get_id_more_detail_experiment(*i);
          experiment.end_train_more_detail(id_more_detail_experiment, *i);
        } else {
          cetak(" tidak ada \n");
        }
      }

    } else {
      df.filter_by_idx_svm(*i);
      f_train_svm(df, *i, experiment);
      clear_worker(2);
    }
  }
  clear_worker(0);
}