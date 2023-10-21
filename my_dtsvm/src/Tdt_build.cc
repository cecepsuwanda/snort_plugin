#include "Tdt_build.h"


Tdt_build::Tdt_build()
{
	idx_svm = 0;
	id_df = 1;
}

Tdt_build::~Tdt_build()
{

}

void Tdt_build::del_dec_tree(tree_node* parent_node)
{
	if (parent_node->left != NULL)
	{
		if (parent_node->left->isLeaf)
		{
			if (parent_node->left->label == "normal")
			{
				del_model_train(parent_node->left->idx_svm);
			}

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
			if (parent_node->right->label == "normal")
			{
				del_model_train(parent_node->right->idx_svm);
			}

			delete parent_node->right;
			parent_node->right = NULL;
		} else {
			del_dec_tree(parent_node->right);
		}
	}

	if ( (parent_node->left == NULL) and (parent_node->right == NULL) )
	{

		if (parent_node->label == "normal")
		{
			del_model_train(parent_node->idx_svm);
		}

		delete parent_node;
		parent_node = NULL;
	}

}

void Tdt_build::del_model_train(int idx)
{
	string filename = global_config.svm_path + "/svm_model_" + to_string(idx) + ".csv";
	remove(filename.c_str());

	filename = global_config.path_model + "/train/train_model_" + to_string(idx) + ".csv";
	remove(filename.c_str());
}

Tmetric_split_value Tdt_build::get_split_value(Tdataframe &df, int idx)
{
	Tmetric_split_value hsl;
	hsl = df.calculate_overall_metric(idx);
	return hsl;
}


void Tdt_build::determine_best_split(Tdataframe &df, int &split_column, Tmy_dttype &split_value)
{
	//cout << "determine_best_split " << endl;

	string current_split_value = "-1";

	split_column = -1;
	split_value.set_value("-1", true);

	Tsplit_value cari_split_value;

	cari_split_value.set_jml_root(df.getjmlrow());

	vector<future<Tmetric_split_value>> async_worker;

	for (int i = 0; i < df.get_jml_valid_attr(); ++i)
	{
		async_worker.push_back(async(std::launch::async, &Tdt_build::get_split_value, ref(df), df.get_valid_attr(i)));
	}

	if (async_worker.size() > 0)
	{

		for (future<Tmetric_split_value> & th : async_worker)
		{
			Tmetric_split_value hsl = th.get();
			if ((hsl.split_value != "-1")) {
				cari_split_value.insert(hsl);
			}
		}

		async_worker.clear();
		async_worker.shrink_to_fit();

	}

	if (!cari_split_value.is_empty())
	{
		cari_split_value.hitung_sd();
		cari_split_value.kalkulasi_id_max();

		split_column = cari_split_value.get_split_column();
		split_value = cari_split_value.get_split_value();
	}

	df.clear_map_col_split();
}

void Tdt_build::thread_save_train( vector<vector<string>> table, int v_idx_svm)
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

void Tdt_build::thread_train_svm( vector<vector<string>> table, int v_idx_svm)
{
	Tglobal_config v_config;
	Tmy_svm my_svm;
	my_svm.train(table);
	my_svm.save_model(v_config.svm_path + "/svm_model_" + to_string(v_idx_svm) + ".csv");
}

void Tdt_build::f_train_svm(Tdataframe &df, int v_idx_svm)
{
	vector<vector<string>> table = df.get_all_record_svm();

	if (global_config.save_train) {

		worker.push_back(thread(&Tdt_build::thread_save_train, table, v_idx_svm));
		//thread_save_test(config,table,v_idx_svm);

	}

	worker.push_back(thread(&Tdt_build::thread_train_svm, table, v_idx_svm));

}

void Tdt_build::clear_worker(size_t limit)
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

string Tdt_build::create_leaf(Tdataframe & df)
{
	string vlabel = df.get_max_label();
	return vlabel;
}


bool Tdt_build::check_purity(Tdataframe &df)
{
	return df.is_single_label();
}

