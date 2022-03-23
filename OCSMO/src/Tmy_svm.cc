#include "Tmy_svm.h"


Tmy_svm::Tmy_svm(Tconfig *v_config){
	_config = v_config;
	_my_alpha = new Tmy_alpha(_config); 
}

Tmy_svm::~Tmy_svm(){
   delete _my_alpha;
   delete _my_kernel;
   delete _my_G;
   _model.clear();
}

bool Tmy_svm::cari_idx_alpha(vector<int> *idx_alpha)
{
   Tmy_list_G* _my_list_G = _my_G->get_list_G();
   vector<Tmy_double> gmax_gmin;
   vector<int> tmp_idx_alpha = _my_list_G->cari_idx(_rho,&gmax_gmin);
   
   Tmy_double diff = gmax_gmin[0]+gmax_gmin[1];
   //cout<<gmax_gmin[0]<<"+"<<gmax_gmin[1]<<"="<<diff<<endl;
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

bool Tmy_svm::cari_idx_a_lain(int idx_b,int *idx_alpha)
{
   Tmy_list_G* _my_list_G = _my_G->get_list_G();
   *idx_alpha = _my_list_G->cari_idx_lain(idx_b,_rho);

   return (*idx_alpha!=-1);
}

bool Tmy_svm::take_step(int idx_b,int idx_a)
{
  if(idx_b==idx_a){
     return false;
  }else{
     Tmy_list_G* _my_list_G = _my_G->get_list_G();
     Tmy_list_alpha* _my_list_alpha = _my_alpha->get_alpha();
     Tmy_double Fb = _my_list_G->get_G(idx_b)-_rho;
     Tmy_double Fa = _my_list_G->get_G(idx_a)-_rho;
     //cout <<"Fb "<<Fb<<" "<<"Fa "<<Fa<<endl;

     vector<Tmy_double> hsl_eta = _my_kernel->hit_eta(idx_b,idx_a);
     
     Tmy_double delta = (Fa-Fb)*hsl_eta[0];
     //cout<<"delta "<<delta<<endl;
     vector<Tmy_double> alpha;
     bool is_pass = _my_list_alpha->is_pass(idx_b,idx_a,delta,&alpha);
     if(is_pass==false)
     {
        return false;
     }else{
        _my_alpha->update_alpha(idx_b,alpha[2],idx_a,alpha[3]);
        Tmy_double delta_alpha_1 = alpha[2]-alpha[0];
        Tmy_double delta_alpha_2 = alpha[3]-alpha[1];        
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
      if((iter%100)==0){
       cetak(".");
      }

      iter = iter+1;
      //cetak("iterasi ke - %d \n",iter);
      
      vector<int> idx_alpha;
      bool pass = cari_idx_alpha(&idx_alpha);
      if(pass==true)
      {
         //cout<<"iterasi ke - "<<iter<<" rho sebelum "<<_rho<<" idx_b "<<idx_alpha[0]<<" idx_a "<<idx_alpha[1];
         is_alpha_changed = take_step(idx_alpha[0],idx_alpha[1]);
         //cout<<" rho sesudah "<<_rho<<endl;
         if(is_alpha_changed==false)
         {
            int idx_a;
            pass = cari_idx_a_lain(idx_alpha[0],&idx_a);
            if(pass==true)
            {
              //cout<<"iterasi ke - "<<iter<<" rho sebelum "<<_rho<<" idx_b "<<idx_alpha[0]<<" idx_a "<<idx_a;   
              is_alpha_changed = take_step(idx_alpha[0],idx_a);
              //cout<<" rho sesudah "<<_rho<<endl; 
            }    
         }         
      }else{
         is_alpha_changed = false;
      }
      
   }

   Tmy_list_alpha *list_alpha = _my_alpha->get_alpha();
   map<int,Tmy_double> alpha_sv = list_alpha->get_list_alpha_sv();
   _model.reserve(alpha_sv.size());
   for (map<int, Tmy_double>::iterator it = alpha_sv.begin(); it != alpha_sv.end(); ++it)
   {
      int idx = it->first;
      vector<string> data = df.goto_rec(idx);
      _model.push_back(data);
   }

}

vector<string> Tmy_svm::test(Tdataframe &df)
{
   Tmy_list_alpha *list_alpha = _my_alpha->get_alpha();
   map<int,Tmy_double> alpha_sv = list_alpha->get_list_alpha_sv();
   int jml_data = df.getjmlrow_svm();
   std::vector<string> hasil;
   hasil.reserve(jml_data);
   hasil.assign(jml_data,"inside");
   
   vector<string> tmp_data;
   
   int j=0;
   df.reset_file();
   while (!df.is_eof()) {
      
      tmp_data = df.get_record_svm();

      Tmy_double sum = 0.0;
      int i=0;
      Tmy_double jml_alpha;
      for (map<int, Tmy_double>::iterator it = alpha_sv.begin(); it != alpha_sv.end(); ++it)
      {
          Tmy_double tmp = _my_kernel->kernel_function_f(tmp_data,_model[i]);
          Tmy_double alpha = it->second;                   
          tmp = alpha * tmp;
          sum = sum + tmp;          
          i=i+1;
      }
      
      sum = sum - _rho;
      if(sum>=0.0){

      }else{
         if(sum<0.0){
           hasil[j]="outside";    
         }  
      }

   df.next_record();
   j=j+1;
  }

  return hasil;
}