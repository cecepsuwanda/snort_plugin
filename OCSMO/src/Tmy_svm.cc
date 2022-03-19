#include "Tmy_svm.h"


Tmy_svm::Tmy_svm(Tconfig *v_config){
	_config = v_config;
	_my_alpha = new Tmy_alpha(_config); 
}

Tmy_svm::~Tmy_svm(){
   delete _my_alpha;
   delete _my_kernel;
   delete _my_G;
}

bool Tmy_svm::cari_idx_alpha(vector<int> *idx_alpha)
{
   Tmy_list_G* _my_list_G = _my_G->get_list_G();
   vector<double> gmax_gmin;
   vector<int> tmp_idx_alpha = _my_list_G->cari_idx(_rho,&gmax_gmin);
   
   double diff = abs(gmax_gmin[0]+gmax_gmin[1]);
   cout<<gmax_gmin[0]<<"+"<<gmax_gmin[1]<<"="<<diff<<endl;
   bool stat = true;
   if((diff<1e-3) or ((tmp_idx_alpha[0]==-1) or (tmp_idx_alpha[1]==-1)))
   {  //(diff<1e-3) or
      stat = false;
   }else{
     idx_alpha->push_back(tmp_idx_alpha[0]);
     idx_alpha->push_back(tmp_idx_alpha[1]);
   }  
   
   return stat;
}

bool Tmy_svm::take_step(int idx_b,int idx_a)
{
  if(idx_b==idx_a){
     return false;
  }else{
     Tmy_list_G* _my_list_G = _my_G->get_list_G();
     Tmy_list_alpha* _my_list_alpha = _my_alpha->get_alpha();
     double Fb = _my_list_G->get_G(idx_b)-_rho;
     double Fa = _my_list_G->get_G(idx_a)-_rho;
     //cout <<"Fb "<<Fb<<" "<<"Fa "<<Fa<<endl;

     vector<double> hsl_eta = _my_kernel->hit_eta(idx_b,idx_a);
     
     double delta = (Fa-Fb)*hsl_eta[0];
     //cout<<"delta "<<delta<<endl;
     vector<double> alpha;
     bool is_pass = _my_list_alpha->is_pass(idx_b,idx_a,delta,&alpha);
     if(is_pass==false)
     {
        return false;
     }else{
        _my_alpha->update_alpha(idx_b,alpha[2],idx_a,alpha[3]);
        double delta_alpha_1 = alpha[2]-alpha[0];
        double delta_alpha_2 = alpha[3]-alpha[1];        
        _my_list_G->update_G(idx_b,idx_a,delta_alpha_1,delta_alpha_2);
        _rho = _my_G->update_rho(idx_b,idx_a);
        return true;
     }

  }
}


void Tmy_svm::train(Tdataframe &df){
   _my_kernel = new Tmy_kernel(df,_config->gamma);   
   _my_alpha->init(df.getjmlrow_svm());
   _my_G = new Tmy_G(df.getjmlrow_svm(),_my_kernel,_my_alpha);
   _my_G->init();
   _rho = _my_G->update_rho(0,1);
   
   int max_iter = 10000000; 
   int iter = 0;
   bool is_alpha_changed = true;

   while((is_alpha_changed==true) and (iter<max_iter))
   {
      iter = iter+1;
      vector<int> idx_alpha;
      bool pass = cari_idx_alpha(&idx_alpha);
      if(pass==true)
      {
         cout<<iter<<" sebelum "<<_rho<<","<<idx_alpha[0]<<","<<idx_alpha[1]<<endl;
         is_alpha_changed = take_step(idx_alpha[0],idx_alpha[1]);
         cout<<iter<<" sesudah "<<_rho<<","<<idx_alpha[0]<<","<<idx_alpha[1]<<endl;         
      }else{
         is_alpha_changed = false;
      }
      
   }

}