bool Tdt_build::limit_jml_dt_cabang(int jml_root, int jml_below, int jml_above)
{
	int jml_row = jml_root;
	float prosen = 1;
	float prosen1 = 0;

	float jml_row_prosen = jml_row;
	float jml_row_prosen1 = 0;

	if (global_config.threshold < 1)
	{
		prosen = 1 - global_config.threshold;
		prosen1 = global_config.threshold;

		jml_row_prosen = ceil(prosen * jml_row);
		jml_row_prosen1 =  ceil(prosen1 * jml_row);
	} else {
		jml_row_prosen = jml_row - global_config.threshold;
		jml_row_prosen1 =  global_config.threshold;
	}

	//pesan.cetak("[%d %d %d %d]\n", global_config.threshold, jml_root, jml_below, jml_above);

	return ((jml_below >= jml_row_prosen1 ) and (jml_below <= jml_row_prosen));
}

tree_node* Tdt_build::train(Tdataframe & df , int counter)
{
	tree_node* parent_node = new tree_node;

	//cout << counter;
	pesan.cetak("[%d %d]", counter, df.getjmlrow());

	int split_column = -1;
	Tmy_dttype split_value;

	if (check_purity(df) or (df.getjmlrow() < global_config.min_sample) or (counter >= (global_config.depth-1)) )
	{
		string tmp_str = create_leaf(df);
		pesan.cetak("*");

		if (tmp_str == "normal") {

			idx_svm++;
			parent_node->idx_svm = idx_svm;

			if ((global_config.train_svm))
			{
				clear_worker(global_config.jml_thread);

				pesan.cetak("{v %d %d ", idx_svm, df.getjmlrow_svm());
				f_train_svm(df, idx_svm);
				pesan.cetak("}");

			}

		} else {
			pesan.cetak("{A}");
		}

		parent_node->isLeaf = true;
		parent_node->label = tmp_str;

		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");


		df.clear_memory();
		df.clear_col_split();

		pesan.cetak("\n");

	} else {

		pesan.cetak("?");

		counter++;

		parent_node->label = df.get_max_label();
		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");

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

		bool is_pass = limit_jml_dt_cabang(df.getjmlrow(), df_below.getjmlrow(), df_above.getjmlrow());

		if ((((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0))  or (split_value == "-1")) or (!is_pass) ) {
			string tmp_str = create_leaf(df);

			pesan.cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				parent_node->idx_svm = idx_svm;

				if ((global_config.train_svm))
				{

					clear_worker(global_config.jml_thread);

					pesan.cetak("{v %d %d ", idx_svm, df.getjmlrow_svm());
					f_train_svm(df, idx_svm);
					pesan.cetak("}");

				}

			} else {
				pesan.cetak("{A}");
			}

			parent_node->isLeaf = true;
			parent_node->label = tmp_str;

			parent_node->jml_known = df.get_jml_stat("known");
			parent_node->jml_normal = df.get_jml_stat("normal");


			df_below.clear_memory();
			df_above.clear_memory();

			df_below.clear_col_split();
			df_above.clear_col_split();

			pesan.cetak("\n");

		} else {

			if (split_value != "-1")
			{

				pesan.cetak("|");

				parent_node->criteriaAttrIndex = split_column;


				// cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode.opt == 0 ? "<=" : "==") << nextNode.attrValue << endl;
				pesan.cetak("->");
				tree_node* yes_node = train(df_below, counter);
				yes_node->attrValue = split_value;
				yes_node->opt = df.get_opt(split_column, 1);


				// cout << tree[node_index].criteriaAttrIndex << " " << df.get_nm_header(tree[node_index].criteriaAttrIndex) << (nextNode1.opt == 1 ? ">" : "!=") << nextNode1.attrValue << endl;

				if (counter == 1)
				{
					pesan.cetak("\n");
				}


				pesan.cetak("<-");
				tree_node* no_node = train(df_above, counter);
				no_node->attrValue = split_value;
				no_node->opt = df.get_opt(split_column, 0);

				if (global_config.prunning) {

					if (((yes_node->isLeaf == true) and (no_node->isLeaf == true)) and (yes_node->label == no_node->label))
					{

						parent_node->isLeaf = true;

						string tmp_str = yes_node->label;

						pesan.cetak("+");

						if (tmp_str == "normal") {

							idx_svm++;
							parent_node->idx_svm = idx_svm;

							if ((global_config.train_svm) )
							{

								clear_worker(global_config.jml_thread);

								pesan.cetak("{v {j %d %d} {d %d %d} ", idx_svm, df.getjmlrow_svm(), yes_node->idx_svm, no_node->idx_svm);
								f_train_svm(df, idx_svm);
								pesan.cetak("}");

								del_model_train(yes_node->idx_svm);
								del_model_train(no_node->idx_svm);
							}

						} else {
							pesan.cetak("{A}");
						}

						parent_node->criteriaAttrIndex = -1;
						parent_node->label = yes_node->label;
						delete yes_node;
						delete no_node;
						yes_node = NULL;
						no_node = NULL;
						//cetak("\n");
					}
				}


				parent_node->left = yes_node;
				parent_node->right = no_node;

			}

		}

	}

	if (counter == 0)
	{
		clear_worker(0);
	}

	return parent_node;

}

