#include "Tdt_build.h"


Tdt_build::Tdt_build(Tconfig *v_config)
{
	idx_svm = 0;
	id_df = 1;

	config = v_config;
}

Tdt_build::~Tdt_build()
{

}

void Tdt_build::del_model_train(int idx)
{
  string filename = config->svm_path + "/svm_model_" + to_string(idx) + ".csv";
  remove(filename.c_str());

  filename = config->path_model + "/train/train_model_" + to_string(idx) + ".csv";
  remove(filename.c_str());
}

Tmetric_split_value Tdt_build::get_split_value(Tdataframe &df, int idx)
{
	Tmetric_split_value hsl;

	string current_split_value = "-1";
	float current_overall_metric = -1;

	df.calculate_overall_metric(idx, current_overall_metric, current_split_value);

	hsl.idx = idx;
	hsl.overall_metric = current_overall_metric;
	hsl.split_value = current_split_value;
	return hsl;
}


void Tdt_build::determine_best_split(Tdataframe &df, int &split_column, string &split_value)
{
	//cout << "determine_best_split " << endl;
	float max_attr = df.getjmlrow();
	float max_gain = 0;
	bool  first_iteration = true;

	string current_split_value = "-1";
	float current_overall_metric = -1;

	split_column = -1;
	split_value = "-1";

	vector<future<Tmetric_split_value>> async_worker;

	for (int i = 0; i < df.get_jml_valid_attr(); ++i)
	{
		async_worker.push_back(async(std::launch::async, &Tdt_build::get_split_value, ref(df), df.get_valid_attr(i)));
	}

	if (async_worker.size() > 0)
	{

		Tmetric_split_value hsl;
		for (future<Tmetric_split_value> & th : async_worker)
		{
			hsl = th.get();
			if (first_iteration or (max_gain < hsl.overall_metric))
			{
				first_iteration = false;
				max_gain = hsl.overall_metric;

				split_column = hsl.idx;
				split_value = hsl.split_value;
			}
		}

		async_worker.clear();
		async_worker.shrink_to_fit();

	}

	/*for (int i = 0; i < df.get_jml_valid_attr(); ++i)
	{

	  //df.get_col_pot_split(i);
	  df.calculate_overall_metric(df.get_valid_attr(i), current_overall_metric, current_split_value);

	  if (first_iteration or (max_gain < current_overall_metric))
	  {
	    first_iteration = false;
	    max_gain = current_overall_metric;

	    split_column = df.get_valid_attr(i);
	    split_value = current_split_value;
	  }
	}*/

	df.clear_map_col_split();
}

void Tdt_build::thread_save_train(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm)
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

