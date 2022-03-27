#include "Tmy_list_G.h"


Tmy_list_G::Tmy_list_G(int jml_data,Tmy_kernel *kernel,Tmy_list_alpha *alpha)
{
  _jml_data = jml_data;
  _kernel = kernel;
  _alpha = alpha;
  _arr_G.reserve(_jml_data);
  _arr_G.assign(_jml_data,0.0);
  init();
}

Tmy_list_G::~Tmy_list_G()
{
   _arr_G.clear();
}

void Tmy_list_G::init()
{   
   //cetak("Start init G : \n");

   vector<int> idx_alpha = _alpha->get_list_lb_ub(0);
   
   int jml_thread = _jml_data/50;
   int sisa = _jml_data % 50;
   if(sisa!=0)
   {
     jml_thread=jml_thread+1;
   }

   for (int i = 0; i < idx_alpha.size(); ++i)
   {   	 
   	 // if((i%100)==0){
     //   cetak(".");
     //  }
     Tmy_double alpha = _alpha->get_alpha(idx_alpha[i]);
   	 vector<Tmy_double> data = _kernel->get_Q(idx_alpha[i]);     
     for (int j = 0; j < _jml_data; ++j)
     {   	    
   	   _arr_G[j]=_arr_G[j]+(alpha*data[j]);
     }     
   } 
   //cetak("\nEnd init G : \n");

}

void Tmy_list_G::update_G(int idx_b,int idx_a,Tmy_double delta_1,Tmy_double delta_2)
{  
  vector<Tmy_double> data_a = _kernel->get_Q(idx_a);
  vector<Tmy_double> data_b = _kernel->get_Q(idx_b);   
  for (int i = 0; i < _jml_data; ++i)
  {    
   _arr_G[i]=_arr_G[i]+((data_b[i]*delta_1)+(data_a[i]*delta_2));            
  }    
}

Tmy_double Tmy_list_G::get_G(int idx)
{
  return _arr_G[idx];
}

bool Tmy_list_G::is_kkt(int idx,Tmy_double rho)
{
  Tmy_double F = _arr_G[idx]-rho;
  vector<bool> hsl = _alpha->is_alpha_sv(idx);
  bool stat = false;
  if((hsl[1]==true)and(F==0.0))
  {
    stat = true; 
  }else{
     if((hsl[3]==true)and(F>0.0))
     {
       stat = true; 
     }else{
       if((hsl[2]==true)and(F<0.0))
       {
         stat = true; 
       }
     } 
  }

  return stat;  
}

