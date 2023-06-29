#include "global.h"
#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include <future>
#include <mutex>
#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "Tmy_dttype.h"
#include "Twrite_file.h"
#include "credal.h"
#include "tb_tree.h"

#include <experimental/filesystem>

using namespace std;
using std::experimental::filesystem::directory_iterator;

#ifndef Included_Tdec_tree_H

#define Included_Tdec_tree_H

class Tdec_tree
{

private:
	vector<Node> tree;
	tree_node* dec_tree;

	map<int, int> branch_number;

	int idx_svm;
	int id_df;

	Tglobal_config global_config;
	Tpesan pesan; 

	vector<thread> worker;
	
	map<int, int> vec_attr;

	vector<vector<string>> _table_svm;
	vector<vector<string>> _table_attack;	
	
	void clear_worker(size_t limit);
	int dfs(vector<string> &data, int treeIndex);	
	
    void test_dfs(tree_node* parent_node , Tdataframe &df_test, Tconf_metrix &dt_conf_metrix, int counter);
	
	
	bool is_pass(int opt, string value1, string value2);

	//static void thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &dt_conf_metrix);
	static void thread_test_attack( map<string,map<string,int>> metrix, Tconf_metrix &dt_conf_metrix);
	
    tree_node* vec_tree_to_dec_tree(int node_index); 
	void del_dec_tree(tree_node* parent_node);

public:
	Tdec_tree();
	~Tdec_tree();

	//void set_config(Tconfig v_config);

	string guess(vector<string> &data);

	void test(Tdataframe &df_test,Tconf_metrix &dt_conf_metrix);
	void read_tree();
};

#endif