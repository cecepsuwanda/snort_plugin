#include <string>
#include <math.h>
#include "Tdataframe.h"
#include "Tmy_double.h"
#include "global.h"

using namespace std;

#ifndef Included_Tmy_kernel_H

#define Included_Tmy_kernel_H

class Tmy_kernel
{
private:
	Tdataframe *_df;
	Tmy_double _gamma;
	int _jml_data;

	Tmy_double dot(vector<string> x,vector<string> y);
	Tmy_double kernel_function(int i,int j);
public:
	Tmy_kernel(Tdataframe &df,double gamma);
	~Tmy_kernel();

	vector<Tmy_double> get_Q(int i);
	vector<Tmy_double> hit_eta(int i,int j);  
	
};

#endif