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
    jml=jml+1;
  }

  for(int idx=jml;idx<_jml_data;idx++){
     update_alpha(idx,0);
  }
  
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

vector<double> Tmy_list_alpha::calculateBoundaries(int i,int j)
{
  double t      = _alpha[i]+_alpha[j];
  double diff   = t-_ub;
  double diff1  = t+abs(_lb);
  vector<double> hasil = {_lb,_ub};
  if(((_alpha[i]<=_ub) and (_alpha[i]>=_lb)) and ((_alpha[j]<=_ub) and (_alpha[j]>=_lb))){
    hasil = {max(diff,_lb),min(_ub,diff1)}; 
  }
  return hasil;
}

vector<double> Tmy_list_alpha::limit_alpha(double alpha_a,double alpha_b,double Low,double High, int flag)
{
  vector<double> hasil={alpha_a,alpha_b};
  if (alpha_a>High){
     if(flag==1)
     {
        double s=alpha_a-High;
        hasil[1]= alpha_b+s;
     }
     hasil[0]=High; 
  }else{
     if (alpha_a<Low){
      if(flag==1)
      {
        double s=alpha_a-Low;
        hasil[1]= alpha_b+s;
      }
      hasil[0]=Low; 
     } 
  }
  return hasil;
}

vector<double> Tmy_list_alpha::calculateNewAlpha(int i,int j,double delta,double Low,double High)
{
  double alpha_a_new = _alpha[i]+delta;
  vector<double> tmp = limit_alpha(alpha_a_new,0,Low,High,0);
  alpha_a_new = tmp[0];
  double alpha_b_new = _alpha[j]+(_alpha[i]-alpha_a_new);
  tmp = limit_alpha(alpha_b_new,alpha_a_new,_lb,_ub,1);
  alpha_b_new = tmp[0];
  alpha_a_new = tmp[1];
  return {_alpha[i],_alpha[j],alpha_a_new,alpha_b_new};
}

bool Tmy_list_alpha::is_pass(int i,int j,double delta,vector<double> *alpha)
{
  
  if(i==j)
  {    
    alpha->push_back(_alpha[i]);
    alpha->push_back(_alpha[j]);
    alpha->push_back(_alpha[i]);
    alpha->push_back(_alpha[j]);
    return false;
  }else{
    vector<double> tmp=calculateBoundaries(i,j);
    double Low=tmp[0],High=tmp[1];
    if(Low==High){
       alpha->push_back(_alpha[i]);
       alpha->push_back(_alpha[j]);
       alpha->push_back(_alpha[i]);
       alpha->push_back(_alpha[j]);
       return false;
    }else{
       tmp=calculateNewAlpha(i,j,delta,Low,High);
       double alpha_a_old=tmp[0],alpha_b_old=tmp[1],alpha_a_new=tmp[2],alpha_b_new=tmp[3];
       if(abs(alpha_a_new-alpha_a_old)<10e-5)
       {
        alpha->push_back(_alpha[i]);
        alpha->push_back(_alpha[j]);
        alpha->push_back(_alpha[i]);
        alpha->push_back(_alpha[j]);
        return false;
       }else{
          alpha->push_back(alpha_a_old);
          alpha->push_back(alpha_b_old);
          alpha->push_back(alpha_a_new);
          alpha->push_back(alpha_b_new);        
          return true;
       }
    }
  }  
}

double Tmy_list_alpha::get_alpha(int idx)
{
  return _alpha[idx];  
}

vector<int> Tmy_list_alpha::get_list_lb_ub(int flag)
{
  if(flag==0){
     return _alpha_not_lb;
  }else{
    if(flag==1){
       return _alpha_not_ub;
    }  
  }
}