void Tdt_build::subtree_stat(tree_node * parent_node, bool is_pruning, int &jml_leaf, int &sum_min)
{
	bool is_pass = parent_node->isLeaf;
	if (is_pruning)
	{
		is_pass = is_pass or parent_node->is_pruning_1;
	}

	if (is_pass)
	{
		jml_leaf = jml_leaf + 1;
		sum_min = sum_min + min(parent_node->jml_known, parent_node->jml_normal);
	} else {
		if (parent_node->left != NULL) {
			subtree_stat(parent_node->left, is_pruning, jml_leaf, sum_min);
		}

		if (parent_node->right != NULL) {
			subtree_stat(parent_node->right, is_pruning, jml_leaf, sum_min);
		}

	}
}


void Tdt_build::pruning_method_2(tree_node* parent_node, Tdataframe &df_train)
{
	pesan.cetak(".");

	if (parent_node->left != NULL)
	{
		if (!parent_node->left->isLeaf)
		{
			Tdataframe df_left;
			df_left = df_train;
			df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue);
			df_left.clear_col_split();
			pruning_method_2(parent_node->left, df_left);
			df_left.clear_memory();

		}
	}

	if (parent_node->right != NULL)
	{
		if (!parent_node->right->isLeaf)
		{
			Tdataframe df_right;
			df_right = df_train;
			df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue);
			df_right.clear_col_split();
			pruning_method_2(parent_node->right, df_right);
			df_right.clear_memory();
		}
	}

	if ((parent_node->left != NULL) and (parent_node->right != NULL) ) {
		if ((parent_node->left->isLeaf) and (parent_node->right->isLeaf))
		{

			int jml_sample = parent_node->jml_known + parent_node->jml_normal;
			int root_false_predic = min(parent_node->jml_known, parent_node->jml_normal);

			float root_error = root_false_predic + global_config.skala_pruning;

			int branch_false_predic = min(parent_node->left->jml_known, parent_node->left->jml_normal) + min(parent_node->right->jml_known, parent_node->right->jml_normal);

			float branch_error = branch_false_predic + (2 * global_config.skala_pruning) ;

			float se = sqrt((branch_error * (jml_sample - branch_error)) / jml_sample);

			branch_error = branch_error + se;
			//cout << "[" << root_error << " " << branch_error << "]";

			if (root_error <= branch_error)
			{
				cout << " cut off " << parent_node->criteriaAttrIndex << " " << parent_node->left->attrValue.get_string() << " ";

				if (parent_node->left->label == "normal")
				{
					del_model_train(parent_node->left->idx_svm);
				}

				delete parent_node->left;

				if (parent_node->right->label == "normal")
				{
					del_model_train(parent_node->right->idx_svm);
				}

				delete parent_node->right;

				parent_node->left = NULL;
				parent_node->right = NULL;

				parent_node->isLeaf = true;

				if (parent_node->label == "normal")
				{
					idx_svm++;
					parent_node->idx_svm = idx_svm;

					if ((global_config.train_svm))
					{
						clear_worker(global_config.jml_thread);

						pesan.cetak("{v %d %d ", idx_svm, df_train.getjmlrow_svm());
						f_train_svm(df_train, idx_svm);
						pesan.cetak("}");
					}

				}

			}

		}

	}

}

