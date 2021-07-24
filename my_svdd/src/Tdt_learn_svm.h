#include "global.h"
#include <thread>
#include "Tdataframe.h"
#include "Timer.h"
#include "Tmy_svdd.h"

#ifndef Included_Tdt_learn_svm_H

#define Included_Tdt_learn_svm_H

class Tdt_learn_svm
{
private:
    vector<Node> tree;

	int idx_svm;
	int id_df;

	Tconfig *config;

	vector<thread> worker;

	static void thread_save_train(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm);
	static void thread_train_svm(Tconfig* v_config, vector<vector<string>> table, int v_idx_svm);

    void f_train_svm(Tdataframe &df, int v_idx_svm);
	void clear_worker(int limit);
	void svm_dfs(int depth , int node_index , Tdataframe &df_train);

public:
	Tdt_learn_svm(Tconfig *v_config);
	~Tdt_learn_svm();

	void read_tree();	
	void learn_svm();
	
};

#endif