#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "svm.h"
#include <string>
#include <sys/mman.h>
#include "Twrite_file.h"

using namespace std;

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

	void read_problem(Tdataframe &df);

	static void print_null(const char *s) {}

	bool _feature_selection = false;
	bool _normal_only = false;
	bool _save_train = true;	

	void cetak ( const char * format, ... );

	int _idx_svm;
	string _model_path;
	string _svm_path;

public:
	Tmy_svm();
	~Tmy_svm();
	Tmy_svm(bool feature_selection, bool _normal_only, int idx_svm,string model_path,string svm_path, bool save_train);

	void train(Tdataframe &df,double gamma, double nu);
	void save_model(string nm_file);
	void load_model(string nm_file);
	void destroy_model();
	void test(Tdataframe &df);
	string guess(vector<string> &data);

};