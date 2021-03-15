#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "Tmy_dttype.h"
#include "Tmy_svm.h"
#include "Twrite_file.h"
#include <experimental/filesystem>

using namespace std;
using std::experimental::filesystem::directory_iterator;

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
	string model_path;
	string svm_path;
	string _f_train;
	string _f_datatype;
	string _f_test;

	bool train_svm = false;
	bool feature_selection = false;
	bool normal_only = false;
	bool save_train = true;
	bool save_test = true;

	int _min_sample = 2;
	int _depth = 1;
	double _gamma = 0.0001;
	double _nu = 0.01;

	map<int, int> vec_attr;

	bool check_purity(Tdataframe &df);

	string create_leaf(Tdataframe &df);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	void train(Tdataframe &df, int node_index , int counter, int min_samples, int max_depth);
	int dfs(vector<string> &data, int treeIndex);
	void pruning_dfs(int node_index , Tdataframe &df_train);
	void post_pruning(Tdataframe &df_train);
	void save_tree();


	void svm_dfs(int depth , int node_index , Tdataframe &df_train);

	void cetak ( const char * format, ... );

	bool is_pass(int opt, string value1, string value2);
	void f_train_svm(Tdataframe &df, int v_idx_svm);
	void del_model_train(int idx);

	static void col_pot_split(Tdataframe df, int i, float & current_overall_metric, string & current_split_value);

public:
	Tdec_tree();
	~Tdec_tree();
	Tdec_tree(int v_train_svm, int v_min_sample, int v_depth,int v_save_train,int v_save_test);


	string guess(vector<string> &data);

	void test();
	void read_tree();
	void build_tree();
	void learn_svm();

	void set_model_path(string path);
	void set_svm_path(string path);
	void set_svm_param(int v_feature_selection, int v_normal_only, double v_gamma, double v_nu);
	void set_f_train(string f_train);
	void set_f_test(string f_test);
	void set_f_datatype(string f_datatype);
};
