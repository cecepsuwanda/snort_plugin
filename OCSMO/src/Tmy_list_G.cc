#include "Tmy_list_G.h"


Tmy_list_G::Tmy_list_G(int jml_data,Tmy_kernel *kernel,Tmy_list_alpha *alpha)
{
  _jml_data = jml_data;
  _kernel = kernel;
  _alpha = alpha;
  init();
}

Tmy_list_G::~Tmy_list_G()
{
   _arr_G.clear();
}

void Tmy_list_G::init()
{
   
   for (int i = 0; i < _jml_data; ++i)
   {  	 
   	 _arr_G.push_back(0);
   } 
   
   vector<int> idx_alpha = _alpha->get_list_lb_ub(0);
   for (int i = 0; i < idx_alpha.size(); ++i)
   {   	 
   	 double alpha = _alpha->get_alpha(idx_alpha[i]);
   	 vector<double> data = _kernel->get_Q(idx_alpha[i]);
     for (int j = 0; j < _jml_data; ++j)
     {   	    
   	    _arr_G[j]=_arr_G[j]+(alpha*data[j]);   	    
     }     
   }

}

void Tmy_list_G::update_G(int idx_b,int idx_a,double delta_1,double delta_2)
{
  vector<double> data_b = _kernel->get_Q(idx_b);
  vector<double> data_a = _kernel->get_Q(idx_a);
  for (int i = 0; i < _jml_data; ++i)
  {
    _arr_G[i]=_arr_G[i]+((data_b[i]*delta_1)+(data_a[i]*delta_2));
  }
    
}

double Tmy_list_G::get_G(int idx)
{
  return _arr_G[idx];
}

bool Tmy_list_G::is_kkt(int idx,double rho)
{
  double F = _arr_G[idx]-rho;
  vector<bool> hsl = _alpha->is_alpha_sv(idx);
  bool stat = false;
  if((hsl[1]==true)and(F==0))
  {
    stat = true; 
  }else{
     if((hsl[3]==true)and(F>0))
     {
       stat = true; 
     }else{
       if((hsl[2]==true)and(F<0))
       {
         stat = true; 
       }
     } 
  }

  return stat;  
}

vector<int> Tmy_list_G::cari_idx(double rho,vector<double> *gmax_gmin)
{
   vector<int> idx_alpha_not_lb = _alpha->get_list_lb_ub(0);
   vector<int> idx_alpha_not_ub = _alpha->get_list_lb_ub(1);

   int idx_b = -1,idx_a=-1;
   double gmax = -100,gmin=100,diff_max=-100;

   for (int i = 0; i < idx_alpha_not_ub.size(); ++i)
    {
      
       double Fb = abs(_arr_G[idx_alpha_not_ub[i]] - rho);       
       if (Fb>=gmax)
       {
          gmax = Fb;          
          if ((is_kkt(idx_alpha_not_ub[i],rho)==false))
          { 
            idx_b = idx_alpha_not_ub[i];
          }          
       }
       
    } 

   if(idx_b!=-1)
   {
      double Fb = _arr_G[idx_b]-rho;
      for (int i = 0; i < idx_alpha_not_lb.size(); ++i)
      {       
          double Fa = _arr_G[idx_alpha_not_lb[i]] - rho;
          if (abs(Fa)<=gmin)
          {
            gmin = abs(Fa);
          }
          double diff = abs(Fb-Fa);
          if(diff>=diff_max)
          {
            diff_max=diff;
            idx_a = idx_alpha_not_lb[i]; 
          }        
     } 
   }

   gmax_gmin->push_back(gmax);
   gmax_gmin->push_back(gmin);

   return {idx_b,idx_a};
}