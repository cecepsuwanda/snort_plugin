#include "Tmy_G.h"


Tmy_G::Tmy_G(int jml_data,Tmy_kernel *kernel,Tmy_alpha *alphas){
  _jml_data = jml_data;
  _kernel = kernel;
  _alphas = alphas;

}

Tmy_G::~Tmy_G(){
   delete _my_list_G;
}

void Tmy_G::init()
{
  _my_list_G = new Tmy_list_G(_jml_data,_kernel,_alphas->get_alpha());
}

double Tmy_G::update_rho(int idx_a,int idx_b)
{
  return (_my_list_G->get_G(idx_b)+_my_list_G->get_G(idx_a))/2;
}

Tmy_list_G* Tmy_G::get_list_G()
{
  return _my_list_G; 
}