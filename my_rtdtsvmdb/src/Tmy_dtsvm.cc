#include "Tmy_dtsvm.h"


Tmy_dtsvm::Tmy_dtsvm()
{

}

Tmy_dtsvm::~Tmy_dtsvm()
{
	tree.clear();
}

void Tmy_dtsvm::read_tree()
{
	vector<string> tmp_data;
	tb_tree dbtree;
	dbtree.baca_tree();

	int i = 0;
	dbtree.reset_file();
	while (!dbtree.is_eof()) {

		tmp_data = dbtree.get_record();

		Node newnode;
		//cout << tmp_data[0] << endl;
		newnode.criteriaAttrIndex = tmp_data[0] == "-1" ?  -1 : stoi(tmp_data[0]);
		newnode.attrValue = tmp_data[1];
		newnode.label = tmp_data[2];
		//cout << tmp_data[2] << endl;
		newnode.treeIndex = tmp_data[3] == "-1" ? -1 : stoi(tmp_data[3]);
		newnode.isLeaf = tmp_data[4] == "1";
		//cout << tmp_data[4] << endl;
		newnode.opt = tmp_data[5] == "-1" ? -1 : stoi(tmp_data[5]);
		//cout << tmp_data[5] << endl;
		newnode.children.push_back(tmp_data[6] == "-1" ? -1 :  stoi(tmp_data[6]));
		//cout << tmp_data[6] << endl;
		newnode.children.push_back(tmp_data[7] == "-1" ? -1 :  stoi(tmp_data[7]));

		newnode.idx_svm = tmp_data[8] == "-1" ? -1 : stoi(tmp_data[8]);

		//   if (idx_svm < newnode.idx_svm)
		//   {
		//     idx_svm = newnode.idx_svm;
		//   }

		tree.push_back(newnode);

		dbtree.next_record();
	}

	dbtree.close_file();
}

int Tmy_dtsvm::dfs(Tkddcup_attr &data, int treeIndex)
{
	if (tree[treeIndex].isLeaf)
	{
		return treeIndex;
	}

	int criteriaAttrIndex = tree[treeIndex].criteriaAttrIndex;

	for (size_t i = 0; i < tree[treeIndex].children.size(); i++) {
		int next = tree[treeIndex].children[i];
		if (data.is_pass(criteriaAttrIndex,tree[next].opt, tree[next].attrValue) ) {
			return dfs(data, next);
		}
	}

	return -1;
}

string Tmy_dtsvm::guess(Tkddcup_attr &data)
{
	string label = "failed";
	int leafNode = dfs(data, 0);

	if (leafNode != -1)
	{
		label = tree[leafNode].label;
		if ((label == "normal") and (tree[leafNode].idx_svm != -1))
		{

		}
	}

	return label;
}