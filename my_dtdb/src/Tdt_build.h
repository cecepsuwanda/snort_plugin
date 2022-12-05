#include "global.h"
#include <future>
#include <thread>
#include "Tdataframe.h"
#include "Timer.h"
#include "Twrite_file.h"
#include "Tmy_svm.h"
#include "tb_tree.h"

#ifndef Included_Tdt_build_H

#define Included_Tdt_build_H

class Tdt_build
{
private:
	vector<Node> tree;	
	vector<Node> prev_tree;

	tree_node* dec_tree; 

	int idx_svm;
	int id_df;
	int prev_tree_depth;

	Tconfig *config;

	vector<thread> worker;

	static Tmetric_split_value get_split_value(Tdataframe &df, int idx);
	
	void del_model_train(int idx);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	
	void clear_worker(size_t limit);
	string create_leaf(Tdataframe &df);
	bool check_purity(Tdataframe &df);
	

	void pruning_dfs(tree_node* parent_node, Tdataframe &df_train, int counter);

	void pruning_dfs(int node_index , Tdataframe &df_train, int counter);	
	void post_pruning(Tdataframe &df_train);

    tree_node* train(Tdataframe &df, int counter);	
	void train(Tdataframe &df, int node_index , int counter);
	
	void train(Tdataframe &df, tree_node* parent_node, int counter);
	void train(Tdataframe &df, int prev_tree_node_index, int node_index , int counter);
	
    void dec_tree_to_vec_tree(tree_node* parent_node, int node_index);
    tree_node* vec_tree_to_dec_tree(int node_index);    

	void save_tree();

public:
	Tdt_build(Tconfig *v_config);
	~Tdt_build();

	void build_tree();
	void read_tree(time_t id_detail_experiment);
	void build_from_prev_tree(int prev_tree_depth);

};
#endif