void Tdt_build::thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm)
{
	Tmy_svm my_svm(v_config);
	my_svm.train(table);
	my_svm.save_model(v_config->svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
}

void Tdt_build::f_train_svm(Tdataframe &df, int v_idx_svm)
{
	vector<vector<string>> table = df.get_all_record_svm();

	if (config->save_train) {

		worker.push_back(thread(&Tdt_build::thread_save_train, ref(config), table, v_idx_svm));
		//thread_save_test(config,table,v_idx_svm);

	}

	worker.push_back(thread(&Tdt_build::thread_train_svm, ref(config), table, v_idx_svm));

}

void Tdt_build::clear_worker(int limit)
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

string Tdt_build::create_leaf(Tdataframe & df)
{
	string vlabel = df.get_max_label();
	return vlabel;
}


bool Tdt_build::check_purity(Tdataframe &df)
{
	return df.is_single_label();
}

void Tdt_build::train(Tdataframe & df, int node_index , int counter)
{
	if (node_index == 0)
	{
		Node root;
		root.treeIndex = 0;
		tree.push_back(root);
	}

	//cout << counter;
	cetak("[%d %d]", counter, df.getjmlrow());

	if (check_purity(df) or (df.getjmlrow() < config->min_sample) or (counter == config->depth) )
	{
		string tmp_str = create_leaf(df);
		cetak("*");

		if (tmp_str == "normal") {

			idx_svm++;
			tree[node_index].idx_svm = idx_svm;

			if ((config->train_svm))
			{
				clear_worker(20);

				cetak("{v %d %d ", idx_svm, df.getjmlrow_svm());
				f_train_svm(df, idx_svm);
				cetak("}");

			}

		} else {
			cetak("{A}");
		}

		tree[node_index].isLeaf = true;
		tree[node_index].label = tmp_str;


		df.clear_memory();
		df.clear_col_split();

		cetak("\n");

	} else {

		cetak("?");

		counter++;

		int split_column = -1;
		string split_value = "-1";

		determine_best_split(df, split_column, split_value);

		Tdataframe df_below, df_above;
		if (split_value != "-1")
		{
			df_below = df;
			df_below.set_id(id_df++);
			df_above = df;
			df_above.set_id(id_df++);

		}
		df.split_data(split_column, split_value, df_below, df_above);

		if (((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0))  or (split_value == "-1")) { //or ((df_below.getjmlrow() < limit) or (df_above.getjmlrow() < limit))
			string tmp_str = create_leaf(df);

			cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				tree[node_index].idx_svm = idx_svm;

				if ((config->train_svm))
				{

					clear_worker(20);

					cetak("{v %d %d ", idx_svm, df.getjmlrow_svm());
					f_train_svm(df, idx_svm);
					cetak("}");

				}

			} else {
				cetak("{A}");
			}

			tree[node_index].isLeaf = true;
			tree[node_index].label = tmp_str;


			df_below.clear_memory();
			df_above.clear_memory();

			df_below.clear_col_split();
			df_above.clear_col_split();

			cetak("\n");

		} else {
			if (split_value != "-1")
			{

				cetak("|");

				tree[node_index].criteriaAttrIndex = split_column;

				int treeIndex_yes, treeIndex_no;

				Node nextNode;
				nextNode.treeIndex = (int)tree.size();
				nextNode.attrValue = split_value;
				nextNode.opt = df.get_opt(split_column, 1);
				treeIndex_yes = nextNode.treeIndex;
				tree[node_index].children.push_back(nextNode.treeIndex);
				tree.push_back(nextNode);

				// cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode.opt == 0 ? "<=" : "==") << nextNode.attrValue << endl;
				cetak("->");
				train(df_below, nextNode.treeIndex, counter);


				Node nextNode1;
				nextNode1.treeIndex = (int)tree.size();
				nextNode1.attrValue = split_value;
				nextNode1.opt = df.get_opt(split_column, 0);
				treeIndex_no = nextNode1.treeIndex;
				tree[node_index].children.push_back(nextNode1.treeIndex);
				tree.push_back(nextNode1);

				// cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;

				if (counter == 1)
				{
					cetak("\n");
				}


				cetak("<-");
				train(df_above, nextNode1.treeIndex, counter);

				if (((tree[treeIndex_yes].isLeaf == true) and (tree[treeIndex_no].isLeaf == true)) and (tree[treeIndex_yes].label == tree[treeIndex_no].label))
				{

					tree[node_index].isLeaf = true;

					string tmp_str = tree[treeIndex_yes].label;

					cetak("+");

					if (tmp_str == "normal") {

						idx_svm++;
						tree[node_index].idx_svm = idx_svm;

						if ((config->train_svm) )
						{

							clear_worker(20);

							cetak("{v {j %d %d} {d %d %d} ", idx_svm, df.getjmlrow_svm(), tree[treeIndex_yes].idx_svm, tree[treeIndex_no].idx_svm);
							f_train_svm(df, idx_svm);
							cetak("}");

							del_model_train(tree[treeIndex_yes].idx_svm);
							del_model_train(tree[treeIndex_no].idx_svm);
						}

					} else {
						cetak("{A}");
					}

					tree[node_index].label = tree[treeIndex_yes].label;
					tree[node_index].children.clear();
					tree[node_index].children.shrink_to_fit();
					tree.erase(tree.begin() + treeIndex_no);
					tree.erase(tree.begin() + treeIndex_yes);
					tree.shrink_to_fit();
					//cetak("\n");
				}
			}



		}

	}

	if (node_index == 0)
	{
		clear_worker(0);
	}

}

