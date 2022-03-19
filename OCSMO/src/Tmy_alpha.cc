#include "Tmy_alpha.h"


Tmy_alpha::Tmy_alpha(Tconfig *v_config){
   _config = v_config;   
}

Tmy_alpha::~Tmy_alpha(){
   delete _my_list_alpha;
}


void Tmy_alpha::init(int jml_data)
{
	double ub = _config->eps/(_config->V * jml_data);
	ub=bulat_nol(ub,1e-5,10);	
	_my_list_alpha = new Tmy_list_alpha(jml_data,0,ub);
	_my_list_alpha->init(_config->V,_config->eps);
}

void Tmy_alpha::update_alpha(int idx1,double value1,int idx2,double value2)
{
	_my_list_alpha->update_alpha(idx1,value1);
	_my_list_alpha->update_alpha(idx2,value2);
}

Tmy_list_alpha* Tmy_alpha::get_alpha()
{
	return _my_list_alpha; 
}