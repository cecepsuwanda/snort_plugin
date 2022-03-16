#include <string>
#include <math.h>
#include "Tdataframe.h"

using namespace std;

#ifndef Included_Tmy_kernel_H

#define Included_Tmy_kernel_H

class Tmy_kernel
{
private:
	Tdataframe *_df;
	double _gamma;
	int _jml_data;

	double dot(vector<string> x,vector<string> y);
	double kernel_function(int i,int j);
public:
	Tmy_kernel(Tdataframe &df,double gamma);
	~Tmy_kernel();

	vector<double> get_Q(int i);
	vector<double> hit_eta(int i,int j);  
	
};

#endif