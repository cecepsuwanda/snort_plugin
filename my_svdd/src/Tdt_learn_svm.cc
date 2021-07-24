#include "Tdt_learn_svm.h"

Tdt_learn_svm::Tdt_learn_svm(Tconfig *v_config)
{
	idx_svm = 0;
	id_df = 1;

	config = v_config;
}

Tdt_learn_svm::~Tdt_learn_svm()
{

}

void Tdt_learn_svm::read_tree()
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

void Tdt_learn_svm::thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm)
{
	Tmy_svdd my_svdd(v_config);
	my_svdd.train(table);
	my_svdd.save_model(v_config->svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
}

void Tdt_learn_svm::f_train_svm(Tdataframe &df, int v_idx_svm)
{
	vector<vector<string>> table = df.get_all_record_svm();

	if (config->save_train) {

		worker.push_back(thread(&Tdt_learn_svm::thread_save_train, ref(config), table, v_idx_svm));
		//thread_save_test(config,table,v_idx_svm);

	}

	worker.push_back(thread(&Tdt_learn_svm::thread_train_svm, ref(config), table, v_idx_svm));

}

void Tdt_learn_svm::clear_worker(int limit)
{

	bool pass = limit == 0  ? true : worker.size() == limit ;

	if (pass)
	{
		cetak("{clear worker}");
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
  cetak("%d?", depth);

  if (tree[node_index].isLeaf)
  {
    cetak("*");

    string label = tree[node_index].label;

    if ((label == "normal") and (tree[node_index].idx_svm != -1))
    {

      clear_worker(20);

      cetak("{v {j %d %d}", tree[node_index].idx_svm, df_train.getjmlrow_svm());
      f_train_svm(df_train, tree[node_index].idx_svm);
      cetak("}");
    } else {
      cetak("{A}");
    }

    cetak("\n");

  } else {

    cetak("|");

    int left = tree[node_index].children[0];
    int right = tree[node_index].children[1];

    Tdataframe df_left, df_right;
    df_left = df_train;
    df_right = df_train;
    df_train.clear_memory();


    if (left != -1) {
      df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
      cetak("->");
      svm_dfs(depth + 1, left, df_left);
    }

    if (right != -1) {
      df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
      cetak("<-");
      svm_dfs(depth + 1, right, df_right);
    }

    clear_worker(20);
  }

  if (node_index == 0)
  {
    clear_worker(0);
  }

}

void Tdt_learn_svm::learn_svm()
{
  Tdataframe df_train(config);
  df_train.read_data(config->f_train);
  df_train.read_data_type(config->f_datatype);
  df_train.set_id(0);
  df_train.info();


  cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
  cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.4f credal : %.4f  feature_selection :%d normal only : %d train : %s  \n", config->depth, config->min_sample, config->gamma, config->nu, config->credal_s , (config->feature_selection ? 1 : 0), (config->normal_only ? 1 : 0), config->f_train.c_str());
  cetak("Start Train Decission Tree : \n");
  {
    Timer timer;
    svm_dfs(0, 0, df_train);
  }

  cetak("End Train Decission Tree : \n");
  df_train.close_file();
}