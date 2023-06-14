#include "Tdt_build.h"


Tdt_build::Tdt_build(Tconfig *v_config)
{
	idx_svm = 0;

	config = v_config;

	dec_tree = NULL;
}

Tdt_build::~Tdt_build()
{
	tree.clear();
	prev_tree.clear();
	del_dec_tree(dec_tree);
}

void Tdt_build::del_dec_tree(tree_node* parent_node)
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

	df.search_col_split();

	float max_gain = 0;
	bool  first_iteration = true;

	string current_split_value = "-1";
	//float current_overall_metric = -1;

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

	df.clear_map_col_split();
}

void Tdt_build::clear_worker(size_t limit)
{

	bool pass = limit == 0  ? true : worker.size() == limit ;

	if (pass)
	{
		for (std::thread & th : worker)
		{
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


void Tdt_build::train(Tdataframe &df, tb_missing_branch &missing_branch, tree_node* parent_node)
{
	cetak(".");
	//cetak("%d", parent_node->depth);

	if (parent_node->isLeaf)
	{
		df.ReFilter(false);
		if (parent_node->is_lanjut) {
			tree_node* tmp_node = train_prev_tree(df, missing_branch, parent_node->depth, parent_node);
			parent_node = tmp_node;
		} else {
			posisi_cabang tmp_posisi = df.get_posisi();

			string tmp_str = parent_node->label;
			//cetak("*");

			if (tmp_str == "normal") {
				idx_svm++;
				parent_node->idx_svm = idx_svm;
				//cetak("{N}\n");
			} else {
				//cetak("{A}\n");
			}

			if (parent_node->is_pure or parent_node->is_min_sample or parent_node->is_depth_limit)
			{
				missing_branch.insert_cut_off(tmp_posisi, tmp_str, (parent_node->is_pure ? 1 : 0) , (parent_node->is_min_sample ? 1 : 0) , (parent_node->is_depth_limit ? 1 : 0), (parent_node->is_lanjut ? 1 : 0));
			}

			if (parent_node->is_same_label)
			{
				missing_branch.insert_same_label(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));
			}

			if (parent_node->is_not_split)
			{
				missing_branch.insert_not_split(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));
			}

			if (parent_node->is_pruning)
			{
				missing_branch.insert_pruning(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0 ));
			}
		}

	} else {
		//cetak("?|");

		posisi_cabang tmp_posisi = df.get_posisi();
		if (parent_node->left != NULL) {
			missing_branch.add_branch(tmp_posisi, parent_node->criteriaAttrIndex, -1, parent_node->left->attrValue);
		} else {
			if (parent_node->right != NULL) {
				missing_branch.add_branch(tmp_posisi, parent_node->criteriaAttrIndex, -1, parent_node->right->attrValue);
			} else {
				if (!parent_node->isLeaf)
				{
					cetak(" kacau !!! ");
					cetak("\n child_depth = %d child_branch = %d child_branch_number = %d parent_depth = %d parent_branch = %d parent_branch_number = %d \n", tmp_posisi.child_depth, tmp_posisi.child_branch, tmp_posisi.child_branch_number, tmp_posisi.parent_depth, tmp_posisi.parent_branch, tmp_posisi.parent_branch_number);
				}
			}
		}

		Tdataframe df_below, df_above;

		if (parent_node->left != NULL) {
			//cetak("->");

			df_below = df;
			df_below.switch_parent_branch();
			df_below.set_branch(parent_node->left->depth, parent_node->left->branch, parent_node->left->branch_number);
			df_below.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, false, false);

			posisi_cabang tmp_posisi_below = df_below.get_posisi();
			missing_branch.add_branch(tmp_posisi_below, parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue);

			train(df_below, missing_branch, parent_node->left);
		}

		if (parent_node->right != NULL) {
			//cetak("<-");

			df_above = df;
			df_above.switch_parent_branch();
			df_above.set_branch(parent_node->right->depth, parent_node->right->branch, parent_node->right->branch_number);
			df_above.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, false, false);

			posisi_cabang tmp_posisi_above = df_above.get_posisi();
			missing_branch.add_branch(tmp_posisi_above, parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue);

			train(df_above, missing_branch, parent_node->right);
		}



		if ((parent_node->left != NULL) and (parent_node->right != NULL))
		{
			if (parent_node->left->isLeaf and parent_node->right->isLeaf)
			{
				if (parent_node->left->label == parent_node->right->label)
				{
					parent_node->isLeaf = true;

					string tmp_str = parent_node->left->label;

					//cetak("+");

					if (tmp_str == "normal") {
						idx_svm++;
						parent_node->idx_svm = idx_svm;
						//cetak("{N}");
					} else {
						//cetak("{A}");
					}

					parent_node->is_lanjut = parent_node->left->is_lanjut or parent_node->right->is_lanjut;

					parent_node->criteriaAttrIndex = -1;
					parent_node->label = parent_node->left->label;
					delete parent_node->left;
					delete parent_node->right;
					parent_node->left = NULL;
					parent_node->right = NULL;

					df_below.clear_memory(0);
					df_above.clear_memory(0);
					df.clear_memory(1);

					missing_branch.insert_same_label(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));
				}
			}
		}
	}

}



