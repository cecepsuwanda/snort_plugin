#include <iostream>
#include <algorithm>
#include "Tmy_double.h"
#include "global.h"

using namespace std;


#ifndef Included_Tmy_list_alpha_H

#define Included_Tmy_list_alpha_H


class Tmy_list_alpha
{
private:
   int _jml_data;
   Tmy_double _lb;
   Tmy_double _ub;
   Tmy_double _jml_alpha;
   int _n_all_sv;
   int _n_sv;
   Tmy_double _jml_alpha_n_sv;

   vector<Tmy_double> _alpha;
   vector<int> _alpha_status;
   vector<int> _alpha_not_ub;
   vector<int> _alpha_not_lb;
   vector<int> _alpha_sv;

   void update_alpha_status(int idx);
   void update_alpha_sv(int idx);
   void update_lb_ub(int idx);

   
   vector<Tmy_double> calculateBoundaries(int i,int j);
   vector<Tmy_double> limit_alpha(Tmy_double alpha_a,Tmy_double alpha_b,Tmy_double Low,Tmy_double High, int flag);
   vector<Tmy_double> calculateNewAlpha(int i,int j,Tmy_double delta,Tmy_double Low,Tmy_double High);

public:
	Tmy_list_alpha(int v_jml_data,Tmy_double v_lb,Tmy_double v_ub);
	~Tmy_list_alpha();

	void init(Tmy_double V,Tmy_double eps);
	void update_alpha(int idx,Tmy_double value);
   
   bool is_lower_bound(int idx);
   bool is_upper_bound(int idx);
   bool is_free(int idx);
   bool is_pass(int i,int j,Tmy_double delta,vector<Tmy_double> *alpha);
   vector<bool> is_alpha_sv(int idx);
   void mv_lb_ub(int idx,int posisi,int flag1);
   
   Tmy_double get_alpha(int idx);
   vector<int> get_list_lb_ub(int flag);   
	
};


#endif