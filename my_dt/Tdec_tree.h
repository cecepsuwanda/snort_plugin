#include "Tdataframe.h"


class Node {
public:
	int criteriaAttrIndex;
	string attrValue;
	string label;

	int treeIndex;
	bool isLeaf;
	int opt;

	vector<int > children;

	Node() {
		criteriaAttrIndex = -1;
		attrValue = "-1";
		label = "-1";
		treeIndex = -1;
		isLeaf = false;
		opt = -1;
	}
};


class Tdec_tree
{

private:
	vector<Node> tree;

	bool check_purity(Tdataframe &df);
	map<int, map<string, int>> get_potential_splits(Tdataframe &df);

	string create_leaf(Tdataframe &df);
	void determine_best_split(Tdataframe &df, int &split_column, string &split_value);
	int dfs(Tdataframe &df, vector<string> &data, int treeIndex);
public:
	Tdec_tree();
	~Tdec_tree();

	string guess(Tdataframe &df, vector<string> &data);

	void test(Tdataframe &df);
	void read_tree(Tdataframe &df);
	void train(Tdataframe &df, int node_index , int counter, int min_samples, int max_depth);
	void save_tree(Tdataframe &df);

};
