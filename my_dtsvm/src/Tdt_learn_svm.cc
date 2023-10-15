#include "Tdt_learn_svm.h"

Tdt_learn_svm::Tdt_learn_svm()
{
	idx_svm = 0;
	id_df = 1;	
}

Tdt_learn_svm::~Tdt_learn_svm()
{

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
  Tdataframe df;
  df.read_data(global_config.path_model + "/dtsvm_model.csv");

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

void Tdt_learn_svm::thread_save_train(vector<vector<string>> table, int v_idx_svm)
{
	Tglobal_config v_config;

  Twrite_file tmp_wf;
	tmp_wf.setnm_f(v_config.path_model + "/train/train_model_" + to_string(v_idx_svm) + ".csv");


	for (size_t i = 0; i < table.size(); ++i)
	{
		string tmp_str = "";
		for (size_t j = 0; j < (table[i].size() - 1); ++j)
		{
			tmp_str = tmp_str + table[i][j] + ",";
		}

		tmp_str = tmp_str + table[i][table[i].size() - 1];
		tmp_wf.write_file(tmp_str);

	}

	tmp_wf.close_file();

}

void Tdt_learn_svm::thread_train_svm( vector<vector<string>> table, int v_idx_svm)
{
	Tglobal_config v_config;

  Tmy_svm my_svm;
	my_svm.train(table);
	my_svm.save_model(v_config.svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
}

void Tdt_learn_svm::f_train_svm(Tdataframe &df, int v_idx_svm)
{
	vector<vector<string>> table = df.get_all_record_svm();

	if (global_config.save_train) {

		worker.push_back(thread(&Tdt_learn_svm::thread_save_train, table, v_idx_svm));
		//thread_save_test(config,table,v_idx_svm);

	}

	worker.push_back(thread(&Tdt_learn_svm::thread_train_svm, table, v_idx_svm));

}

void Tdt_learn_svm::clear_worker(size_t limit)
{

	bool pass = limit == 0  ? true : worker.size() == limit ;

	if (pass)
	{
		pesan.cetak("{clear worker}");
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

void Tdt_learn_svm::svm_dfs(int depth , tree_node* parent_node , Tdataframe & df_train)
{
  pesan.cetak("%d?", depth);

  if (parent_node->isLeaf)
  {
    pesan.cetak("*");

    string label = parent_node->label;

    if ((label == "normal") and (parent_node->idx_svm != -1))
    {

      clear_worker(20);

      pesan.cetak("{v {j %d %d}", parent_node->idx_svm, df_train.getjmlrow_svm());
      f_train_svm(df_train, parent_node->idx_svm);
      pesan.cetak("}");
    } else {
      pesan.cetak("{A}");
    }

    pesan.cetak("\n");

  } else {

    pesan.cetak("|");

    tree_node* left = parent_node->left;
    tree_node* right = parent_node->right;

    Tdataframe df_left, df_right;
    df_left = df_train;
    df_right = df_train;
    df_train.clear_memory();


    if (left != NULL) {
      df_left.add_filter(parent_node->criteriaAttrIndex, left->opt, left->attrValue);
      pesan.cetak("->");
      svm_dfs(depth + 1, left, df_left);
    }

    if (right != NULL) {
      df_right.add_filter(parent_node->criteriaAttrIndex, right->opt, right->attrValue);
      pesan.cetak("<-");
      svm_dfs(depth + 1, right, df_right);
    }

    clear_worker(global_config.jml_thread);
  }

  if (depth == 0)
  {
    clear_worker(0);
  }

}

void Tdt_learn_svm::learn_svm()
{
  Tdataframe df_train;
  df_train.read_data(global_config.f_train);
  df_train.read_data_type(global_config.f_datatype);
  df_train.set_id(0);
  df_train.info();


  pesan.cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
  pesan.cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.4f credal : %.4f  feature_selection :%d normal only : %d train : %s  \n", global_config.depth, global_config.min_sample, global_config.gamma, global_config.nu, global_config.credal_s , (global_config.feature_selection ? 1 : 0), (global_config.normal_only ? 1 : 0), global_config.f_train.c_str());
  pesan.cetak("Start Train Decission Tree : \n");
  {
    Timer timer;
    svm_dfs(0, dec_tree, df_train);
    clear_worker(0);
  }

  pesan.cetak("End Train Decission Tree : \n");
  df_train.close_file();
}