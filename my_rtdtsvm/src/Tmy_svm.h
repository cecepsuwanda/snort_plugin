#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "svm.h"
#include <string>
#include <sys/mman.h>

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

	void cetak ( const char * format, ... );

public:
	Tmy_svm();
	~Tmy_svm();
	Tmy_svm(bool feature_selection, bool _normal_only);

	void train(Tdataframe &df,double gamma, double nu);
	void save_model(string nm_file);
	void load_model(string nm_file);
	void test(Tdataframe &df);
	string guess(Tdataframe &df,vector<string> &data);

};