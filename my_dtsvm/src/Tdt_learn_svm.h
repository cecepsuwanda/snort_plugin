#include "global.h"
#include <thread>
#include "Tdataframe.h"
#include "Timer.h"
#include "Tmy_svm.h"

#ifndef Included_Tdt_learn_svm_H

#define Included_Tdt_learn_svm_H

class Tdt_learn_svm
{
private:
    vector<Node> tree;
    tree_node* dec_tree;

	int idx_svm;
	int id_df;

	Tpesan pesan;
	Tglobal_config global_config;

	vector<thread> worker;

	static void thread_save_train(vector<vector<string>> table, int v_idx_svm);
	static void thread_train_svm(vector<vector<string>> table, int v_idx_svm);

    void f_train_svm(Tdataframe &df, int v_idx_svm);
	void clear_worker(size_t limit);
	void svm_dfs(int depth , tree_node* parent_node , Tdataframe &df_train);

	tree_node* vec_tree_to_dec_tree(int node_index);

public:
	Tdt_learn_svm();
	~Tdt_learn_svm();

	void read_tree();	
	void learn_svm();
	
};

#endif