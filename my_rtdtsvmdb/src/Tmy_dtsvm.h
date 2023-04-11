#include <string>

#include "global.h"
#include "tb_tree.h"
#include "Tkddcup_attr.h"

using namespace std;

#ifndef Included_tmy_dtsvm_H

#define Included_tmy_dtsvm_H

class Tmy_dtsvm
{
private:
	vector<Node> tree;

	int dfs(Tkddcup_attr &data, int treeIndex);

public:
	Tmy_dtsvm();
	~Tmy_dtsvm();

	void read_tree();
	string guess(Tkddcup_attr &data);
	
};

#endif