tree_node* Tdt_build::train(Tdataframe &df, tb_missing_branch &missing_branch, int counter)
{
	tree_node* parent_node = new tree_node;

	//cetak(".");
	cetak("[%d %d]", counter, df.getjmlrow());

	bool is_pure = check_purity(df);
	bool is_min_sample = (df.getjmlrow() < config->min_sample);
	bool is_depth_limit = (counter >= config->depth);

	posisi_cabang tmp_posisi = df.get_posisi();
	int split_column = -1;
	string split_value = "-1";

	if (is_pure or is_min_sample or is_depth_limit)
	{
		string tmp_str = create_leaf(df);
		cetak("*");

		if (tmp_str == "normal") {
			idx_svm++;
			parent_node->idx_svm = idx_svm;
			cetak("{N}");

		} else {
			cetak("{A}");
		}

		parent_node->isLeaf = true;
		parent_node->label = tmp_str;

		//df.clear_memory();
		df.clear_col_split();

		parent_node->is_lanjut = !(is_pure or is_min_sample);

		missing_branch.insert_cut_off(tmp_posisi, tmp_str, (is_pure ? 1 : 0) , (is_min_sample ? 1 : 0) , (is_depth_limit ? 1 : 0), (parent_node->is_lanjut ? 1 : 0));

		cetak("\n");

	} else {

		cetak("?");

		counter++;

		df.stat_tabel(true, true, true);
		determine_best_split(df, split_column, split_value);

		cetak(" [%d,%s] ", split_column, split_value.c_str());


		if (split_value != "-1")
		{

			missing_branch.add_branch(tmp_posisi, split_column, -1, split_value);

			Tdataframe df_below, df_above;

			auto itr = branch_number.find(counter);
			if (itr == branch_number.end()) {
				branch_number.insert({counter, 1});
			} else {
				branch_number[counter] = branch_number[counter] + 1;
			}

			df_below = df;
			df_below.switch_parent_branch();
			df_below.set_branch(counter, 1, branch_number[counter]);

			branch_number[counter] = branch_number[counter] + 1;

			df_above = df;
			df_above.switch_parent_branch();
			df_above.set_branch(counter, 2, branch_number[counter]);

			int jml_row = df.getjmlrow();
			float prosen = 1;
			float prosen1 = 0;

			float jml_row_prosen = jml_row;
			float jml_row_prosen1 = 0;

			if (config->threshold < 1)
			{
				prosen = 1 - config->threshold;
				prosen1 = config->threshold;

				jml_row_prosen = ceil(prosen * jml_row);
				jml_row_prosen1 =  ceil(prosen1 * jml_row);
			} else {
				jml_row_prosen = jml_row - config->threshold;
				jml_row_prosen1 =  config->threshold;
			}

			df.split_data(split_column, split_value, df_below, df_above);

            bool is_pass = (df_below.getjmlrow() >= jml_row_prosen1 ) and (df_below.getjmlrow() <= jml_row_prosen);

			if ( ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) or (!is_pass) ) {
				string tmp_str = create_leaf(df);

				cetak("-");

				if (tmp_str == "normal") {
					idx_svm++;
					parent_node->idx_svm = idx_svm;
					cetak("{N}");
				} else {
					cetak("{A}");
				}

				parent_node->isLeaf = true;
				parent_node->label = tmp_str;

				df_below.clear_memory(0);
				df_above.clear_memory(0);

				df_below.clear_col_split();
				df_above.clear_col_split();

				df.clear_memory(1);
				df.clear_col_split();

				parent_node->is_lanjut = false;

				missing_branch.insert_not_split(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

				cetak("\n");

			} else {
				cetak("|");

				parent_node->criteriaAttrIndex = split_column;

				cetak("->");
				posisi_cabang tmp_posisi_below = df_below.get_posisi();
				missing_branch.add_branch(tmp_posisi_below, split_column, df.get_opt(split_column, 1), split_value);

				tree_node* yes_node = train(df_below, missing_branch, counter);
				yes_node->attrValue = split_value;
				yes_node->opt = df.get_opt(split_column, 1);

				yes_node->depth = tmp_posisi_below.child_depth;
				yes_node->branch = tmp_posisi_below.child_branch;
				yes_node->branch_number = tmp_posisi_below.child_branch_number;


				if (counter == 1)
				{
					cetak("\n");
				}


				cetak("<-");
				posisi_cabang tmp_posisi_above = df_above.get_posisi();
				missing_branch.add_branch(tmp_posisi_above, split_column, df.get_opt(split_column, 0), split_value);

				tree_node* no_node = train(df_above, missing_branch, counter);
				no_node->attrValue = split_value;
				no_node->opt = df.get_opt(split_column, 0);

				no_node->depth = tmp_posisi_above.child_depth;
				no_node->branch = tmp_posisi_above.child_branch;
				no_node->branch_number = tmp_posisi_above.child_branch_number;

				if (config->prunning) {

					if (((yes_node->isLeaf == true) and (no_node->isLeaf == true)) and (yes_node->label == no_node->label))
					{

						parent_node->isLeaf = true;

						string tmp_str = yes_node->label;

						cetak("+");

						if (tmp_str == "normal") {

							idx_svm++;
							parent_node->idx_svm = idx_svm;
							cetak("{N}");
						} else {
							cetak("{A}");
						}

						parent_node->is_lanjut = yes_node->is_lanjut or no_node->is_lanjut;

						parent_node->criteriaAttrIndex = -1;
						parent_node->label = yes_node->label;
						delete yes_node;
						delete no_node;
						yes_node = NULL;
						no_node = NULL;

						df_below.clear_memory(0);
						df_above.clear_memory(0);
						df.clear_memory(1);

						missing_branch.insert_same_label(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

						cetak("\n");
					}
				}

				parent_node->left = yes_node;
				parent_node->right = no_node;

			}

		} else {

			string tmp_str = create_leaf(df);

			cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				parent_node->idx_svm = idx_svm;
				cetak("{N}");
			} else {
				cetak("{A}");
			}

			parent_node->isLeaf = true;
			parent_node->label = tmp_str;

			//df.clear_memory();
			df.clear_col_split();

			parent_node->is_lanjut = false;

			missing_branch.insert_not_split(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

			cetak("\n");

		}
	}

	return parent_node;
}

tree_node* Tdt_build::train_prev_tree(Tdataframe &df, tb_missing_branch &missing_branch, int counter, tree_node* prev_tree)
{
	tree_node* parent_node = NULL;

	if (prev_tree == NULL)
	{
		parent_node = new tree_node;
	} else
	{
		parent_node = prev_tree;
	}

	cetak(".");
	//cetak("[%d %d]", counter, df.getjmlrow());

	bool is_pure = check_purity(df);
	bool is_min_sample = (df.getjmlrow() < config->min_sample);
	bool is_depth_limit = (counter >= config->depth);

	posisi_cabang tmp_posisi = df.get_posisi();

	int split_column = -1;
	string split_value = "-1";

	if (is_pure or is_min_sample or is_depth_limit)
	{
		string tmp_str = create_leaf(df);
		//cetak("*");

		if (tmp_str == "normal") {
			idx_svm++;
			parent_node->idx_svm = idx_svm;
			//cetak("{N}");

		} else {
			//cetak("{A}");
		}

		parent_node->is_lanjut = !(is_pure or is_min_sample);

		parent_node->isLeaf = true;
		parent_node->label = tmp_str;

		//df.clear_memory();
		df.clear_col_split();

		missing_branch.insert_cut_off(tmp_posisi, tmp_str, (is_pure ? 1 : 0) , (is_min_sample ? 1 : 0) , (is_depth_limit ? 1 : 0), (parent_node->is_lanjut ? 1 : 0));

		//cetak("\n");

	} else {

		// if (!prev_tree->is_pruning)
		// {

		//cetak("?");

		counter++;

		df.stat_tabel(true, true, true);
		parent_node->isLeaf = false;
		bool new_branch_number = true;

		//int split_column_tmp = -1;
		//string split_value_tmp = "-1";


		if (prev_tree != NULL)
		{
			//cetak("*");
			if ( (prev_tree->left != NULL)  and (prev_tree->right != NULL))
			{
				//cetak("+");
				// if (prev_tree->left->attrValue == prev_tree->right->attrValue)
				// {
				//cetak("-");
				split_column = prev_tree->criteriaAttrIndex;
				split_value = prev_tree->left->attrValue;
				//cetak("x");

				new_branch_number = false;
				//}
			}
			//cetak("*");
		}

		//cetak(" [%d,%s] ", split_column, split_value.c_str());

		if (new_branch_number)
		{
			determine_best_split(df, split_column, split_value);
			//cetak(" [%d,%s] [%d,%s] ", split_column_tmp, split_value_tmp.c_str(), split_column, split_value.c_str());
		}

		if (split_value != "-1")
		{

			missing_branch.add_branch(tmp_posisi, split_column, -1, split_value);

			Tdataframe df_below, df_above;

			if (new_branch_number)
			{
				auto itr = branch_number.find(counter);
				if (itr == branch_number.end()) {
					branch_number.insert({counter, 1});
				} else {
					branch_number[counter] = branch_number[counter] + 1;
				}
			}


			df_below = df;
			df_below.switch_parent_branch();

			if (new_branch_number) {
				df_below.set_branch(counter, 1, branch_number[counter]);
				branch_number[counter] = branch_number[counter] + 1;
			} else {
				df_below.set_branch(prev_tree->left->depth, prev_tree->left->branch, prev_tree->left->branch_number);
			}

			df_above = df;
			df_above.switch_parent_branch();

			if (new_branch_number) {
				df_above.set_branch(counter, 2, branch_number[counter]);
			} else {
				df_above.set_branch(prev_tree->right->depth, prev_tree->right->branch, prev_tree->right->branch_number);
			}

			int jml_row = df.getjmlrow();
			float prosen = 1;
			float prosen1 = 0;

			float jml_row_prosen = jml_row;
			float jml_row_prosen1 = 0;

			if (config->threshold < 1)
			{
				prosen = 1 - config->threshold;
				prosen1 = config->threshold;

				jml_row_prosen = ceil(prosen * jml_row);
				jml_row_prosen1 =  ceil(prosen1 * jml_row);
			} else {
				jml_row_prosen = jml_row - config->threshold;
				jml_row_prosen1 =  config->threshold;
			}

			df.split_data(split_column, split_value, df_below, df_above);

			bool is_pass = (df_below.getjmlrow() >= jml_row_prosen1 ) and (df_above.getjmlrow() <= jml_row_prosen);

			if ( ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) or (!is_pass)) {
				string tmp_str = create_leaf(df);

				//cetak("-");

				if (tmp_str == "normal") {
					idx_svm++;
					parent_node->idx_svm = idx_svm;
					//cetak("{N}");
				} else {
					//cetak("{A}");
				}

				parent_node->isLeaf = true;
				parent_node->label = tmp_str;

				df_below.clear_memory(0);
				df_above.clear_memory(0);

				df_below.clear_col_split();
				df_above.clear_col_split();

				df.clear_memory(1);
				df.clear_col_split();

				parent_node->is_lanjut = false;

				missing_branch.insert_not_split(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

				//cetak("\n");

			} else {
				//cetak("|");

				parent_node->criteriaAttrIndex = split_column;

				//cetak("->");
				posisi_cabang tmp_posisi_below = df_below.get_posisi();
				missing_branch.add_branch(tmp_posisi_below, split_column, df.get_opt(split_column, 1), split_value);

				tree_node* yes_node = NULL;

				if (new_branch_number)
				{
					yes_node = train_prev_tree(df_below, missing_branch, counter, NULL);
				}
				else {
					yes_node = train_prev_tree(df_below, missing_branch, counter, prev_tree->left);
				}

				yes_node->attrValue = split_value;
				yes_node->opt = df.get_opt(split_column, 1);

				yes_node->depth = tmp_posisi_below.child_depth;
				yes_node->branch = tmp_posisi_below.child_branch;
				yes_node->branch_number = tmp_posisi_below.child_branch_number;


				if (counter == 1)
				{
					cetak("\n");
				}


				//cetak("<-");
				posisi_cabang tmp_posisi_above = df_above.get_posisi();
				missing_branch.add_branch(tmp_posisi_above, split_column, df.get_opt(split_column, 0), split_value);

				tree_node* no_node = NULL;
				if (new_branch_number)
				{
					no_node = train_prev_tree(df_above, missing_branch, counter, NULL);
				} else {
					no_node = train_prev_tree(df_above, missing_branch, counter, prev_tree->right);
				}

				no_node->attrValue = split_value;
				no_node->opt = df.get_opt(split_column, 0);

				no_node->depth = tmp_posisi_above.child_depth;
				no_node->branch = tmp_posisi_above.child_branch;
				no_node->branch_number = tmp_posisi_above.child_branch_number;

				if (config->prunning) {

					if (((yes_node->isLeaf == true) and (no_node->isLeaf == true)) and (yes_node->label == no_node->label))
					{

						parent_node->isLeaf = true;

						string tmp_str = yes_node->label;

						//cetak("+");

						if (tmp_str == "normal") {

							idx_svm++;
							parent_node->idx_svm = idx_svm;
							//cetak("{N}");
						} else {
							//cetak("{A}");
						}

						parent_node->is_lanjut = yes_node->is_lanjut or no_node->is_lanjut;

						parent_node->criteriaAttrIndex = -1;
						parent_node->label = yes_node->label;
						delete yes_node;
						delete no_node;
						yes_node = NULL;
						no_node = NULL;

						df_below.clear_memory(0);
						df_above.clear_memory(0);
						df.clear_memory(1);

						missing_branch.insert_same_label(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

						//cetak("\n");
					}
				}

				parent_node->left = yes_node;
				parent_node->right = no_node;

			}


		} else {

			string tmp_str = create_leaf(df);

			//cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				parent_node->idx_svm = idx_svm;
				//cetak("{N}");
			} else {
				//cetak("{A}");
			}

			parent_node->isLeaf = true;
			parent_node->label = tmp_str;

			//df.clear_memory();
			df.clear_col_split();

			parent_node->is_lanjut = false;

			missing_branch.insert_not_split(tmp_posisi, tmp_str, (parent_node->is_lanjut ? 1 : 0));

			//cetak("\n");

		}

		// } else {
		//     string tmp_str = create_leaf(df);

		// 	parent_node->isLeaf = true;
		// 	parent_node->label = tmp_str;

		// 	del_dec_tree(parent_node->left);
		// 	del_dec_tree(parent_node->right);

		// 	parent_node->left = NULL;
		// 	parent_node->right = NULL;
		// }
	}

	return parent_node;
}



void Tdt_build::pruning_dfs(tree_node* parent_node, Tdataframe & df_train, tb_missing_branch &missing_branch)
{
	cetak(".");

	Tdataframe df_left;
	df_left = df_train;
	df_left.switch_parent_branch();

	Tdataframe df_right;
	df_right = df_train;
	df_right.switch_parent_branch();

	if (parent_node->left != NULL) {
		df_left.set_branch(parent_node->left->depth, parent_node->left->branch, parent_node->left->branch_number);
		df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, false, false);
		if (!parent_node->left->isLeaf) {
			pruning_dfs(parent_node->left, df_left, missing_branch);
			//df_left.clear_memory();
		}
	}

	if (parent_node->right != NULL) {
		df_right.set_branch(parent_node->right->depth, parent_node->right->branch, parent_node->right->branch_number);
		df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, false, false);
		if (!parent_node->right->isLeaf) {
			pruning_dfs(parent_node->right, df_right, missing_branch);
			//df_right.clear_memory();
		}
	}


	if ((parent_node->left != NULL) and (parent_node->right != NULL) ) {
		if ((parent_node->left->isLeaf) and (parent_node->right->isLeaf))
		{
			cetak("+");

			df_train.ReFilter(false);

			float error_node, error_left, error_right, sum_error;

			string left_label = parent_node->left->label;
			string right_label = parent_node->right->label;

			error_node = df_train.get_estimate_error();
			string node_label = df_train.get_max_label();

			df_left.ReFilter(false);
			error_left = df_left.get_estimate_error();

			df_right.ReFilter(false);
			error_right = df_right.get_estimate_error();


			sum_error = (((float) df_left.getjmlrow() / df_train.getjmlrow()) * error_left) + (((float) df_right.getjmlrow() / df_train.getjmlrow()) * error_right);

			// cetak("[AttrIndex : %d train jml row : %d left jml row : %d right jml row : %d  error_node : %f sum_error : %f]\n",parent_node->criteriaAttrIndex,df_train.getjmlrow(),df_left.getjmlrow(),df_right.getjmlrow(),error_node,sum_error);

			if (error_node < sum_error)
			{
				cetak("*");

				parent_node->is_lanjut = parent_node->left->is_lanjut or parent_node->right->is_lanjut;

				posisi_cabang tmp_posisi = df_train.get_posisi();
				missing_branch.insert_pruning(tmp_posisi, node_label, (parent_node->is_lanjut ? 1 : 0 ));

				delete parent_node->left;
				delete parent_node->right;

				parent_node->left = NULL;
				parent_node->right = NULL;

				parent_node->criteriaAttrIndex = -1;
				parent_node->isLeaf = true;
				parent_node->label = node_label;

				if (node_label == "normal")
				{
					idx_svm++;
					parent_node->idx_svm = idx_svm;
				}

				df_left.clear_memory(0);
				df_right.clear_memory(0);
				df_train.clear_memory(1);

			}

		}

	}

}


