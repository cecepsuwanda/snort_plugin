#include "Tdt_build.h"


Tdt_build::Tdt_build()
{
	idx_svm = 0;
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


Tmetric_split_value Tdt_build::get_split_value(Tdataframe &df, int idx)
{
	Tmetric_split_value hsl;
	Tmy_dttype current_split_value;
	float current_overall_metric = 0.0;

	df.calculate_overall_metric(idx, current_overall_metric, current_split_value);

	hsl.idx = idx;
	hsl.overall_metric = current_overall_metric;
	hsl.split_value = current_split_value;
	return hsl;
}


void Tdt_build::determine_best_split(Tdataframe &df, int &split_column, Tmy_dttype &split_value)
{

	df.search_col_split();

	float max_gain = 0;
	bool  first_iteration = true;

	string current_split_value = "-1";
	//float current_overall_metric = -1;

	split_column = -1;
	split_value.set_value("-1", true);

	vector<future<Tmetric_split_value>> async_worker;

	for (int i = 0; i < df.get_jml_valid_attr(); ++i)
	{
		async_worker.push_back(async(std::launch::async, &Tdt_build::get_split_value, ref(df), df.get_valid_attr(i)));
	}

	if (async_worker.size() > 0)
	{
		//pesan.cetak("\n");
		Tmetric_split_value hsl;
		for (future<Tmetric_split_value> & th : async_worker)
		{
			hsl = th.get();

			//pesan.cetak("[%d,%f]\n", hsl.idx, hsl.overall_metric);

			if (first_iteration or (max_gain < hsl.overall_metric))
			{
				max_gain = hsl.overall_metric;
				split_column = hsl.idx;
				split_value = hsl.split_value;

				first_iteration = false;

				//pesan.cetak("1  [%d,%f]\n", split_column, max_gain);

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


void Tdt_build::train(Tdataframe &df, tree_node* parent_node)
{
	//pesan.cetak(".");
	pesan.cetak("%d", parent_node->depth);

	if (parent_node->isLeaf)
	{
		df.ReFilter(false);
		if (parent_node->is_lanjut) {

			tree_node* tmp_node = train_prev_tree(df, parent_node->depth, parent_node);
			parent_node = tmp_node;

		} else {

			string tmp_str = parent_node->label;
			pesan.cetak("*");

			if (tmp_str == "normal") {
				idx_svm++;
				parent_node->idx_svm = idx_svm;
				pesan.cetak("{N}\n");
			} else {
				pesan.cetak("{A}\n");
			}
		}

	} else {
		pesan.cetak("?|");

		Tposisi_cabang tmp_posisi = df.get_posisi();

		if (tmp_posisi.get_id_branch() = "000000")
		{
			bool get_stat = (parent_node->jml_known == 0) or (parent_node->jml_normal == 0);
			if (get_stat) {
				df.ReFilter(false);
				parent_node->label = df.get_max_label();
				parent_node->jml_known = df.get_jml_stat("known");
				parent_node->jml_normal = df.get_jml_stat("normal");
			}
		}

		Tdataframe df_below, df_above;

		if (parent_node->left != NULL) {
			pesan.cetak("->");

			df_below = df;
			df_below.switch_parent_branch();
			df_below.set_branch(parent_node->left->depth, parent_node->left->branch, parent_node->left->branch_number);

			bool get_stat = (parent_node->left->jml_known == 0) or (parent_node->left->jml_normal == 0);
			df_below.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, get_stat, get_stat);

			if (get_stat)
			{
				parent_node->left->label = df_below.get_max_label();
				parent_node->left->jml_known = df_below.get_jml_stat("known");
				parent_node->left->jml_normal = df_below.get_jml_stat("normal");
			}

			train(df_below, parent_node->left);
		}

		if (parent_node->right != NULL) {
			pesan.cetak("<-");

			df_above = df;
			df_above.switch_parent_branch();
			df_above.set_branch(parent_node->right->depth, parent_node->right->branch, parent_node->right->branch_number);
			bool get_stat = (parent_node->right->jml_known == 0) or (parent_node->right->jml_normal == 0);
			df_above.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, get_stat, get_stat);

			if (get_stat)
			{
				parent_node->right->label = df_above.get_max_label();
				parent_node->right->jml_known = df_above.get_jml_stat("known");
				parent_node->right->jml_normal = df_above.get_jml_stat("normal");
			}

			train(df_above, parent_node->right);
		}



		if ((parent_node->left != NULL) and (parent_node->right != NULL))
		{
			if (parent_node->left->isLeaf and parent_node->right->isLeaf)
			{
				if (parent_node->left->label == parent_node->right->label)
				{
					parent_node->isLeaf = true;

					string tmp_str = parent_node->left->label;

					pesan.cetak("+");

					if (tmp_str == "normal") {
						idx_svm++;
						parent_node->idx_svm = idx_svm;
						pesan.cetak("{N}");
					} else {
						pesan.cetak("{A}");
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

					missing_branch.add_cut_off(tmp_posisi, 4);
				}
			}
		}
	}

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

tree_node* Tdt_build::train(Tdataframe &df, int counter)
{
	tree_node* parent_node = new tree_node;

	//cetak(".");
	pesan.cetak("[%d %d]", counter, df.getjmlrow());

	bool is_pure = check_purity(df);
	bool is_min_sample = (df.getjmlrow() < global_config.min_sample);
	bool is_depth_limit = (counter == (global_config.depth - 1));

	Tposisi_cabang tmp_posisi = df.get_posisi();
	int split_column = -1;
	Tmy_dttype split_value;

	if (is_pure or is_min_sample or is_depth_limit)
	{
		string tmp_str = create_leaf(df);
		pesan.cetak("*");

		if (tmp_str == "normal") {
			idx_svm++;
			parent_node->idx_svm = idx_svm;
			pesan.cetak("{N}");

		} else {
			pesan.cetak("{A}");
		}

		parent_node->isLeaf = true;
		parent_node->label = tmp_str;


		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");

		//df.clear_memory();
		df.clear_col_split();

		parent_node->is_lanjut = !(is_pure or is_min_sample);

		if (is_pure) {
			missing_branch.add_cut_off(tmp_posisi, 11);
		}

		if (is_min_sample) {
			missing_branch.add_cut_off(tmp_posisi, 12);
		}

		if (is_depth_limit) {
			missing_branch.add_cut_off(tmp_posisi, 13);
		}


		pesan.cetak("\n");

	} else {

		pesan.cetak("?");

		counter++;

		df.stat_tabel(true, true, true);
		determine_best_split(df, split_column, split_value);

		parent_node->label = df.get_max_label();
		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");

		pesan.cetak(" [%d,%s] ", split_column, split_value.get_string().c_str());


		if (split_value != "-1")
		{

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

			df.split_data(split_column, split_value, df_below, df_above);

			bool is_pass = limit_jml_dt_cabang(df.getjmlrow(), df_below.getjmlrow(), df_above.getjmlrow());

			if ( ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) or (!is_pass)) { //or (!is_pass)
				string tmp_str = create_leaf(df);

				pesan.cetak("-");

				if (tmp_str == "normal") {
					idx_svm++;
					parent_node->idx_svm = idx_svm;
					pesan.cetak("{N}");
				} else {
					pesan.cetak("{A}");
				}

				Tposisi_cabang tmp_posisi_above = df_above.get_posisi();
				Tposisi_cabang tmp_posisi_below = df_below.get_posisi();

				parent_node->isLeaf = true;
				parent_node->label = tmp_str;

				df_below.clear_memory(0);
				df_above.clear_memory(0);

				df_below.clear_col_split();
				df_above.clear_col_split();

				df.clear_memory(1);
				df.clear_col_split();

				parent_node->is_lanjut = false;

				missing_branch.add_cut_off(tmp_posisi, 3);
				missing_branch.add_cut_off(tmp_posisi_below, 3);
				missing_branch.add_cut_off(tmp_posisi_above, 3);

				pesan.cetak("\n");

			} else {
				pesan.cetak("|");

				parent_node->criteriaAttrIndex = split_column;

				pesan.cetak("->");
				Tposisi_cabang tmp_posisi_below = df_below.get_posisi();

				tree_node* yes_node = train(df_below, counter);
				yes_node->attrValue = split_value;
				yes_node->opt = df.get_opt(split_column, 1);

				yes_node->depth = tmp_posisi_below.child_depth;
				yes_node->branch = tmp_posisi_below.child_branch;
				yes_node->branch_number = tmp_posisi_below.child_branch_number;


				if (counter == 1)
				{
					pesan.cetak("\n");
				}


				pesan.cetak("<-");
				Tposisi_cabang tmp_posisi_above = df_above.get_posisi();

				tree_node* no_node = train(df_above, counter);
				no_node->attrValue = split_value;
				no_node->opt = df.get_opt(split_column, 0);

				no_node->depth = tmp_posisi_above.child_depth;
				no_node->branch = tmp_posisi_above.child_branch;
				no_node->branch_number = tmp_posisi_above.child_branch_number;

				if (global_config.prunning) {

					if (((yes_node->isLeaf == true) and (no_node->isLeaf == true)) and (yes_node->label == no_node->label))
					{

						parent_node->isLeaf = true;

						string tmp_str = yes_node->label;

						pesan.cetak("+");

						if (tmp_str == "normal") {

							idx_svm++;
							parent_node->idx_svm = idx_svm;
							pesan.cetak("{N}");
						} else {
							pesan.cetak("{A}");
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


						missing_branch.add_cut_off(tmp_posisi, 4);

						pesan.cetak("\n");
					}
				}

				parent_node->left = yes_node;
				parent_node->right = no_node;

			}

		} else {

			string tmp_str = create_leaf(df);

			pesan.cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				parent_node->idx_svm = idx_svm;
				pesan.cetak("{N}");
			} else {
				pesan.cetak("{A}");
			}

			parent_node->isLeaf = true;
			parent_node->label = tmp_str;

			parent_node->jml_known = df.get_jml_stat("known");
			parent_node->jml_normal = df.get_jml_stat("normal");

			//df.clear_memory();
			df.clear_col_split();

			parent_node->is_lanjut = false;


			missing_branch.add_cut_off(tmp_posisi, 2);

			pesan.cetak("\n");

		}
	}

	return parent_node;
}

tree_node* Tdt_build::train_prev_tree(Tdataframe &df, int counter, tree_node* prev_tree)
{
	tree_node* parent_node = NULL;

	bool new_branch_number = false;

	if (prev_tree == NULL)
	{
		parent_node = new tree_node;
	} else
	{
		parent_node = prev_tree;
		new_branch_number = true;
	}

	//pesan.cetak(".");
	pesan.cetak("[%d %d]", counter, df.getjmlrow());

	bool is_pure = check_purity(df);
	bool is_min_sample = (df.getjmlrow() < global_config.min_sample);
	bool is_depth_limit = (counter == (global_config.depth - 1));

	Tposisi_cabang tmp_posisi = df.get_posisi();

	if (new_branch_number)
	{
		new_branch_number = !((tmp_posisi.child_depth == parent_node->depth) and (tmp_posisi.child_branch == parent_node->branch) and (tmp_posisi.child_branch_number == parent_node->branch_number));
	}


	int split_column = -1;
	Tmy_dttype split_value;

	if (is_pure or is_min_sample or is_depth_limit)
	{
		string tmp_str = create_leaf(df);
		pesan.cetak("*");

		if (tmp_str == "normal") {
			idx_svm++;
			parent_node->idx_svm = idx_svm;
			pesan.cetak("{N}");

		} else {
			pesan.cetak("{A}");
		}

		parent_node->is_lanjut = !(is_pure or is_min_sample);

		parent_node->isLeaf = true;
		parent_node->label = tmp_str;

		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");

		//df.clear_memory();
		df.clear_col_split();



		if (is_pure) {
			missing_branch.add_cut_off(tmp_posisi, 11);
		}

		if (is_min_sample) {
			missing_branch.add_cut_off(tmp_posisi, 12);
		}

		if (is_depth_limit) {
			missing_branch.add_cut_off(tmp_posisi, 13);
		}


		pesan.cetak("\n");

	} else {

		// if (!prev_tree->is_pruning)
		// {

		pesan.cetak("?");

		counter++;

		df.stat_tabel(true, true, true);
		parent_node->isLeaf = false;

		//int split_column_tmp = -1;
		//string split_value_tmp = "-1";

		parent_node->jml_known = df.get_jml_stat("known");
		parent_node->jml_normal = df.get_jml_stat("normal");


		if (!new_branch_number)
		{
			//pesan.cetak("*");
			split_column = prev_tree->criteriaAttrIndex;
			if ( (prev_tree->left != NULL))
			{
				split_value = prev_tree->left->attrValue;
			} else {
				if ((prev_tree->right != NULL))
				{
					split_value = prev_tree->right->attrValue;
				} else {
					if (parent_node->is_not_split)
					{
						pesan.cetak(" no-split ");
						//missing_branch.delete_split(tmp_posisi);
					}
					missing_branch.delete_histori(tmp_posisi);
					new_branch_number = true;
				}
			}

		}

		if (new_branch_number)
		{
			determine_best_split(df, split_column, split_value);
		}

		pesan.cetak(" [%s %d,%s] ", tmp_posisi.get_id_branch().c_str(), split_column, split_value.get_string().c_str());

		if (split_value != "-1")
		{

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


			df.split_data(split_column, split_value, df_below, df_above);

			bool is_pass = limit_jml_dt_cabang(df.getjmlrow(), df_below.getjmlrow(), df_above.getjmlrow());

			if ( ((df_below.getjmlrow() == 0) or (df_above.getjmlrow() == 0)) or (!is_pass) ) { //or (!is_pass)
				string tmp_str = create_leaf(df);

				pesan.cetak("-");

				if (tmp_str == "normal") {
					idx_svm++;
					parent_node->idx_svm = idx_svm;
					pesan.cetak("{%s N}", tmp_posisi.get_id_branch().c_str());
				} else {
					pesan.cetak("%s {A}", tmp_posisi.get_id_branch().c_str());
				}

				Tposisi_cabang tmp_posisi_below = df_below.get_posisi();
				Tposisi_cabang tmp_posisi_above = df_above.get_posisi();

				parent_node->isLeaf = true;
				parent_node->label = tmp_str;

				df_below.clear_memory(0);
				df_above.clear_memory(0);

				df_below.clear_col_split();
				df_above.clear_col_split();

				df.clear_memory(1);
				df.clear_col_split();

				parent_node->is_lanjut = false;

				missing_branch.add_cut_off(tmp_posisi, 3);
				missing_branch.add_cut_off(tmp_posisi_below, 3);
				missing_branch.add_cut_off(tmp_posisi_above, 3);

				pesan.cetak("\n");

			} else {
				pesan.cetak("|");

				parent_node->criteriaAttrIndex = split_column;
				parent_node->isLeaf = false;

				pesan.cetak("->");
				Tposisi_cabang tmp_posisi_below = df_below.get_posisi();


				tree_node* yes_node = NULL;

				if (new_branch_number)
				{
					yes_node = train_prev_tree(df_below, counter, NULL);
				}
				else {
					yes_node = train_prev_tree(df_below, counter, prev_tree->left);
				}

				yes_node->attrValue = split_value;
				yes_node->opt = df.get_opt(split_column, 1);

				yes_node->depth = tmp_posisi_below.child_depth;
				yes_node->branch = tmp_posisi_below.child_branch;
				yes_node->branch_number = tmp_posisi_below.child_branch_number;


				if (counter == 1)
				{
					pesan.cetak("\n");
				}


				//cetak("<-");
				Tposisi_cabang tmp_posisi_above = df_above.get_posisi();

				tree_node* no_node = NULL;
				if (new_branch_number)
				{
					no_node = train_prev_tree(df_above, counter, NULL);
				} else {
					no_node = train_prev_tree(df_above, counter, prev_tree->right);
				}

				no_node->attrValue = split_value;
				no_node->opt = df.get_opt(split_column, 0);

				no_node->depth = tmp_posisi_above.child_depth;
				no_node->branch = tmp_posisi_above.child_branch;
				no_node->branch_number = tmp_posisi_above.child_branch_number;

				if (global_config.prunning) {

					if (((yes_node->isLeaf == true) and (no_node->isLeaf == true)) and (yes_node->label == no_node->label))
					{

						parent_node->isLeaf = true;

						string tmp_str = yes_node->label;

						pesan.cetak("+");

						if (tmp_str == "normal") {

							idx_svm++;
							parent_node->idx_svm = idx_svm;
							pesan.cetak("{N}");
						} else {
							pesan.cetak("{A}");
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


						missing_branch.add_cut_off(tmp_posisi, 4);

						pesan.cetak("\n");
					}
				}

				parent_node->left = yes_node;
				parent_node->right = no_node;

			}


		} else {

			string tmp_str = create_leaf(df);

			pesan.cetak("-");

			if (tmp_str == "normal") {

				idx_svm++;
				parent_node->idx_svm = idx_svm;
				pesan.cetak("{N}");
			} else {
				pesan.cetak("{A}");
			}

			parent_node->isLeaf = true;
			parent_node->label = tmp_str;

			parent_node->jml_known = df.get_jml_stat("known");
			parent_node->jml_normal = df.get_jml_stat("normal");

			//df.clear_memory();
			df.clear_col_split();

			parent_node->is_lanjut = false;

			missing_branch.add_cut_off(tmp_posisi, 2);

			pesan.cetak("\n");

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



void Tdt_build::pruning_dfs(tree_node* parent_node, Tdataframe & df_train)
{
	//pesan.cetak(".");

	pesan.cetak("[%d %s %d %d]", parent_node->depth, parent_node->label.c_str(), parent_node->jml_known, parent_node->jml_normal);

	Tlabel_stat root_stat, left_stat, right_stat;
	float error_node = 0.0, error_left = 0.0, error_right = 0.0, sum_error = 0.0;

	Tdataframe df_left;
	df_left = df_train;
	df_left.switch_parent_branch();

	Tdataframe df_right;
	df_right = df_train;
	df_right.switch_parent_branch();

	if ((parent_node->left != NULL) and (parent_node->right != NULL) ) {
		df_left.set_branch(parent_node->left->depth, parent_node->left->branch, parent_node->left->branch_number);
		df_right.set_branch(parent_node->right->depth, parent_node->right->branch, parent_node->right->branch_number);

		root_stat.add("known", parent_node->jml_known);
		root_stat.add("normal", parent_node->jml_normal);

		left_stat.add("known", parent_node->left->jml_known);
		left_stat.add("normal", parent_node->left->jml_normal);

		right_stat.add("known", parent_node->right->jml_known);
		right_stat.add("normal", parent_node->right->jml_normal);

		error_node = root_stat.get_estimate_error();
		error_left = left_stat.get_estimate_error();
		error_right = right_stat.get_estimate_error();

		sum_error = (((float) left_stat.get_jml_row() / root_stat.get_jml_row()) * error_left) + (((float) right_stat.get_jml_row() / root_stat.get_jml_row()) * error_right);
	}
	if (error_node < sum_error) {
		pesan.cetak("*");

		parent_node->is_lanjut = parent_node->left->is_lanjut or parent_node->right->is_lanjut;

		Tposisi_cabang tmp_posisi = df_train.get_posisi();
		missing_branch.add_cut_off(tmp_posisi, 5);

		parent_node->isLeaf = true;

		if (parent_node->label == "normal")
		{
			idx_svm++;
			parent_node->idx_svm = idx_svm;
		}

		del_dec_tree(parent_node->left);
		del_dec_tree(parent_node->right);

	} else {
		if (parent_node->left != NULL) {
			pesan.cetak("->");
			pruning_dfs(parent_node->left, df_left);
		}

		if (parent_node->depth == 1)
		{
			pesan.cetak("\n");
		}

		if (parent_node->right != NULL) {
			pesan.cetak("<-");
			pruning_dfs(parent_node->right, df_right);
		}
	}







	// Tdataframe df_left;
	// df_left = df_train;
	// df_left.switch_parent_branch();

	// Tdataframe df_right;
	// df_right = df_train;
	// df_right.switch_parent_branch();

	// if (parent_node->left != NULL) {
	// 	//pesan.cetak("->");
	// 	df_left.set_branch(parent_node->left->depth, parent_node->left->branch, parent_node->left->branch_number);
	// 	df_left.add_filter(parent_node->criteriaAttrIndex, parent_node->left->opt, parent_node->left->attrValue, false, false);
	// 	if (!parent_node->left->isLeaf) {
	// 		pruning_dfs(parent_node->left, df_left);
	// 		//df_left.clear_memory();
	// 	}
	// }



	// if (parent_node->right != NULL) {
	// 	//pesan.cetak("<-");
	// 	df_right.set_branch(parent_node->right->depth, parent_node->right->branch, parent_node->right->branch_number);
	// 	df_right.add_filter(parent_node->criteriaAttrIndex, parent_node->right->opt, parent_node->right->attrValue, false, false);
	// 	if (!parent_node->right->isLeaf) {
	// 		pruning_dfs(parent_node->right, df_right);
	// 		//df_right.clear_memory();
	// 	}
	// }


	// if ((parent_node->left != NULL) and (parent_node->right != NULL) ) {
	// 	if ((parent_node->left->isLeaf) and (parent_node->right->isLeaf))
	// 	{
	// 		pesan.cetak("+");

	// 		df_train.ReFilter(false);

	// 		float error_node, error_left, error_right, sum_error;

	// 		string left_label = parent_node->left->label;
	// 		string right_label = parent_node->right->label;

	// 		error_node = df_train.get_estimate_error();
	// 		string node_label = df_train.get_max_label();

	// 		df_left.ReFilter(false);
	// 		error_left = df_left.get_estimate_error();

	// 		df_right.ReFilter(false);
	// 		error_right = df_right.get_estimate_error();


	// 		sum_error = (((float) df_left.getjmlrow() / df_train.getjmlrow()) * error_left) + (((float) df_right.getjmlrow() / df_train.getjmlrow()) * error_right);

	// 		// cetak("[AttrIndex : %d train jml row : %d left jml row : %d right jml row : %d  error_node : %f sum_error : %f]\n",parent_node->criteriaAttrIndex,df_train.getjmlrow(),df_left.getjmlrow(),df_right.getjmlrow(),error_node,sum_error);

	// 		if (error_node < sum_error)
	// 		{
	// 			pesan.cetak("*");

	// 			parent_node->is_lanjut = parent_node->left->is_lanjut or parent_node->right->is_lanjut;

	// 			Tposisi_cabang tmp_posisi = df_train.get_posisi();
	// 			missing_branch.add_cut_off(tmp_posisi, 5);

	// 			pesan.cetak("[%d]", parent_node->criteriaAttrIndex);

	// 			delete parent_node->left;
	// 			delete parent_node->right;

	// 			parent_node->left = NULL;
	// 			parent_node->right = NULL;

	// 			parent_node->criteriaAttrIndex = -1;
	// 			parent_node->isLeaf = true;
	// 			parent_node->label = node_label;

	// 			if (node_label == "normal")
	// 			{
	// 				idx_svm++;
	// 				parent_node->idx_svm = idx_svm;
	// 			}

	// 			df_left.clear_memory(0);
	// 			df_right.clear_memory(0);
	// 			df_train.clear_memory(1);

	// 		}

	// 	}

	// }

}


void Tdt_build::post_pruning(Tdataframe & df_train)
{
	pruning_dfs(dec_tree, df_train);
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

	string head = to_string(global_config.id_detail_experiment) + "," + to_string(global_config.id_experiment) + "," ;


	for (size_t i = 0; i < tree.size(); ++i)
	{
		tmp_str = to_string(tree[i].criteriaAttrIndex) + ",'" + tree[i].attrValue.get_string() + "','" + tree[i].label + "'," + to_string(tree[i].treeIndex) + "," + (tree[i].isLeaf == true ? "1" : "0") + "," + to_string(tree[i].opt) ;

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

tree_node* Tdt_build::build_missing_branch(int counter, Tposisi_cabang posisi)
{
	cout << posisi.get_id_branch() << " masuk !!!" << endl;

	tree_node* parent_node = new tree_node;

	//cetak("%d ", counter);

	//missing_branch.get_stat(posisi);
	missing_branch.get_branch_stat(posisi);

	parent_node->is_pure = missing_branch.get_is_pure();
	parent_node->is_min_sample = missing_branch.get_is_min_sample();
	parent_node->is_depth_limit = missing_branch.get_is_depth_limit();
	parent_node->is_same_label = missing_branch.get_is_same_label();
	parent_node->is_pruning = missing_branch.get_is_pruning();
	parent_node->is_not_split = missing_branch.get_is_not_split();
	parent_node->is_lanjut = missing_branch.get_is_lanjut();

	bool is_split = missing_branch.get_is_split();

	parent_node->label = missing_branch.get_label();
	parent_node->jml_known = missing_branch.get_jml_known();
	parent_node->jml_normal = missing_branch.get_jml_normal();

	if (parent_node->is_pure or parent_node->is_min_sample or parent_node->is_depth_limit)
	{
		parent_node->isLeaf = true;

		//cetak("\n");

	} else {

		if ( parent_node->is_same_label  or parent_node->is_pruning or parent_node->is_not_split)
		{
			parent_node->isLeaf = true;

			if (is_split)
			{
				cout << posisi.get_id_branch() << " split !!!" << endl;

				parent_node->isLeaf = false;

				if (parent_node->is_not_split)
				{
					parent_node->isLeaf = true;
					//parent_node->is_lanjut = false;
				}

				//cetak("?|");

				counter++;

				auto itr = branch_number.find(counter);
				if (itr == branch_number.end()) {
					branch_number.insert({counter, 1});
				} else {
					branch_number[counter] = branch_number[counter] + 1;
				}

				int attrindex = missing_branch.get_attrindex();
				Tmy_dttype attrvalue = missing_branch.get_attrvalue();

				parent_node->criteriaAttrIndex = attrindex;

				Tposisi_cabang posisi_left = posisi;
				posisi_left.switch_parent_branch();
				posisi_left.set_child(counter, 1, branch_number[counter]);

				Tposisi_cabang posisi_right = posisi;
				posisi_right.switch_parent_branch();
				branch_number[counter] = branch_number[counter] + 1;
				posisi_right.set_child(counter, 2, branch_number[counter]);

				tree_node* left_node = NULL;
				if (missing_branch.get_opt_label_child(1, posisi, posisi_left))
				{
					branch_number[posisi_left.child_depth] = (branch_number[posisi_left.child_depth] < posisi_left.child_branch_number) ? posisi_left.child_branch_number : branch_number[posisi_left.child_depth];

					left_node = build_missing_branch(counter, posisi_left);

					missing_branch.get_branch_stat(posisi_left);

					if (left_node != NULL) {
						left_node->attrValue = attrvalue;
						left_node->opt = missing_branch.get_opt();

						left_node->depth = posisi_left.child_depth;
						left_node->branch = posisi_left.child_branch;
						left_node->branch_number = posisi_left.child_branch_number;

						cout << posisi.get_id_branch() << " left !!!" << endl;

					}

				}

				tree_node* right_node = NULL;
				if (missing_branch.get_opt_label_child(2, posisi, posisi_right))
				{
					branch_number[posisi_right.child_depth] = (branch_number[posisi_right.child_depth] < posisi_right.child_branch_number) ? posisi_right.child_branch_number : branch_number[posisi_right.child_depth];

					right_node = build_missing_branch(counter, posisi_right);

					missing_branch.get_branch_stat(posisi_right);

					if (right_node != NULL) {
						right_node->attrValue = attrvalue;
						right_node->opt = missing_branch.get_opt();

						right_node->depth = posisi_right.child_depth;
						right_node->branch = posisi_right.child_branch;
						right_node->branch_number = posisi_right.child_branch_number;

						cout << posisi.get_id_branch() << " right !!!" << endl;

					}

				}

				parent_node->right = right_node;
				parent_node->left = left_node;

			} else {
				//parent_node = NULL;
			}



		} else {
			//parent_node = NULL;
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

tree_node* Tdt_build::vec_tree_to_dec_tree(int node_index, int counter, Tposisi_cabang posisi)
{
	tree_node* parent_node = NULL;

	if (prev_tree[node_index].isLeaf)
	{
		parent_node = new tree_node;

		parent_node->label = prev_tree[node_index].label;
		parent_node->isLeaf = true;

	} else {

		parent_node = new tree_node;

		counter++;

		auto itr = branch_number.find(counter);
		if (itr == branch_number.end()) {
			branch_number.insert({counter, 0});
		}

		parent_node->criteriaAttrIndex = prev_tree[node_index].criteriaAttrIndex;

		int left = prev_tree[node_index].children[0];
		int right = prev_tree[node_index].children[1];

		Tposisi_cabang posisi_left;
		if (left != -1) {
			posisi_left = posisi;
			posisi_left.switch_parent_branch();
			branch_number[counter] = branch_number[counter] + 1;
			posisi_left.set_child(counter, 1, branch_number[counter]);
		}

		Tposisi_cabang posisi_right;
		if (right != -1) {
			posisi_right = posisi;
			posisi_right.switch_parent_branch();
			branch_number[counter] = branch_number[counter] + 1;
			posisi_right.set_child(counter, 2, branch_number[counter]);
		}


		if (left != -1) {
			tree_node* left_node = vec_tree_to_dec_tree(left, counter, posisi_left);
			left_node->opt = prev_tree[left].opt;
			left_node->attrValue.set_value(prev_tree[left].attrValue.get_string(), (prev_tree[left].opt == 0) or (prev_tree[left].opt == 1));
			left_node->depth = posisi_left.child_depth;
			left_node->branch = posisi_left.child_branch;
			left_node->branch_number = posisi_left.child_branch_number;

			parent_node->left = left_node;
		}

		if (right != -1) {
			tree_node* right_node = vec_tree_to_dec_tree(right, counter, posisi_right);
			right_node->opt = prev_tree[right].opt;
			right_node->attrValue.set_value(prev_tree[right].attrValue.get_string(), (prev_tree[right].opt == 0) or (prev_tree[right].opt == 1));
			right_node->depth = posisi_right.child_depth;
			right_node->branch = posisi_right.child_branch;
			right_node->branch_number = posisi_right.child_branch_number;

			parent_node->right = right_node;
		}

	}

	return parent_node;
}

void Tdt_build::read_tree(time_t id_detail_experiment)
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

		idx_svm = (tmp_data[8] == "-1") ? idx_svm : idx_svm + 1;

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

	Tposisi_cabang posisi_root;
	posisi_root.reset();

	dec_tree = vec_tree_to_dec_tree(0, 0, posisi_root);
	//trim_dec_tree(dec_tree);

	prev_tree.clear();
}

void Tdt_build::build_from_prev_tree(Tdataframe &df_train, int prev_tree_depth, bool bypass)
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
		train(df_train, dec_tree);
		//train(df_train, 0, 0, 0);
		pesan.cetak("\n");
		//branch_number.clear();
		df_train.set_parent(0, 0, 0);
		df_train.set_branch(0, 0, 0);
		//df_train.reset_depth_branch();

		if (global_config.prunning) {
			pesan.cetak("Start Prunning Decission Tree : \n");
			post_pruning(df_train);
			//pesan.cetak("\nEnd Prunning Decission Tree : \n");
		}
		pesan.cetak("\n");
	}

	missing_branch.delete_non_missing();
	save_tree();
	//df_train.close_file();

}

void Tdt_build::build_tree(Tdataframe &df_train)
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
		pesan.cetak("Start Train Decission Tree : \n");
		//train(df_train, 0, 0);
		dec_tree = train(df_train, 0);
	}

	pesan.cetak("\n");

	//branch_number.clear();
	df_train.set_parent(0, 0, 0);
	df_train.set_branch(0, 0, 0);
	//df_train.reset_depth_branch();


	if (global_config.prunning) {
		pesan.cetak("Start Prunning Decission Tree : \n");
		post_pruning(df_train);
		//cetak("\nEnd Prunning Decission Tree : \n");
	}

	pesan.cetak("\n");

	missing_branch.delete_non_missing();
	save_tree();
	//df_train.close_file();
}


