#include <string>
#include "Tdataframe.h"
#include "Tmy_alpha.h"
#include "Tmy_kernel.h"
#include "Tmy_G.h"
#include "Tmy_list_G.h"
#include "Tmy_list_alpha.h"
#include "Tmy_double.h"

using namespace std;

#ifndef Included_Tmy_svm_H

#define Included_Tmy_svm_H

class Tmy_svm
{

private:
    Tconfig *_config;
    Tmy_alpha *_my_alpha;
    Tmy_kernel *_my_kernel;
    Tmy_G *_my_G;
    Tmy_double _rho;
public:
	Tmy_svm(Tconfig *v_config);
	~Tmy_svm();
	void train(Tdataframe &df);
	bool cari_idx_alpha(vector<int> *idx_alpha);
	bool cari_idx_a_lain(int idx_b,int *idx_alpha);
	bool take_step(int idx_b,int idx_a);
	
};

#endif
