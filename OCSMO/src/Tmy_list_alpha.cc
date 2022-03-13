#include "Tmy_list_alpha.h"



Tmy_list_alpha::Tmy_list_alpha(int v_jml_data,double v_lb,double v_ub){
   _jml_data = v_jml_data;
   _lb = v_lb;
   _ub = v_ub;
   _jml_alpha=0;
   for(int i=0;i<_jml_data;i++)
   {
     _alpha.push_back(0);
     _alpha_status.push_back(0);
   }
}

Tmy_list_alpha::~Tmy_list_alpha(){
  cout << "Hapus my_list_alpha " << endl;
  _alpha.clear();
  _alpha_status.clear();  
}


void Tmy_list_alpha::init(double V,double eps){
  int jml = (int) (V*_jml_data);
  for(int idx=0;idx<jml;idx++){
  	 update_alpha(idx,_ub);
  }

  if(_jml_alpha<eps)
  {
    update_alpha(jml,1-_jml_alpha);
  }

  cout << "_jml_alpha = " << _jml_alpha << endl;
}

void Tmy_list_alpha::update_alpha(int idx,double value)
{
	if(_alpha[idx]!=0){
      _jml_alpha = _jml_alpha - _alpha[idx];
    } 

	_alpha[idx]=value;    
    
    _jml_alpha = _jml_alpha + _alpha[idx];
	update_alpha_status(idx);
	
}

void Tmy_list_alpha::update_alpha_status(int idx)
{
   if(_alpha[idx]>=_ub)
   {
   	 _alpha_status[idx]=1;
   }else{
   	  if(_alpha[idx]<=_lb)
      {
   	    _alpha_status[idx]=0;
      }else{
   	    _alpha_status[idx]=2;
      }
   } 	
}
