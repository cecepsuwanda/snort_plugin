#include "Tmy_list_alpha.h"



Tmy_list_alpha::Tmy_list_alpha(int v_jml_data,double v_lb,double v_ub){
   _jml_data = v_jml_data;
   _lb = v_lb;
   _ub = v_ub;
   _jml_alpha=0;
   _jml_alpha_n_sv=0;
   _n_all_sv=0;
   _n_sv=0;
   for(int i=0;i<_jml_data;i++)
   {
     _alpha.push_back(0);
     _alpha_status.push_back(0);
   }
}

Tmy_list_alpha::~Tmy_list_alpha(){
  _alpha.clear();
  _alpha_status.clear();
  _alpha_not_ub.clear();
  _alpha_not_lb.clear();
  _alpha_sv.clear();  
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

  cout << "_ub             = " << _ub << endl;
  cout << "_jml_alpha      = " << _jml_alpha << endl;
  cout << "_jml_alpha_n_sv = " << _jml_alpha_n_sv << endl;
  cout << "_n_all_sv       = " << _n_all_sv << endl;
  cout << "_n_sv           = " << _n_sv << endl;
}

void Tmy_list_alpha::update_alpha(int idx,double value)
{
	if(_alpha[idx]!=0){
      _jml_alpha = _jml_alpha - _alpha[idx];
    }

  vector<bool> hasil = is_alpha_sv(idx);
  
  if(hasil[0]==true){
    if(_n_all_sv!=0){
      _n_all_sv = _n_all_sv-1;
    } 
  }

  if(hasil[1]==true){
    if(_n_sv!=0){
      _jml_alpha_n_sv = _jml_alpha_n_sv - _alpha[idx];    
      _n_sv = _n_sv-1;
    }
  }   

	_alpha[idx]=value;
  _jml_alpha = _jml_alpha + _alpha[idx];
	
  hasil = is_alpha_sv(idx);
  
  if(hasil[0]==true){    
      _n_all_sv = _n_all_sv+1;    
  }

  if(hasil[1]==true){
      _jml_alpha_n_sv = _jml_alpha_n_sv + _alpha[idx];    
      _n_sv = _n_sv+1;    
  }   

  update_alpha_sv(idx);
  update_alpha_status(idx);
  update_lb_ub(idx);
	
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

void Tmy_list_alpha::update_alpha_sv(int idx)
{
  for(int i=0;i<_alpha_sv.size();i++){
    if(_alpha_sv[i]==idx){
      _alpha_sv.erase(_alpha_sv.begin()+i);
      break;
    } 
  }

  vector<bool> hasil = is_alpha_sv(idx);
  if(hasil[0]==true){
     _alpha_sv.push_back(idx);  
  }
}

void Tmy_list_alpha::update_lb_ub(int idx)
{
  for(int i=0;i<_alpha_not_ub.size();i++){
    if(_alpha_not_ub[i]==idx){
      _alpha_not_ub.erase(_alpha_not_ub.begin()+i);
      break;
    } 
  }

  for(int i=0;i<_alpha_not_lb.size();i++){
    if(_alpha_not_lb[i]==idx){
      _alpha_not_lb.erase(_alpha_not_lb.begin()+i);
      break;
    } 
  }

  if (is_upper_bound(idx)==false)
  {
    _alpha_not_ub.push_back(idx);
  }

  if (is_lower_bound(idx)==false)
  {
    _alpha_not_lb.push_back(idx);
  }


}

vector<bool> Tmy_list_alpha::is_alpha_sv(int idx)
{
  vector<bool> tmp;
  tmp.push_back((_alpha[idx]>=_lb) and (_alpha[idx]<=_ub) and (_alpha[idx]!=0));
  tmp.push_back((_alpha[idx]>_lb) and (_alpha[idx]<_ub) and (_alpha[idx]!=0));
  tmp.push_back((_alpha[idx]==_ub));
  tmp.push_back((_alpha[idx]==_lb));
  return tmp;
}

bool Tmy_list_alpha::is_lower_bound(int idx)
{
  return (_alpha_status[idx]==0);
}

bool Tmy_list_alpha::is_upper_bound(int idx)
{
  return (_alpha_status[idx]==1);
}

bool Tmy_list_alpha::is_free(int idx)
{
  return (_alpha_status[idx]==2);
}
