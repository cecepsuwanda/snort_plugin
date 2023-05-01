#include "global.h"
#include <future>
#include <thread>
#include "tb_missing_branch.h"
#include "Tdataframe.h"
#include "Twrite_file.h"
#include "tb_tree.h"

#ifndef Included_Tdt_build_H

#define Included_Tdt_build_H

class Tdt_build
{
private:
	vector<Node> tree;	
	vector<Node> prev_tree;

	map<int, int> branch_number;

	tree_node* dec_tree; 

	int idx_svm;
	int prev_tree_depth;

	Tconfig *config;

	vector<thread> worker;

	static Tmetric_split_value get_split_value(Tdataframe &df, int idx);
	
	void del_model_train(int idx);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	
	void clear_worker(size_t limit);
	string create_leaf(Tdataframe &df);
	bool check_purity(Tdataframe &df);
	

	void pruning_dfs(tree_node* parent_node, Tdataframe &df_train,tb_missing_branch &missing_branch);

	//void pruning_dfs(int node_index , Tdataframe &df_train, int counter);	
	void post_pruning(Tdataframe &df_train,tb_missing_branch &missing_branch);

    tree_node* train(Tdataframe &df,tb_missing_branch &missing_branch, int counter);
    tree_node* train_prev_tree(Tdataframe &df,tb_missing_branch &missing_branch, int counter, tree_node* prev_tree);	
	tree_node* build_missing_branch(int counter,posisi_cabang posisi,tb_missing_branch &missing_branch);

	//void train(Tdataframe &df, int node_index , int counter);
	
	void train(Tdataframe &df,tb_missing_branch &missing_branch, tree_node* parent_node);
	//void train(Tdataframe &df, int prev_tree_node_index, int node_index , int counter);
	
    void dec_tree_to_vec_tree(tree_node* parent_node, int node_index);
    tree_node* vec_tree_to_dec_tree(int node_index,int counter,posisi_cabang posisi,tb_missing_branch &missing_branch);
    void trim_dec_tree(tree_node* parent_node); 


	void save_tree();
	void del_dec_tree(tree_node* parent_node);

public:
	Tdt_build(Tconfig *v_config);
	~Tdt_build();

	void build_tree(Tdataframe &df_train,tb_missing_branch &missing_branch);
	void read_tree(time_t id_detail_experiment,tb_missing_branch &missing_branch);
	void build_from_prev_tree(Tdataframe &df_train,tb_missing_branch &missing_branch,int prev_tree_depth,bool bypass);

};
#endif
