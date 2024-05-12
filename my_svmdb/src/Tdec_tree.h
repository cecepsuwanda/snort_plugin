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
#include <chrono>

using namespace std;
using std::experimental::filesystem::directory_iterator;

#ifndef Included_Tdec_tree_H

#define Included_Tdec_tree_H

struct Thsl_test
{
  int idx_svm;
  time_t id_more_detail_experiment;
  map<string, string> data;
};

class Tdec_tree
{

private:
	
	Tpesan pesan;
	Tglobal_config global_config;

	vector<thread> worker;
	
		
	void clear_worker(size_t limit);
	int dfs(vector<string> &data, int treeIndex);	
	
    
	static Thsl_test thread_test_svm(map<string,vector<string>> table, int v_idx_svm,tb_experiment v_experiment);	

public:
	Tdec_tree();
	~Tdec_tree();

	//void set_config(Tconfig v_config);
	
	void test(Tdataframe &df_test,tb_experiment &experiment);
	
};

#endif