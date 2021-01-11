#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

class Tmy_svm
{

private:
	struct svm_parameter param;
	struct svm_problem prob;
	struct svm_model *model;
	struct svm_node *x_space;

	bool is_read_problem; 

	void read_problem(Tdataframe &df);

public:
	Tmy_svm();
	~Tmy_svm();

	void train(Tdataframe &df,double gamma, double nu);
	void save_model(string nm_file);
	void load_model(string nm_file);
	void test(Tdataframe &df);
	string guess(Tdataframe &df,vector<string> &data);

};