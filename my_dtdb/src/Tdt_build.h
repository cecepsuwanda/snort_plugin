#include "global.h"
#include <future>
#include <thread>
#include "Tmy_dttype.h"
#include "tb_missing_branch.h"
#include "Tdataframe.h"
#include "Twrite_file.h"
#include "tb_tree.h"

#ifndef Included_Tdt_build_H

#define Included_Tdt_build_H

struct Tmetric_split_value
{
	int idx = 0;
	float overall_metric = -1;
	Tmy_dttype split_value;	
};

class Tdt_build
{
private:
	vector<Node> tree;
	vector<Node> prev_tree;

	map<int, int> branch_number;

	tree_node* dec_tree;

	int idx_svm;
	int prev_tree_depth;

	Tglobal_config global_config;
	Tpesan pesan;
	tb_missing_branch missing_branch;

	vector<thread> worker;

	static Tmetric_split_value get_split_value(Tdataframe &df, int idx);


	void determine_best_split(Tdataframe &df, int &split_column, Tmy_dttype &split_value);

	void clear_worker(size_t limit);
	string create_leaf(Tdataframe &df);
	bool check_purity(Tdataframe &df);


	void pruning_dfs(tree_node* parent_node, Tdataframe &df_train);

	//void pruning_dfs(int node_index , Tdataframe &df_train, int counter);
	void post_pruning(Tdataframe &df_train);

	bool limit_jml_dt_cabang(int jml_root, int jml_below, int jml_above);

	tree_node* train(Tdataframe &df, int counter);
	tree_node* train_prev_tree(Tdataframe &df, int counter, tree_node* prev_tree);
	tree_node* build_missing_branch(int counter, Tposisi_cabang posisi);

	//void train(Tdataframe &df, int node_index , int counter);

	void train(Tdataframe &df, tree_node* parent_node);
	//void train(Tdataframe &df, int prev_tree_node_index, int node_index , int counter);

	void dec_tree_to_vec_tree(tree_node* parent_node, int node_index);
	tree_node* vec_tree_to_dec_tree(int node_index, int counter, Tposisi_cabang posisi);
	void trim_dec_tree(tree_node* parent_node);


	void save_tree();
	void del_dec_tree(tree_node* parent_node);

public:
	Tdt_build();
	~Tdt_build();

	void build_tree(Tdataframe &df_train);
	void read_tree(time_t id_detail_experiment);
	void build_from_prev_tree(Tdataframe &df_train, int prev_tree_depth, bool bypass);

};
#endif
