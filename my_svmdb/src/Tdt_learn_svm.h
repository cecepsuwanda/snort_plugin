#include "global.h"
#include <thread>
#include <experimental/filesystem>
#include "tb_experiment.h"
#include "Tdataframe.h"
#include "Tmy_svm.h"
#include "tb_tree.h"

#include<unistd.h>

#ifndef Included_Tdt_learn_svm_H

#define Included_Tdt_learn_svm_H

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;
using std::experimental::filesystem::directory_iterator;

namespace fs = std::experimental::filesystem;

class Tdt_learn_svm
{
private:
    vector<Node> tree;
    tree_node* dec_tree;

    map<int, int> branch_number;

	int idx_svm;
	int id_df;

	Tpesan pesan;
	Tglobal_config global_config;

	vector<thread> worker;

	static void thread_save_train(vector<vector<string>> table, int v_idx_svm);
	static void thread_train_svm(vector<vector<string>> table, int v_idx_svm);

    void f_train_svm(Tdataframe &df, int v_idx_svm);
	
	void clear_worker(int limit);
	
	void svm_dfs(int depth , int node_index , Tdataframe &df_train);

	tree_node* vec_tree_to_dec_tree(int node_index); 
	void del_dec_tree(tree_node* parent_node);

	void svm_datatset_dfs(tree_node* parent_node , Tdataframe &df,int counter);

public:
	Tdt_learn_svm();
	~Tdt_learn_svm();

	void set_svm_dataset(Tdataframe &df);

	void read_tree();	
	void learn_svm(Tdataframe &df);
	
};

#endif