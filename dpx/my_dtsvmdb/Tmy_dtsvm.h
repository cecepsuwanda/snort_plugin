#include <string>
#include <cstring>

#include "global.h"
#include "svm.h"
#include "tb_tree.h"
#include "Tkddcup_attr.h"

using namespace std;

#ifndef Included_tmy_dtsvm_H

#define Included_tmy_dtsvm_H

class Tmy_dtsvm
{
private:
	char *svm_path;
    char *dataset_file;
    char svm_file[225]; 	

	struct svm_model *model;
	struct svm_node *x_space;

	vector<Node> tree;

	int dfs(Tkddcup_attr &data, int treeIndex);

public:
	Tmy_dtsvm();
	~Tmy_dtsvm();

	void read_tree();

	void set_svm_path(char *path);
	void set_dataset_file(char *nm_file);
	
	string svm_guess(vector<string> vec_attr,int idx_svm);

	string guess(Tkddcup_attr &data);
	
};

#endif