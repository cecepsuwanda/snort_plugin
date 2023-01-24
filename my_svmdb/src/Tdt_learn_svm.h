#include "global.h"
#include <thread>
#include "tb_experiment.h"
#include "Tdataframe.h"
#include "Tmy_svm.h"
#include "tb_tree.h"

#ifndef Included_Tdt_learn_svm_H

#define Included_Tdt_learn_svm_H

class Tdt_learn_svm
{
private:
    vector<Node> tree;
    tree_node* dec_tree;

    map<int, int> branch_number;

	int idx_svm;
	int id_df;

	Tconfig *config;

	vector<thread> worker;

	static void thread_save_train(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm);
	static void thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm,tb_experiment &experiment);

    void f_train_svm(Tdataframe &df, int v_idx_svm,tb_experiment &experiment);
	
	void clear_worker(int limit);
	
	void svm_dfs(int depth , int node_index , Tdataframe &df_train);

	tree_node* vec_tree_to_dec_tree(int node_index); 
	void del_dec_tree(tree_node* parent_node);

	void svm_datatset_dfs(tree_node* parent_node , Tdataframe &df,int counter);

public:
	Tdt_learn_svm(Tconfig *v_config);
	~Tdt_learn_svm();

	void set_svm_dataset(Tdataframe &df);

	void read_tree();	
	void learn_svm(Tdataframe &df,tb_experiment &experiment);
	
};

#endif