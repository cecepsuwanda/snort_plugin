#include <string>
#include "Tdataframe.h"
#include "Tmy_alpha.h"
#include "Tmy_kernel.h"

using namespace std;

#ifndef Included_Tmy_svm_H

#define Included_Tmy_svm_H

class Tmy_svm
{

private:
    Tconfig *_config;
    Tmy_alpha *_my_alpha;
    Tmy_kernel *_my_kernel;
public:
	Tmy_svm(Tconfig *v_config);
	~Tmy_svm();
	void train(Tdataframe &df);
	
};

#endif