void Tdt_build::post_pruning(Tdataframe & df_train, tb_missing_branch &missing_branch)
{
	pruning_dfs(dec_tree, df_train, missing_branch);
	//pruning_dfs(0, df_train, 0);
}

void Tdt_build::dec_tree_to_vec_tree(tree_node* parent_node, int node_index)
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
			if (idx_svm != parent_node->idx_svm)
			{
				tree[node_index].idx_svm = idx_svm;
			}

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

	tb_tree dbtree;

	string tmp_str = "";
	vector<string> vec;

	string head = to_string(config->id_detail_experiment) + "," + to_string(config->id_experiment) + "," ;


	for (size_t i = 0; i < tree.size(); ++i)
	{
		tmp_str = to_string(tree[i].criteriaAttrIndex) + ",'" + tree[i].attrValue + "','" + tree[i].label + "'," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;

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
		vec.push_back(head + tmp_str);
	}


	dbtree.write_tree(vec);
	dbtree.close_file();
}

tree_node* Tdt_build::build_missing_branch(int counter, posisi_cabang posisi, tb_missing_branch &missing_branch)
{
	tree_node* parent_node = new tree_node;

	//cetak("%d ", counter);

	missing_branch.get_stat(posisi);

	parent_node->is_pure = missing_branch.get_is_pure();
	parent_node->is_min_sample = missing_branch.get_is_min_sample();
	parent_node->is_depth_limit = missing_branch.get_is_depth_limit();
	parent_node->is_same_label = missing_branch.get_is_same_label();
	parent_node->is_pruning = missing_branch.get_is_pruning();
	parent_node->is_not_split = missing_branch.get_is_not_split();
	parent_node->is_lanjut = missing_branch.get_is_lanjut();
	parent_node->label = missing_branch.get_label();

	if (parent_node->is_pure or parent_node->is_min_sample or parent_node->is_depth_limit or parent_node->is_not_split)
	{
		parent_node->isLeaf = true;

		//cetak("\n");

	} else {

		if ( parent_node->is_same_label or parent_node->is_pruning)
		{
			parent_node->isLeaf = true;

			//cetak("?|");
			counter++;

			posisi_cabang posisi_tmp = posisi;
			posisi_tmp.switch_parent_branch();
			posisi_tmp.set_child(counter, -1, -1);

			if (missing_branch.parent_exixst(posisi_tmp)) {

				tree_node* left_node = NULL;
				tree_node* right_node = NULL;

				int attrindex = -1;
				string attrValue = "-1";
				int opt = -1;

				missing_branch.get_split(posisi, attrindex, opt, attrValue);
				parent_node->criteriaAttrIndex = attrindex;

				int max_branch_number = 0;

				auto itr = branch_number.find(counter);
				if (itr != branch_number.end()) {
					max_branch_number = branch_number[counter];
				}

				int tmp_max_branch_number = max_branch_number + 1;

				posisi_cabang posisi_left = posisi;
				posisi_left.switch_parent_branch();
				posisi_left.set_child(counter, 1, tmp_max_branch_number);

				if (missing_branch.parent_exixst(posisi_left)) {
					int attrindex_left = -1;
					string attrValue_left = "-1";
					int opt_left = -1;

					missing_branch.get_split(posisi_left, attrindex_left, opt_left, attrValue_left);

					//cetak("->");
					left_node = build_missing_branch(counter, posisi_left, missing_branch);
					if (left_node != NULL) {
						left_node->attrValue = attrValue;
						left_node->opt = opt_left;

						left_node->depth = counter;
						left_node->branch = 1;
						left_node->branch_number = tmp_max_branch_number;
					} else {
						tmp_max_branch_number -= 1;
					}
				} else {
					//cetak(" Masalah 1 %d %d %d %d %d %d ", posisi_left.child_depth, posisi_left.child_branch, posisi_left.child_branch_number, posisi_left.parent_depth, posisi_left.parent_branch, posisi_left.parent_branch_number);
					tmp_max_branch_number -= 1;
				}

				tmp_max_branch_number += 1;

				posisi_cabang posisi_right = posisi;
				posisi_right.switch_parent_branch();
				posisi_right.set_child(counter, 2, tmp_max_branch_number);

				if (missing_branch.parent_exixst(posisi_right)) {

					int attrindex_right = -1;
					string attrValue_right = "-1";
					int opt_right = -1;

					missing_branch.get_split(posisi_right, attrindex_right, opt_right, attrValue_right);

					//cetak("<-");
					right_node = build_missing_branch(counter, posisi_right, missing_branch);
					if (right_node != NULL) {
						right_node->attrValue = attrValue;
						right_node->opt = opt_right;

						right_node->depth = counter;
						right_node->branch = 2;
						right_node->branch_number = tmp_max_branch_number;
					} else {
						tmp_max_branch_number -= 1;
					}
				} else {
					//cetak(" Masalah 2 %d %d %d %d %d %d", posisi_right.child_depth, posisi_right.child_branch, posisi_right.child_branch_number, posisi_right.parent_depth, posisi_right.parent_branch, posisi_right.parent_branch_number);
					tmp_max_branch_number -= 1;
				}

				parent_node->right = right_node;
				parent_node->left = left_node;

				if (tmp_max_branch_number > max_branch_number) {
					auto itr = branch_number.find(counter);
					if (itr == branch_number.end()) {
						branch_number.insert({counter, tmp_max_branch_number});
					} else {
						branch_number[counter] = tmp_max_branch_number;
					}

					parent_node->isLeaf = false;
				}


			} else {
				parent_node = NULL;
			}

		} else {
			parent_node = NULL;
		}
	}


	return parent_node;
}