void Tdt_build::pruning_method_3(tree_node * parent_node, Tdataframe &df_train)
{
	pesan.cetak(".");

	//pesan.cetak("[%d %s %d %d]", parent_node->depth, parent_node->label.c_str(), parent_node->jml_known, parent_node->jml_normal);

	Tlabel_stat root_stat, left_stat, right_stat;
	float root_error = 0.0;

	bool del_branch = false, cut_left = false, cut_right = false;

	if ((parent_node->left != NULL) or (parent_node->right != NULL)) {
		//pesan.cetak("+");

		int jml_sample = parent_node->jml_known + parent_node->jml_normal;
		int root_false_predic = min(parent_node->jml_known, parent_node->jml_normal);

		root_error = root_false_predic + global_config.skala_pruning;

		if (parent_node->left != NULL)
		{
			int jml_leaf = 0;
			int sum_min = 0;

			parent_node->left->is_pruning_1 = true;
			subtree_stat(parent_node, true, jml_leaf, sum_min);
			parent_node->left->is_pruning_1 = false;

			int branch_false_predic = sum_min;

			float branch_error_left = branch_false_predic + (jml_leaf * global_config.skala_pruning) ;
			//cout << "[" << branch_false_predic << " " << jml_leaf << "]" << endl;

			float se = sqrt((branch_error_left * (jml_sample - branch_error_left)) / jml_sample);

			branch_error_left = branch_error_left + se;
			//cout << "[" << root_error << " " << branch_error_left << "]" << endl;

			if (root_error >= branch_error_left)
			{
				cut_left = true;
			}
		}

		if (parent_node->right != NULL)
		{
			int jml_leaf = 0;
			int sum_min = 0;

			parent_node->right->is_pruning_1 = true;
			subtree_stat(parent_node, true, jml_leaf, sum_min);
			parent_node->right->is_pruning_1 = false;

			int branch_false_predic = sum_min;
			float branch_error_right = branch_false_predic + (jml_leaf * global_config.skala_pruning) ;
			//cout << "[" << branch_false_predic << " " << jml_leaf << "]" << endl;

			float se = sqrt((branch_error_right * (jml_sample - branch_error_right)) / jml_sample);

			branch_error_right = branch_error_right + se;
			//cout << "[" << root_error << " " << branch_error_right << "]" << endl;

			if (root_error >= branch_error_right)
			{
				cut_right = true;
			}
		}

		if (cut_right and cut_left)
		{
			del_branch = false;
		} else {
			if (cut_right or cut_left)
			{
				del_branch = true;
			}
		}


	}
	if (del_branch) {
		del_branch = false;
		//pesan.cetak("*");

		if ((parent_node->left != NULL) and cut_left) {
			cut_left = false;

			cout << " cut off left " << parent_node->criteriaAttrIndex << " " << parent_node->left->attrValue.get_string() << " ";

			if (parent_node->left->left != NULL)
			{
				del_dec_tree(parent_node->left->left);				
				parent_node->left->left = NULL;
			}

			if (parent_node->left->right != NULL)
			{				
				del_dec_tree(parent_node->left->right);
				parent_node->left->right = NULL;
			}


			parent_node->left->isLeaf = true;

			if (parent_node->left->label == "normal")
			{
				idx_svm++;
				parent_node->left->idx_svm = idx_svm;

				if ((global_config.train_svm))
				{
					clear_worker(global_config.jml_thread);

					pesan.cetak("{v %d %d ", idx_svm, df_train.getjmlrow_svm());
					f_train_svm(df_train, idx_svm);
					pesan.cetak("}");
				}
			}
		}

		if ((parent_node->right != NULL) and cut_right) {
			cut_right = false;

			cout << " cut off right " << parent_node->criteriaAttrIndex << " " << parent_node->right->attrValue.get_string()  << " ";

			if (parent_node->right->left != NULL)
			{
				del_dec_tree(parent_node->right->left);
				parent_node->right->left = NULL;
			}

			if (parent_node->right->right != NULL)
			{
				del_dec_tree(parent_node->right->right);
				parent_node->right->right = NULL;
			}


			parent_node->right->isLeaf = true;
			if (parent_node->right->label == "normal")
			{
				idx_svm++;
				parent_node->right->idx_svm = idx_svm;

				if ((global_config.train_svm))
				{
					clear_worker(global_config.jml_thread);

					pesan.cetak("{v %d %d ", idx_svm, df_train.getjmlrow_svm());
					f_train_svm(df_train, idx_svm);
					pesan.cetak("}");
				}
			}
		}


	}

	if (parent_node->left != NULL) {
		//pesan.cetak("->");

		Tdataframe df_left;
		df_left = df_train;
		df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue);
		df_left.clear_col_split();
		pruning_method_3(parent_node->left, df_left);
		df_left.clear_memory();
	}

	// if (parent_node->depth == 1)
	// {
	// 	pesan.cetak("\n");
	// }

	if (parent_node->right != NULL) {
		//pesan.cetak("<-");

		Tdataframe df_right;
		df_right = df_train;
		df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue);
		df_right.clear_col_split();
		pruning_method_3(parent_node->right, df_right);
		df_right.clear_memory();
	}



}

