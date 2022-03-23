#include "Tmy_kernel.h"


Tmy_kernel::Tmy_kernel(Tdataframe &df,double gamma){
   _df = &df;
   _gamma = gamma;
   _jml_data = _df->getjmlrow_svm();

   _cache = new Tmy_cache(_jml_data,1000);   
}

Tmy_kernel::~Tmy_kernel(){

}

Tmy_double Tmy_kernel::dot(vector<string> x,vector<string> y){
   int jml = x.size();
   Tmy_double sum=0;
   for (int i = 0; i < jml; ++i)
   {
      sum = sum+(stod(x[i])*stod(y[i]));
   }   
   return sum;
}

Tmy_double Tmy_kernel::kernel_function(int i,int j){
  vector<string> x_i = _df->goto_rec(i);
  vector<string> x_j = _df->goto_rec(j);
  double tmp1 = (dot(x_i,x_i)+dot(x_j,x_j)-2.0*dot(x_i,x_j));
  double tmp2 = (-1.0*_gamma);
  Tmy_double tmp = exp(tmp2*tmp1);
  return tmp;
}

Treturn_data Tmy_kernel::thread_hit_data(int idx_map,int idx_vec,vector<string> x,vector<string> y,double gamma)
{
  int jml = x.size();
   
   double sum1 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum1=sum1+(stod(y[i])*stod(y[i]));
   }

   double sum2 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum2=sum2+(stod(x[i])*stod(x[i]));
   }
   
   double sum3 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum3=sum3+(stod(x[i])*stod(y[i]));
   }
      
   double tmp = -1.0*gamma * (sum1+sum2-2.0*sum3);
   Tmy_double hasil = exp(tmp);
   return {idx_map,idx_vec,hasil};
}

vector<Tmy_double> Tmy_kernel::get_Q(int i)
{   
   bool is_pass = _cache->is_in_head(i);   
   if(is_pass==false){
     vector<string> x_j = _df->goto_rec(i);        
     vector<future<Treturn_data>> async_worker;
     for (int j = 0; j < _jml_data; ++j)
     { 
       //cetak("*");
       vector<string> x_i = _df->goto_rec(j);

       async_worker.push_back(async(std::launch::async, &Tmy_kernel::thread_hit_data,i,j,x_i,x_j,_gamma));
        
       if ((async_worker.size() % 10 )==0)
       {
          Treturn_data hsl;
          for (future<Treturn_data> & th : async_worker)
          {
            hsl = th.get();
            _cache->isi_head(hsl.idx_map,hsl.idx_vec,hsl.data);
          }

          async_worker.clear();
          async_worker.shrink_to_fit();  
       }

       //_cache->isi_head(i,j,kernel_function(j,i)); 
     }

     if (async_worker.size() > 0)
     {
          Treturn_data hsl;
          for (future<Treturn_data> & th : async_worker)
          {
            hsl = th.get();
            _cache->isi_head(hsl.idx_map,hsl.idx_vec,hsl.data);
          }

          async_worker.clear();
          async_worker.shrink_to_fit();  
     }
       
   }   
   vector<Tmy_double> data = _cache->get_head(i);
   return data;
}

vector<Tmy_double> Tmy_kernel::hit_eta(int i,int j)
{
   Tmy_double k11 = kernel_function(i,i);
   Tmy_double k12 = kernel_function(j,i);
   Tmy_double k22 = kernel_function(j,j);
   Tmy_double p_eta = k11+k22-(2.0*k12);
   Tmy_double eta = -1.0;
   if(p_eta!=0.0)
   {
     eta = 1.0/p_eta;
   }else{
     eta = 1.0/1e-12;
   }   
   //cout<<eta<<","<<k11<<","<<k12<<","<<k22<<endl; 
   return {eta,k11,k12,k22};
}

Tmy_double Tmy_kernel::kernel_function_f(vector<string> x,vector<string> y)
{
   
   int jml = y.size();
   
   double sum1 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum1=sum1+(stod(y[i])*stod(y[i]));
   }

   double sum2 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum2=sum2+(stod(x[i])*stod(x[i]));
   }
   
   double sum3 = 0.0; 
   for (int i = 0; i < jml; ++i)
   {
     sum3=sum3+(stod(x[i])*stod(y[i]));
   }
      

   double tmp = -1.0*_gamma * (sum1+sum2-2.0*sum3);
   Tmy_double hasil = exp(tmp);     
   return hasil;
}