void Tdt_build::trim_dec_tree(tree_node* parent_node)
{
	//cetak("%d", parent_node->depth);

	if (parent_node->left != NULL)
	{
		if (!parent_node->left->isLeaf) {
			//cetak("->");
			trim_dec_tree(parent_node->left);
		}
	}

	if (parent_node->right != NULL)
	{
		if (!parent_node->right->isLeaf) {
			//cetak("<-");
			trim_dec_tree(parent_node->right);
		}
	}

	if ((parent_node->left != NULL) and (parent_node->right != NULL))
	{
		if (parent_node->left->isLeaf and parent_node->right->isLeaf)
		{
			if (!parent_node->is_lanjut)
			{
				//cetak(" %d tdk lanjut !!!", parent_node->depth);

				parent_node->isLeaf = true;

				delete parent_node->left;
				delete parent_node->right;

				parent_node->left = NULL;
				parent_node->right = NULL;

				//cetak("\n");
			}
		}
		// else {
		// 	if (!parent_node->is_lanjut)
		// 	{
		// 		parent_node->is_lanjut = parent_node->left->is_lanjut or parent_node->right->is_lanjut;
		// 	}
		// }
	}

}

tree_node* Tdt_build::vec_tree_to_dec_tree(int node_index, int counter, posisi_cabang posisi, tb_missing_branch &missing_branch)
{
	//cetak("%d ", counter);

	tree_node* parent_node = NULL;

	if (prev_tree[node_index].isLeaf)
	{
		if (missing_branch.parent_exixst(posisi)) {
			parent_node = build_missing_branch(counter, posisi, missing_branch);
		}

		if (parent_node == NULL)
		{
			parent_node = new tree_node;
		}

		parent_node->label = prev_tree[node_index].label;
		parent_node->isLeaf = true;

		if ((parent_node->left != NULL) and (parent_node->right != NULL))
		{
			parent_node->isLeaf = false;
		}
		//cetak("\n");
	} else {

		parent_node = new tree_node;

		//cetak("?|");

		counter++;

		auto itr = branch_number.find(counter);
		if (itr == branch_number.end()) {
			branch_number.insert({counter, 1});
		} else {
			branch_number[counter] = branch_number[counter] + 1;
		}

		parent_node->criteriaAttrIndex = prev_tree[node_index].criteriaAttrIndex;

		int left = prev_tree[node_index].children[0];
		int right = prev_tree[node_index].children[1];

		posisi_cabang posisi_left = posisi;
		posisi_left.switch_parent_branch();
		posisi_left.set_child(counter, 1, branch_number[counter]);


		if (left != -1) {
			//cetak("->");
			tree_node* left_node = vec_tree_to_dec_tree(left, counter, posisi_left, missing_branch);
			left_node->attrValue = prev_tree[left].attrValue;
			left_node->opt = prev_tree[left].opt;

			left_node->depth = counter;
			left_node->branch = 1;
			left_node->branch_number = branch_number[counter];

			parent_node->left = left_node;
		}

		if (counter == 1)
		{
			//cetak("\n");
		}

		posisi_cabang posisi_right = posisi;
		posisi_right.switch_parent_branch();
		branch_number[counter] = branch_number[counter] + 1;
		posisi_right.set_child(counter, 2, branch_number[counter]);

		if (right != -1) {
			//cetak("<-");
			tree_node* right_node = vec_tree_to_dec_tree(right, counter, posisi_right, missing_branch);
			right_node->attrValue = prev_tree[right].attrValue;
			right_node->opt = prev_tree[right].opt;

			right_node->depth = counter;
			right_node->branch = 2;
			right_node->branch_number = branch_number[counter];

			parent_node->right = right_node;
		}
	}

	return parent_node;
}

