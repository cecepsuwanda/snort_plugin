#include <cstdio>
#include <stdarg.h>
#include <stdio.h>
#include <thread>
#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "Tmy_dttype.h"
#include "Tmy_svm.h"
#include "Twrite_file.h"

using namespace std;

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

	bool check_purity(Tdataframe &df);
	
	string create_leaf(Tdataframe &df);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	int dfs(vector<string> &data, int treeIndex);
	void pruning_dfs(int node_index ,Tdataframe &df_train);

    vector<int> vec_attr;
    
	void cetak ( const char * format, ... );

	bool is_pass(int opt, string value1, string value2);

	bool train_svm = false;
	bool feature_selection = false;
	bool normal_only = false;
	double _gamma = 0.0001;
	double _nu = 0.01;

	static void col_pot_split(Tdataframe df,int i, float & current_overall_metric, string & current_split_value);
	void f_train_svm(Tdataframe &df,int v_idx_svm);
    void del_model_train(int idx);
public:
	Tdec_tree();
	~Tdec_tree();
    Tdec_tree(int v_train_svm,int v_feature_selection,int v_normal_only,double v_gamma,double v_nu);


	string guess(Tdataframe &df, vector<string> &data);

	void test(Tdataframe &df);
	void read_tree(Tdataframe &df);
	void train(Tdataframe &df, int node_index , int counter, int min_samples, int max_depth);
	void post_pruning(Tdataframe &df_train);
	void save_tree();
 
    void set_model_path(string path);
};
