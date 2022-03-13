#include <iostream>
#include "Tmy_list_alpha.h"
#include "global.h"

using namespace std;

#ifndef Included_Tmy_alpha_H

#define Included_Tmy_alpha_H

class Tmy_alpha
{
private:
	Tconfig *_config;
    Tmy_list_alpha *_my_list_alpha;
public:
	Tmy_alpha(Tconfig *v_config);
	~Tmy_alpha();

    void init(int jml_data);
	
};


#endif