void Tdt_build::pruning_dfs(int node_index , Tdataframe & df_train)
{
	cetak(".");

	int left = tree[node_index].children[0];
	int right = tree[node_index].children[1];


	if ((left != -1) and (!tree[left].isLeaf)) {
		Tdataframe df_left;
		df_left = df_train;
		df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
		df_left.clear_col_split();
		pruning_dfs(left, df_left);
		df_left.clear_memory();
	}
	if ((right != -1) and (!tree[right].isLeaf) ) {
		Tdataframe df_right;
		df_right = df_train;
		df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
		df_right.clear_col_split();
		pruning_dfs(right, df_right);
		df_right.clear_memory();

	}


	if ((left != -1) and (right != -1) ) {
		if ((tree[left].isLeaf) and (tree[right].isLeaf))
		{

			cetak("+");

			float error_node, error_left, error_right, sum_error;

			string left_label = tree[left].label;
			string right_label = tree[right].label;

			error_node = df_train.get_estimate_error();
			string node_label = df_train.get_max_label();

			Tdataframe df_left, df_right;
			df_left = df_train;
			df_right = df_train;

			df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
			error_left = df_left.get_estimate_error();

			df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
			error_right = df_right.get_estimate_error();


			sum_error = (((float) df_left.getjmlrow() / df_train.getjmlrow()) * error_left) + (((float) df_right.getjmlrow() / df_train.getjmlrow()) * error_right);

			df_left.clear_memory();
			df_right.clear_memory();
			df_train.clear_memory();


			if (error_node < sum_error)
			{
				cetak("*");
				tree[node_index].children[0] = -1;
				tree[node_index].children[1] = -1;
				tree[node_index].isLeaf = true;
				tree[node_index].label = node_label;

				if (node_label == "normal")
				{
					idx_svm++;
					tree[node_index].idx_svm = idx_svm;

					if ((config->train_svm) ) //and (df.getjmlrow() < 10000)
					{

						clear_worker(0);

						cetak("{v {j %d %d} ", idx_svm, df_train.getjmlrow());
						f_train_svm(df_train, idx_svm);

						cetak("{d ");

						if (left_label == "normal")
						{
							del_model_train(tree[left].idx_svm);
							cetak(" %d ", tree[left].idx_svm);
						}

						if (right_label == "normal")
						{
							del_model_train(tree[right].idx_svm );
							cetak(" %d ", tree[right].idx_svm);
						}

						cetak("} }");

					}

				}

				tree[left].idx_svm = -1;
				tree[right].idx_svm = -1;
			}

		}

	}

	if (node_index == 0)
	{
		clear_worker(0);
	}

}

void Tdt_build::post_pruning(Tdataframe & df_train)
{
	pruning_dfs(0, df_train);
}

void Tdt_build::save_tree()
{
	Twrite_file tmp_wf;
	tmp_wf.setnm_f(config->path_model + "/dtsvm_model.csv");

	string tmp_str = "";
	vector<string> vec;

	for (size_t i = 0; i < tree.size(); ++i)
	{
		tmp_str = to_string(tree[i].criteriaAttrIndex) + "," + tree[i].attrValue + "," + tree[i].label + "," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;

		if (tree[i].children.size() > 0)
		{
			if (tree[i].children.size() == 2) {
				tmp_str += "," + to_string(tree[i].children[0]) + "," + to_string(tree[i].children[1]);
			} else {
				cout << "Ada yg kurang !!!";
			}

		} else {
			tmp_str += ",-1,-1";
		}

		tmp_str += "," + to_string(tree[i].idx_svm);

		// cout << tmp_str << endl;
		vec.push_back(tmp_str);
	}

	tmp_wf.write_file(vec);
	tmp_wf.close_file();
}

void Tdt_build::build_tree()
{
	config->search_uniqe_val = true;

	Tdataframe df_train(config);
	df_train.read_data(config->f_train);
	df_train.read_data_type(config->f_datatype);
	df_train.set_id(0);
	df_train.info();
	df_train.setjmltotalrow();

	cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
	cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.4f credal : %.4f feature_selection :%d normal only : %d  train : %s \n", config->depth, config->min_sample, config->gamma, config->nu, config->credal_s, (config->feature_selection ? 1 : 0), (config->normal_only ? 1 : 0), config->f_train.c_str());
	cetak("Start Train Decission Tree : \n");

	{
		Timer timer;
		train(df_train, 0, 0);
	}

	cetak("End Train Decission Tree : \n");

	config->search_uniqe_val = false;

	cetak("Start Prunning Decission Tree : \n");
	post_pruning(df_train);
	cetak("\nEnd Prunning Decission Tree : \n");

	save_tree();

	df_train.close_file();
}

