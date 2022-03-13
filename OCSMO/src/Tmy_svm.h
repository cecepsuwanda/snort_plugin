#include <string>
#include "Tdataframe.h"
#include "Tmy_alpha.h"

using namespace std;

#ifndef Included_Tmy_svm_H

#define Included_Tmy_svm_H

class Tmy_svm
{

private:
    Tconfig *_config;
    Tmy_alpha *_my_alpha;
public:
	Tmy_svm(Tconfig *v_config);
	~Tmy_svm();
	void train(Tdataframe &df);
	
};

#endif
