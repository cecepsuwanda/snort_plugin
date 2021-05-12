#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "Tmy_dttype.h"
#include "Tmy_svm.h"
#include "Twrite_file.h"
#include "credal.h"
#include "global.h"
#include "Timer.h"
#include <experimental/filesystem>

using namespace std;
using std::experimental::filesystem::directory_iterator;

#ifndef Included_Tdec_tree_H

#define Included_Tdec_tree_H

static void cetak_stdout(const char *s);



class Node {
public:
	int criteriaAttrIndex;
	string attrValue;
	string label;

	int treeIndex;
	bool isLeaf;
	int opt;
	int idx_svm;

	vector<int > children;

	Node() {
		criteriaAttrIndex = -1;
		attrValue = "-1";
		label = "-1";
		treeIndex = -1;
		isLeaf = false;
		opt = -1;
		idx_svm = -1;
	}
};


class Tdec_tree
{

private:
	vector<Node> tree;

	int idx_svm;
	int id_df;
	
    Tconfig config;

	map<int, int> vec_attr;


	bool check_purity(Tdataframe &df);

	string create_leaf(Tdataframe &df);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	void train(Tdataframe &df, int node_index , int counter);
	int dfs(vector<string> &data, int treeIndex);
	void pruning_dfs(int node_index , Tdataframe &df_train);
	void post_pruning(Tdataframe &df_train);
	void test_dfs(int node_index , Tdataframe &df_test, Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &svm_conf_metrix, Tconf_metrix &dt_svm_conf_metrix);
	void save_tree();


	void svm_dfs(int depth , int node_index , Tdataframe &df_train);

	void cetak ( const char * format, ... );

	bool is_pass(int opt, string value1, string value2);
	void f_train_svm(Tdataframe &df, int v_idx_svm);
	void del_model_train(int idx);

	//static void col_pot_split(Tdataframe df, int i, float & current_overall_metric, string & current_split_value);

    static void thread_save_train(Tconfig v_config, vector<vector<string>> table,int v_idx_svm);
    
    static void thread_train_svm(Tconfig v_config, vector<vector<string>> table,int v_idx_svm);    
    
    static void thread_test_attack(string label,vector<vector<string>> table, Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &dt_svm_conf_metrix);

    static void thread_test_svm(int idx_svm,Tconfig v_config,string label,vector<vector<string>> table,Tconf_metrix &conf_metrix, Tconf_metrix &dt_conf_metrix, Tconf_metrix &svm_conf_metrix, Tconf_metrix &dt_svm_conf_metrix);
        
    vector<thread> worker;
    void clear_worker(int limit);
    
public:
	Tdec_tree();
	~Tdec_tree();

	void set_config(Tconfig v_config);
	
	string guess(vector<string> &data);

	void test();
	void read_tree();
	void build_tree();
	void learn_svm();	
};

#endif