#include "global.h"
#include <thread>
#include "Tdataframe.h"
#include "Timer.h"
#include "Twrite_file.h"
#include "Tmy_svdd.h"

#ifndef Included_Tdt_build_H

#define Included_Tdt_build_H

class Tdt_build
{
private:
	vector<Node> tree;

	int idx_svm;
	int id_df;

	Tconfig *config;

	vector<thread> worker;

	static Tmetric_split_value get_split_value(Tdataframe &df, int idx);
	static void thread_save_train(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm);
	static void thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm);

	void del_model_train(int idx);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	void f_train_svm(Tdataframe &df, int v_idx_svm);
	void clear_worker(int limit);
	string create_leaf(Tdataframe &df);
	bool check_purity(Tdataframe &df);
	void pruning_dfs(int node_index , Tdataframe &df_train);
	void post_pruning(Tdataframe &df_train);
	void train(Tdataframe &df, int node_index , int counter);
	void save_tree();

public:
	Tdt_build(Tconfig *v_config);
	~Tdt_build();

	void build_tree();

};
#endif
