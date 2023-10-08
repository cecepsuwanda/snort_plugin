#include "global.h"
#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include <future>
#include <mutex>
#include "tb_experiment.h"
#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "Tmy_dttype.h"
#include "Tmy_svm.h"
#include "Twrite_file.h"
#include "tb_tree.h"

#include <experimental/filesystem>

using namespace std;
using std::experimental::filesystem::directory_iterator;

#ifndef Included_Tdec_tree_H

#define Included_Tdec_tree_H

class Tdec_tree
{

private:
	
	Tpesan pesan;
	Tglobal_config global_config;

	vector<thread> worker;
	
		
	void clear_worker(size_t limit);
	int dfs(vector<string> &data, int treeIndex);	
	
    
	static void thread_test_attack(string label, vector<vector<string>> table, Tconf_metrix &dt_conf_metrix);	

public:
	Tdec_tree();
	~Tdec_tree();

	//void set_config(Tconfig v_config);
	
	void test(Tdataframe &df_test,tb_experiment &experiment);
	
};

#endif