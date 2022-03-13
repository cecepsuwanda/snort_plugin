#include "Tmy_kernel.h"


Tmy_kernel::Tmy_kernel(Tdataframe &df,double gamma){
   _df = &df;
   _gamma = gamma;   
}

Tmy_kernel::~Tmy_kernel(){

}

double Tmy_kernel::dot(vector<string> x,vector<string> y){
   int jml = x.size();
   double sum=0;
   for (int i = 0; i < jml; ++i)
   {
      sum = (stof(x[i])*stof(y[i]));
   }
   return sum;
}

double Tmy_kernel::kernel_function(int i,int j){
  vector<string> x_i = _df->goto_rec(i);
  vector<string> x_j = _df->goto_rec(j);
  return exp(-_gamma*dot(x_i,x_i)+dot(x_j,x_j)-2*dot(x_i,x_j));
}