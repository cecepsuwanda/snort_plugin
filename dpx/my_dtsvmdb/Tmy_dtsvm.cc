#include "Tmy_dtsvm.h"


Tmy_dtsvm::Tmy_dtsvm()
{
	svm_path = NULL;
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
		if (data.is_pass(criteriaAttrIndex, tree[next].opt, tree[next].attrValue) ) {
			return dfs(data, next);
		}
	}

	return -1;
}

void Tmy_dtsvm::set_svm_path(char *path)
{

	svm_path = path;
}

void Tmy_dtsvm::set_dataset_file(char *nm_file)
{
	dataset_file = nm_file;
}


string Tmy_dtsvm::svm_guess(vector<string> vec_attr, int idx_svm)
{
	
	string label = "failed";

	char *endptr;
	x_space = (struct svm_node *) malloc((vec_attr.size()+1) * sizeof(struct svm_node));

	size_t k = 0;
	for (; k < vec_attr.size(); k++) {
		string str = vec_attr[k];
		x_space[k].index = k;
		x_space[k].value = strtod(str.c_str(), &endptr);
	}
	x_space[k].index = -1;

	char file_model[225];
	strcpy(file_model, svm_path);
	string nm_file = "/svm_model_" + to_string(idx_svm) + ".csv";
	strcat(file_model, nm_file.c_str());
	strcpy(svm_file,file_model);
    
	if ((model = svm_load_model(file_model)) == 0)
	{
		fprintf(stderr, "can't open model file %s\n", file_model);
		exit(1);
	}

	double predict_label = svm_predict(model, x_space);

	free(x_space);
	svm_free_and_destroy_model(&model);

	label = "normal";
	if (predict_label == -1)
	{
		label = "unknown";
	}
	

	return label;
}


string Tmy_dtsvm::guess(Tkddcup_attr &data)
{	
	string label = "failed";
	int leafNode = dfs(data, 0);	

	if (leafNode != -1)
	{
		label = tree[leafNode].label;		
		strcpy(svm_file,"");
		if ((label == "normal") and (tree[leafNode].idx_svm != -1))
		{
			vector<string> vec_attr = data.get_svm_attr();
			label = svm_guess(vec_attr, tree[leafNode].idx_svm);
			vec_attr.clear();
			vec_attr.shrink_to_fit();						
		}		
		//data.set_file(dataset_file,svm_file);		
	}
	return label;
}