void Tdt_build::read_tree(time_t id_detail_experiment, tb_missing_branch &missing_branch)
{
	idx_svm = 0;
	vector<string> tmp_data;
	tb_tree tree;
	tree.baca_tree(id_detail_experiment);

	tree.reset_file();
	while (!tree.is_eof()) {

		tmp_data = tree.get_record();

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

		idx_svm = (tmp_data[8] == "-1") ? idx_svm : idx_svm+1;

		newnode.idx_svm = (tmp_data[8] == "-1") ? -1 : stoi(tmp_data[8]);

		if (idx_svm != newnode.idx_svm)
		{
			newnode.idx_svm = idx_svm;
		}

		// if (idx_svm < newnode.idx_svm)
		// {
		//  	idx_svm = newnode.idx_svm;
		// }

		prev_tree.push_back(newnode);

		tree.next_record();
	}

	tree.close_file();

	posisi_cabang posisi_root;
	posisi_root.reset();

	dec_tree = vec_tree_to_dec_tree(0, 0, posisi_root, missing_branch);
	trim_dec_tree(dec_tree);

	prev_tree.clear();
}

void Tdt_build::build_from_prev_tree(Tdataframe &df_train, tb_missing_branch &missing_branch, int prev_tree_depth, bool bypass)
{
	//branch_number.clear();

	// Tdataframe df_train(config);
	// df_train.set_dataset(config->id_dt_train, config->jns_dt_train, config->partition_train);
	// df_train.read_header_type();
	// df_train.set_parent(0, 0, 0);
	// df_train.set_branch(0, 0, 0);
	// df_train.clone_dataset();
	// df_train.stat_tabel(true, false, true);
	// df_train.setjmltotalrow();

	this->prev_tree_depth = prev_tree_depth;

	if (!bypass) {
		train(df_train, missing_branch, dec_tree);
		//train(df_train, 0, 0, 0);
		cetak("\n");
		//branch_number.clear();
		df_train.set_parent(0, 0, 0);
		df_train.set_branch(0, 0, 0);
		//df_train.reset_depth_branch();

		if (config->prunning) {
			//cetak("Start Prunning Decission Tree : \n");
			post_pruning(df_train, missing_branch);
			//cetak("\nEnd Prunning Decission Tree : \n");
		}
		cetak("\n");
	}

	missing_branch.delete_non_missing();
	save_tree();
	//df_train.close_file();

}

void Tdt_build::build_tree(Tdataframe &df_train, tb_missing_branch &missing_branch)
{
	branch_number.clear();

	// Tdataframe df_train(config);
	// df_train.set_dataset(config->id_dt_train, config->jns_dt_train, config->partition_train);
	// df_train.read_header_type();
	// df_train.set_parent(0, 0, 0);
	// df_train.set_branch(0, 0, 0);
	// df_train.clone_dataset();
	// df_train.stat_tabel(true, true, true);
	// df_train.info();
	// df_train.setjmltotalrow();

	{
		cetak("Start Train Decission Tree : \n");
		//train(df_train, 0, 0);
		dec_tree = train(df_train, missing_branch, 0);
	}

	cetak("\n");

	//branch_number.clear();
	df_train.set_parent(0, 0, 0);
	df_train.set_branch(0, 0, 0);
	//df_train.reset_depth_branch();


	if (config->prunning) {
		cetak("Start Prunning Decission Tree : \n");
		post_pruning(df_train, missing_branch);
		//cetak("\nEnd Prunning Decission Tree : \n");
	}

	cetak("\n");

	missing_branch.delete_non_missing();
	save_tree();
	//df_train.close_file();
}