vector<int> Tmy_list_G::cari_idx(Tmy_double rho,vector<Tmy_double> *gmax_gmin)
{
   vector<int> idx_alpha_not_lb = _alpha->get_list_lb_ub(0);
   vector<int> idx_alpha_not_ub = _alpha->get_list_lb_ub(1);

   // cout<<"idx_alpha_not_lb : "<<endl;
   // for (int i = 0; i < idx_alpha_not_lb.size(); ++i)
   // {
   //   cout<<idx_alpha_not_lb[i]<<" ";
   //   if((i%20)==0)
   //   {
   //    cout<<endl;
   //   }
   // }

   // cout<<endl;

   // cout<<"idx_alpha_not_ub : "<<endl;

   // for (int i = 0; i < idx_alpha_not_ub.size(); ++i)
   // {
   //   cout<<idx_alpha_not_ub[i]<<" ";
   //   if((i%20)==0)
   //   {
   //    cout<<endl;
   //   }
   // }
   
   // cout<<endl;

   int idx_b = -1,idx_a=-1;
   Tmy_double gmax = -100,kkt_max=-100,gmin=-100,diff_max=-100,obj_diff_min=100;

   for (int i = 0; i < idx_alpha_not_ub.size(); ++i)
    {
      
         Tmy_double Fb = _arr_G[idx_alpha_not_ub[i]];       
         Tmy_double diff = Fb-rho;
         Tmy_double tmp = -1.0*diff;         
         if (tmp>=gmax)
         {
           gmax = tmp;           
           // if ((is_kkt(idx_alpha_not_ub[i],rho)==false))
           // {
           idx_b = idx_alpha_not_ub[i];
           //_alpha->mv_lb_ub(idx_alpha_not_ub[i],idx_alpha_not_ub.size()-1,1);
           // }            
         }          
      
          //if ((is_kkt(idx_alpha_not_ub[i],rho)==false))
          //{ 
           // tmp = -1.0*(diff-rho);
           //tmp = abs((double)(diff));                  
           // if (tmp>=kkt_max)
           // {
           //   kkt_max = tmp;          
           //   idx_b = idx_alpha_not_ub[i];
           //   _alpha->mv_lb_ub(idx_alpha_not_ub[i],0,1);
             //cout<<"Fb : "<<idx_alpha_not_ub[i]<<" "<<kkt_max<<endl; 
          // }
         //}       
    } 

   if(idx_b!=-1)
   {
      Tmy_double Fb = _arr_G[idx_b];
      for (int i = 0; i < idx_alpha_not_lb.size(); ++i)
      {       
          Tmy_double Fa = _arr_G[idx_alpha_not_lb[i]];
          Tmy_double diff = Fa-rho;                   
          if (diff>=gmin)
          {
            gmin = diff;
          }

          Tmy_double grad_diff = gmax+(Fa-rho);           
          if (grad_diff>0.0)
          {
             Tmy_double obj_diff;
             vector<Tmy_double> tmp_hsl = _kernel->hit_eta(i,idx_b);
             Tmy_double quad_coef = tmp_hsl[0];
             obj_diff = -1.0*(grad_diff*grad_diff)*quad_coef;
             if(obj_diff<=obj_diff_min)
             {
                obj_diff_min=obj_diff;
                idx_a = idx_alpha_not_lb[i];
             }
            
          }

          // if((kkt_max-(Fa-rho))>0.0)
          // { 
          //    diff = kkt_max-(Fa-rho);
             //diff = Fb-Fa;
             //diff = abs((double) diff);
             //cout<<"Fa : "<<idx_alpha_not_lb[i]<<" "<<Fa<<" "<<diff<<endl;             
             // if(diff>=diff_max)
             // {
             //   diff_max=diff;
             //   idx_a = idx_alpha_not_lb[i];
             //   _alpha->mv_lb_ub(idx_alpha_not_lb[i],0,0);
               //cout<<"idx_a "<<idx_a<<" "<<diff_max<<endl;            
             // }
          // }           
     } 
   }

   gmax_gmin->push_back(gmax);
   gmax_gmin->push_back(gmin);

   return {idx_b,idx_a};
}

int Tmy_list_G::cari_idx_lain(int idx_b,Tmy_double rho)
{
   vector<int> idx_alpha_not_lb = _alpha->get_list_lb_ub(0);
   vector<int> idx_alpha_not_ub = _alpha->get_list_lb_ub(1);
      
   int idx_a = -1;

   for (int i = 0; i < idx_alpha_not_lb.size(); ++i)
   {
     Tmy_double Fb = _arr_G[idx_b]-rho;
     Tmy_double Fa = _arr_G[idx_alpha_not_lb[i]]-rho;
     
     vector<Tmy_double> hsl_eta = _kernel->hit_eta(idx_b,idx_alpha_not_lb[i]);
     
     Tmy_double delta = (Fa-Fb)*hsl_eta[0];
     
     vector<Tmy_double> alpha;
     bool is_pass = _alpha->is_pass(idx_b,idx_alpha_not_lb[i],delta,&alpha);
     if(is_pass==true)
     {
       idx_a = idx_alpha_not_lb[i];
       break;
     }

   }

   if(idx_a==-1)
   {
     for (int i = 0; i < idx_alpha_not_ub.size(); ++i)
     {
        Tmy_double Fb = _arr_G[idx_b]-rho;
        Tmy_double Fa = _arr_G[idx_alpha_not_ub[i]]-rho;
     
        vector<Tmy_double> hsl_eta = _kernel->hit_eta(idx_b,idx_alpha_not_ub[i]);
     
        Tmy_double delta = (Fa-Fb)*hsl_eta[0];
     
        vector<Tmy_double> alpha;
        bool is_pass = _alpha->is_pass(idx_b,idx_alpha_not_ub[i],delta,&alpha);
        if(is_pass==true)
        {
          idx_a = idx_alpha_not_ub[i];
          break;
        }
     }
   }


   return idx_a;

}