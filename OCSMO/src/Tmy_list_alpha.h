#include <iostream>
#include "global.h"

using namespace std;


#ifndef Included_Tmy_list_alpha_H

#define Included_Tmy_list_alpha_H


class Tmy_list_alpha
{
private:
   int _jml_data;
   double _lb;
   double _ub;
   double _jml_alpha;

   vector<double> _alpha;
   vector<int> _alpha_status;

   void update_alpha_status(int idx);

public:
	Tmy_list_alpha(int v_jml_data,double v_lb,double v_ub);
	~Tmy_list_alpha();

	void init(double V,double eps);
	void update_alpha(int idx,double value);

	
};


#endif