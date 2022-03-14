#include <iostream>
#include <algorithm>
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
   int _n_all_sv;
   int _n_sv;
   double _jml_alpha_n_sv;

   vector<double> _alpha;
   vector<int> _alpha_status;
   vector<int> _alpha_not_ub;
   vector<int> _alpha_not_lb;
   vector<int> _alpha_sv;

   void update_alpha_status(int idx);
   void update_alpha_sv(int idx);

   vector<bool> is_alpha_sv(int idx);
   vector<double> calculateBoundaries(int i,int j);
   vector<double> limit_alpha(double alpha_a,double alpha_b,double Low,double High, int flag);

public:
	Tmy_list_alpha(int v_jml_data,double v_lb,double v_ub);
	~Tmy_list_alpha();

	void init(double V,double eps);
	void update_alpha(int idx,double value);
   void update_lb_ub(int idx);

   bool is_lower_bound(int idx);
   bool is_upper_bound(int idx);
   bool is_free(int idx);


	
};


#endif