void Tdt_build::pruning_dfs(int node_index , Tdataframe & df_train)
{
	// pesan.cetak(".");

	// int left = tree[node_index].children[0];
	// int right = tree[node_index].children[1];


	// if ((left != -1) and (!tree[left].isLeaf)) {
	// 	Tdataframe df_left;
	// 	df_left = df_train;
	// 	df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
	// 	df_left.clear_col_split();
	// 	pruning_dfs(left, df_left);
	// 	df_left.clear_memory();
	// }
	// if ((right != -1) and (!tree[right].isLeaf) ) {
	// 	Tdataframe df_right;
	// 	df_right = df_train;
	// 	df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
	// 	df_right.clear_col_split();
	// 	pruning_dfs(right, df_right);
	// 	df_right.clear_memory();

	// }


	// if ((left != -1) and (right != -1) ) {
	// 	if ((tree[left].isLeaf) and (tree[right].isLeaf))
	// 	{

	// 		pesan.cetak("+");

	// 		float error_node, error_left, error_right, sum_error;

	// 		string left_label = tree[left].label;
	// 		string right_label = tree[right].label;

	// 		error_node = df_train.get_estimate_error();
	// 		string node_label = df_train.get_max_label();

	// 		Tdataframe df_left, df_right;
	// 		df_left = df_train;
	// 		df_right = df_train;

	// 		df_left.add_filter(tree[node_index].criteriaAttrIndex, tree[left].opt, tree[left].attrValue);
	// 		error_left = df_left.get_estimate_error();

	// 		df_right.add_filter(tree[node_index].criteriaAttrIndex, tree[right].opt, tree[right].attrValue);
	// 		error_right = df_right.get_estimate_error();


	// 		sum_error = (((float) df_left.getjmlrow() / df_train.getjmlrow()) * error_left) + (((float) df_right.getjmlrow() / df_train.getjmlrow()) * error_right);

	// 		df_left.clear_memory();
	// 		df_right.clear_memory();
	// 		df_train.clear_memory();


	// 		if (error_node < sum_error)
	// 		{
	// 			pesan.cetak("*");
	// 			tree[node_index].children[0] = -1;
	// 			tree[node_index].children[1] = -1;
	// 			tree[node_index].isLeaf = true;
	// 			tree[node_index].label = node_label;

	// 			if (node_label == "normal")
	// 			{
	// 				idx_svm++;
	// 				tree[node_index].idx_svm = idx_svm;

	// 				if ((global_config.train_svm) ) //and (df.getjmlrow() < 10000)
	// 				{

	// 					clear_worker(0);

	// 					pesan.cetak("{v {j %d %d} ", idx_svm, df_train.getjmlrow());
	// 					f_train_svm(df_train, idx_svm);

	// 					pesan.cetak("{d ");

	// 					if (left_label == "normal")
	// 					{
	// 						del_model_train(tree[left].idx_svm);
	// 						pesan.cetak(" %d ", tree[left].idx_svm);
	// 					}

	// 					if (right_label == "normal")
	// 					{
	// 						del_model_train(tree[right].idx_svm );
	// 						pesan.cetak(" %d ", tree[right].idx_svm);
	// 					}

	// 					pesan.cetak("} }");

	// 				}

	// 			}

	// 			tree[left].idx_svm = -1;
	// 			tree[right].idx_svm = -1;
	// 		}

	// 	}

	// }

	// if (node_index == 0)
	// {
	// 	clear_worker(0);
	// }

}

