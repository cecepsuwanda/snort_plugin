#include "Tdataframe.h"
#include "Tconf_metrix.h"
#include "svm.h"
#include <string>
#include <sys/mman.h>
#include "Twrite_file.h"
#include "global.h"

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

	void cetak ( const char * format, ... );

	int _idx_svm;
	
	Tconfig config;

public:
	Tmy_svm();
	~Tmy_svm();
	void set_config(Tconfig v_config);
	void train(Tdataframe &df);
	void save_model(string nm_file);
	void load_model(string nm_file);
	void destroy_model();
	void test(Tdataframe &df);
	string guess(vector<string> &data);

};