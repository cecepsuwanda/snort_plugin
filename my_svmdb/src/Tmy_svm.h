#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "svm.h"
#include <string>
#include <sys/mman.h>
#include "Twrite_file.h"
#include "global.h"

using namespace std;

#ifndef Included_Tmy_svm_H

#define Included_Tmy_svm_H

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class Tmy_svm
{

private:
	struct svm_parameter param;
	struct svm_problem prob;
	struct svm_model *model;
	struct svm_node *x_space;

	//size_t elements_size;

	bool is_read_problem; 

	void read_problem(vector<vector<string>> table);

	static void print_null(const char *s) {}
	
	int _idx_svm;
	
	Tconfig* config;

public:
	Tmy_svm();
	Tmy_svm(Tconfig* v_config);
	~Tmy_svm();
	void train(vector<vector<string>> table);
	void save_model(string nm_file);
	void load_model(string nm_file);
	void destroy_model();
	void test(Tdataframe &df,Tconf_metrix &p_conf_metrix);
	string guess(vector<string> &data);

};

#endif