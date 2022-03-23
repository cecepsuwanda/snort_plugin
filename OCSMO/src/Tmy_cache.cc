#include "Tmy_cache.h"



Tmy_cache::Tmy_cache(int jml_data,int size)
{
   _jml_data = jml_data;
   _size = size;
}

Tmy_cache::~Tmy_cache()
{

}

bool Tmy_cache::is_in_head(int idx)
{
  map<int,vector<Tmy_double>>::iterator it;  
  it = _head.find(idx);
  if (it != _head.end())
  {   
    return true;
  }else{
     if(_head.size()==_size){
       _head.erase (_head.begin()); 
     }
    _head[idx].reserve(_jml_data);
    _head[idx].assign(_jml_data,0.0);    
    return false;
  }
}

vector<Tmy_double> Tmy_cache::get_head(int idx)
{
   return _head[idx];
}

void Tmy_cache::isi_head(int idx_map,int idx_vec,Tmy_double val)
{
   _head[idx_map][idx_vec]=val;
}