void Tdt_build::post_pruning(Tdataframe & df_train)
{
	 pruning_method_2(dec_tree, df_train);
	 clear_worker(0);
	 pruning_method_3(dec_tree, df_train);
	 clear_worker(0);
	//pruning_dfs(0, df_train);
}

void Tdt_build::dec_tree_to_vec_tree(tree_node * parent_node, int node_index)
{
	if (node_index == 0)
	{
		Node root;
		root.treeIndex = 0;
		tree.push_back(root);
	}

	if (parent_node->isLeaf)
	{
		if (parent_node->label == "normal")
		{
			idx_svm++;
			tree[node_index].idx_svm = parent_node->idx_svm;
			// if (idx_svm != parent_node->idx_svm)
			// {
			// 	tree[node_index].idx_svm = idx_svm;
			// }

		}

		tree[node_index].isLeaf = parent_node->isLeaf;
		tree[node_index].label = parent_node->label;

	} else {

		tree[node_index].criteriaAttrIndex = parent_node->criteriaAttrIndex;

		Node nextNode;
		nextNode.treeIndex = (int)tree.size();
		nextNode.attrValue = parent_node->left->attrValue;
		nextNode.opt = parent_node->left->opt;
		tree[node_index].children.push_back(nextNode.treeIndex);
		tree.push_back(nextNode);

		dec_tree_to_vec_tree(parent_node->left, nextNode.treeIndex);


		Node nextNode1;
		nextNode1.treeIndex = (int)tree.size();
		nextNode1.attrValue = parent_node->right->attrValue;
		nextNode1.opt = parent_node->right->opt;
		tree[node_index].children.push_back(nextNode1.treeIndex);
		tree.push_back(nextNode1);

		dec_tree_to_vec_tree(parent_node->right, nextNode1.treeIndex);

	}

}


