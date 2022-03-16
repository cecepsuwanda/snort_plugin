#include <string>
#include "Tmy_kernel.h"
#include "Tmy_list_alpha.h"

using namespace std;

#ifndef Included_Tmy_list_G_H

#define Included_Tmy_list_G_H


class Tmy_list_G
{
private:
	int _jml_data;
	Tmy_kernel *_kernel;
    Tmy_list_alpha *_alpha;

    vector<double> _arr_G;

    bool is_kkt(int idx,double rho);
public:
	Tmy_list_G(int jml_data,Tmy_kernel *kernel,Tmy_list_alpha *alpha);
	~Tmy_list_G();
	void init();
	void update_G(int idx_b,int idx_a,double delta_1,double delta_2);
	double get_G(int idx);
	vector<int> cari_idx(double rho,vector<double> *gmax_gmin);
	
};

#endif