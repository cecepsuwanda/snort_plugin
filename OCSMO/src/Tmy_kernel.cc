#include "Tmy_kernel.h"


Tmy_kernel::Tmy_kernel(Tdataframe &df,double gamma){
   _df = &df;
   _gamma = gamma;
   _jml_data = _df->getjmlrow_svm();   
}

Tmy_kernel::~Tmy_kernel(){

}

Tmy_double Tmy_kernel::dot(vector<string> x,vector<string> y){
   int jml = x.size();
   Tmy_double sum=0;
   for (int i = 0; i < jml; ++i)
   {
      sum = sum+(double)(stof(x[i])*stof(y[i]));
   }   
   return sum;
}

Tmy_double Tmy_kernel::kernel_function(int i,int j){
  vector<string> x_i = _df->goto_rec(i);
  vector<string> x_j = _df->goto_rec(j);
  double tmp1 = (dot(x_i,x_i)+dot(x_j,x_j)-2*dot(x_i,x_j));
  double tmp2 = (-1*_gamma);
  Tmy_double tmp = exp(tmp2*tmp1);
  return tmp;
}

vector<Tmy_double> Tmy_kernel::get_Q(int i)
{
   vector<Tmy_double> data;
   for (int j = 0; j < _jml_data; ++j)
   {
      Tmy_double tmp_Q = kernel_function(j,i);
      data.push_back(tmp_Q);
   }

   return data;
}

vector<Tmy_double> Tmy_kernel::hit_eta(int i,int j)
{
   Tmy_double k11 = kernel_function(i,i);
   Tmy_double k12 = kernel_function(j,i);
   Tmy_double k22 = kernel_function(j,j);
   Tmy_double p_eta = k11+k22-(2*k12);
   Tmy_double eta = -1;
   if(p_eta!=0)
   {
     eta = 1/p_eta;
   }else{
     eta = 1/1e-12;
   }   
   //cout<<eta<<","<<k11<<","<<k12<<","<<k22<<endl; 
   return {eta,k11,k12,k22};
}