void Tdt_build::save_tree()
{
	idx_svm = 0;
	
    dec_tree_to_vec_tree(dec_tree, 0);

	Twrite_file tmp_wf;
	tmp_wf.setnm_f(global_config.path_model + "/dtsvm_model.csv");

	string tmp_str = "";
	vector<string> vec;

	for (size_t i = 0; i < tree.size(); ++i)
	{
		tmp_str = to_string(tree[i].criteriaAttrIndex) + "," + tree[i].attrValue.get_string() + "," + tree[i].label + "," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;

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
	global_config.search_uniqe_val = true;

	Tdataframe df_train;
	df_train.read_data(global_config.f_train);
	df_train.read_data_type(global_config.f_datatype);
	df_train.set_id(0);
	df_train.info();
	df_train.setjmltotalrow();

	pesan.cetak("Train : Jumlah Baris : %d Jumlah Kolom : %d \n", df_train.getjmlrow(), df_train.getjmlcol());
	pesan.cetak("Depth : %d Minimum Sample : %d gamma : %.4f nu : %.4f credal : %.4f feature_selection :%d normal only : %d  train : %s \n", global_config.depth, global_config.min_sample, global_config.gamma, global_config.nu, global_config.credal_s, (global_config.feature_selection ? 1 : 0), (global_config.normal_only ? 1 : 0), global_config.f_train.c_str());
	pesan.cetak("Start Train Decission Tree : \n");

	{
		Timer timer;
		dec_tree = train(df_train, 0);
		clear_worker(0);
	}

	pesan.cetak("End Train Decission Tree : \n");

	global_config.search_uniqe_val = false;

	if (global_config.prunning) {
		pesan.cetak("Start Prunning Decission Tree : \n");
		post_pruning(df_train);
		pesan.cetak("\nEnd Prunning Decission Tree : \n");
	}

	save_tree();

	df_train.close_file();
}




Tdt_build::Tsplit_value::Tsplit_value()
{
	_sum_neg = 0.0;
	_sum_po = 0.0;
	_rata2 = 0.0;
	_sd = 0.0;
	_jml = 0;
	_jml_root = 0;

	_split_column = -1;
	_split_value.set_value("-1", true);

	_list_split_value.clear();
	_idx_max_gain_ratio.clear();
	_idx_rata2.clear();
}

Tdt_build::Tsplit_value::~Tsplit_value()
{
	_list_split_value.clear();
	_idx_max_gain_ratio.clear();
	_idx_rata2.clear();
}

void Tdt_build::Tsplit_value::set_jml_root(int jml)
{
	_jml_root = jml;
}

void Tdt_build::Tsplit_value::insert(Tmetric_split_value value)
{
	_list_split_value.push_back(value);

	_sum_po += (value.max_gain > 0.0) ? value.max_gain : 0.0;
	_sum_neg += (value.max_gain < 0.0) ? value.max_gain : 0.0;
	_jml++;

	_rata2 = (_sum_po - abs(_sum_neg)) / _jml;
}

void Tdt_build::Tsplit_value::hitung_sd()
{
	double tmp_sum = 0.0;
	for (auto itr = _list_split_value.begin(); itr != _list_split_value.end(); ++itr)
	{
		tmp_sum += pow((itr->max_gain - _rata2), 2);
	}

	if (_jml > 1)
	{
		_sd = sqrt(tmp_sum / (_jml - 1));
	}
}

bool Tdt_build::Tsplit_value::limit_jml_dt_cabang(int jml_below, int jml_above)
{
	int jml_row = _jml_root;
	float prosen = 1;
	float prosen1 = 0;

	float jml_row_prosen = jml_row;
	float jml_row_prosen1 = 0;

	if (global_config.threshold < 1)
	{
		prosen = 1 - global_config.threshold;
		prosen1 = global_config.threshold;

		jml_row_prosen = ceil(prosen * jml_row);
		jml_row_prosen1 =  ceil(prosen1 * jml_row);
	} else {
		jml_row_prosen = jml_row - global_config.threshold;
		jml_row_prosen1 =  global_config.threshold;
	}

	return ((jml_below >= jml_row_prosen1 ) and (jml_below <= jml_row_prosen));
}

bool Tdt_build::Tsplit_value::is_empty()
{
	return _jml == 0;
}

void Tdt_build::Tsplit_value::kalkulasi_id_max()
{
	bool first_iteration = true;
	float max_gain_ratio = 0.0;

	//cout << " max_gain_ratio " << endl;
	for (size_t i = 0; i < _list_split_value.size(); ++i)
	{
		if (first_iteration or (max_gain_ratio < _list_split_value[i].max_gain_ratio))
		{
			//cout << _list_split_value[i].idx << "," << _list_split_value[i].split_value.get_string() << endl;
			_idx_max_gain_ratio.push_back(i);
			max_gain_ratio = _list_split_value[i].max_gain_ratio;
			first_iteration = false;
		}
	}

	int idx_gain_ratio_max = -1;

	_split_column = -1;
	_split_value.set_value("-1", true);

	if (_idx_max_gain_ratio.size() > 0)
	{
		idx_gain_ratio_max = _idx_max_gain_ratio[_idx_max_gain_ratio.size() - 1];

		_split_column = _list_split_value[idx_gain_ratio_max].idx;
		_split_value = _list_split_value[idx_gain_ratio_max].split_value;
	}

	if (global_config.gunakan_rata2gain)
	{
		//cout << " rata2 " << endl;

		for (size_t i = 0; i < _idx_max_gain_ratio.size(); ++i)
		{
			size_t tmp_idx = _idx_max_gain_ratio[i];
			double z_score = 0.0;

			if (_sd != 0)
			{
				z_score = ((_list_split_value[tmp_idx].max_gain - _rata2) / _sd);
			}

			bool pass = (z_score > 0.0); //and (z_score < 3.0)
			if (pass)
			{
				//cout << _list_split_value[tmp_idx].idx << "," << _list_split_value[tmp_idx].split_value.get_string() << endl;
				_idx_rata2.push_back(tmp_idx);
			}
		}

		idx_gain_ratio_max = -1;

		_split_column = -1;
		_split_value.set_value("-1", true);

		if (_idx_rata2.size() > 0)
		{
			idx_gain_ratio_max = _idx_rata2[_idx_rata2.size() - 1];

			_split_column = _list_split_value[idx_gain_ratio_max].idx;
			_split_value = _list_split_value[idx_gain_ratio_max].split_value;
		}

	}

	bool pass = true;

	if (idx_gain_ratio_max != -1)
	{
		pass = limit_jml_dt_cabang(_list_split_value[idx_gain_ratio_max].jml_below, _list_split_value[idx_gain_ratio_max].jml_above);
	}

	if (!pass)
	{
		if (global_config.find_other_attr)
		{
			idx_gain_ratio_max = -1;

			_split_column = -1;
			_split_value.set_value("-1", true);

			if (global_config.gunakan_rata2gain)
			{
				if (_idx_rata2.size() > 0)
				{
					int tmp_idx = -1;
					size_t i = 0;
					while ( (i < (_idx_rata2.size() - 1)) and !pass)
					{
						tmp_idx = _idx_rata2[_idx_rata2.size() - (i + 1)];
						pass = limit_jml_dt_cabang(_list_split_value[tmp_idx].jml_below, _list_split_value[tmp_idx].jml_above);
						i++;
					}

					if (pass and (tmp_idx != -1))
					{
						idx_gain_ratio_max = tmp_idx;
						_split_column = _list_split_value[idx_gain_ratio_max].idx;
						_split_value = _list_split_value[idx_gain_ratio_max].split_value;
					}
				}

			} else {

				if (_idx_max_gain_ratio.size() > 0)
				{
					int tmp_idx = -1;
					size_t i = 0;
					while ( (i < (_idx_max_gain_ratio.size() - 1)) and !pass)
					{
						tmp_idx = _idx_max_gain_ratio[_idx_max_gain_ratio.size() - (i + 1)];
						pass = limit_jml_dt_cabang(_list_split_value[tmp_idx].jml_below, _list_split_value[tmp_idx].jml_above);
						i++;
					}

					if (pass and (tmp_idx != -1))
					{
						idx_gain_ratio_max = tmp_idx;
						_split_column = _list_split_value[idx_gain_ratio_max].idx;
						_split_value = _list_split_value[idx_gain_ratio_max].split_value;
					}
				}
			}
		}
	}


	if (idx_gain_ratio_max != -1)
	{
		max_gain_ratio = _list_split_value[idx_gain_ratio_max].max_gain_ratio;

		if (max_gain_ratio < 0)
		{
			_split_column = -1;
			_split_value.set_value("-1", true);
		}
	}

}

int Tdt_build::Tsplit_value::get_split_column()
{
	return _split_column;
}

Tmy_dttype Tdt_build::Tsplit_value::get_split_value()
{
	return _split_value;
}


