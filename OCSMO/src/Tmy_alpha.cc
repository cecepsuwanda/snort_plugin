#include "Tmy_alpha.h"


Tmy_alpha::Tmy_alpha(Tconfig *v_config){
   _config = v_config;   
}

Tmy_alpha::~Tmy_alpha(){
   cout << "Hapus my_alpha" << endl;
   delete _my_list_alpha;
}


void Tmy_alpha::init(int jml_data)
{
	double ub = _config->eps/(_config->V * jml_data);	
	_my_list_alpha = new Tmy_list_alpha(jml_data,0,ub);
	_my_list_alpha->init(_config->V,_config->eps);
}