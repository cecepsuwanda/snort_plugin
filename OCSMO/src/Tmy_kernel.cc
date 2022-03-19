#include "Tmy_kernel.h"


Tmy_kernel::Tmy_kernel(Tdataframe &df,double gamma){
   _df = &df;
   _gamma = gamma;
   _jml_data = _df->getjmlrow_svm();   
}

Tmy_kernel::~Tmy_kernel(){

}

double Tmy_kernel::dot(vector<string> x,vector<string> y){
   int jml = x.size();
   double sum=0;
   for (int i = 0; i < jml; ++i)
   {
      sum = sum+(stof(x[i])*stof(y[i]));
   }
   return sum;
}

double Tmy_kernel::kernel_function(int i,int j){
  vector<string> x_i = _df->goto_rec(i);
  vector<string> x_j = _df->goto_rec(j);
  double tmp = exp(-_gamma*(dot(x_i,x_i)+dot(x_j,x_j)-2*dot(x_i,x_j)));
  return bulat_nol(tmp,1e-5,10);
}

vector<double> Tmy_kernel::get_Q(int i)
{
   vector<double> data;
   for (int j = 0; j < _jml_data; ++j)
   {
      data.push_back(kernel_function(j,i));
   }

   return data;
}

vector<double> Tmy_kernel::hit_eta(int i,int j)
{
   double k11 = kernel_function(i,i);
   double k12 = kernel_function(j,i);
   double k22 = kernel_function(j,j);
   double p_eta = k11+k22-(2*k12);
   double eta = -1;
   if(p_eta!=0)
   {
     eta = 1/p_eta;
   }else{
     eta = 1/1e-12;
   }
   //cout<<eta<<","<<k11<<","<<k12<<","<<k22<<endl; 
   return {eta,k11,k12,k22};
}