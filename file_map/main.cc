
#include <iostream>
#include "Tdataframe.h"

using namespace std;

//#define pagesize  4096

int main()
{
   
  /* int ukuran_array = 100000; 
   int pengali = 1;
   if( (ukuran_array*sizeof(int)) > pagesize )
   {
     //pengali = ((ukuran_array*sizeof(int))/pagesize)+1;
   }

   uint8_t *_idx_in_memory = (uint8_t*) mmap(NULL, pengali*pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

   if(_idx_in_memory == MAP_FAILED)
   {
      cout << "gagal" << endl;
   }

   
   for (int i = 0; i < ukuran_array; ++i)
   {
     _idx_in_memory[i]= i;
   }
     

   //cout << no[_idx_in_memory[1]] << endl;

   munmap(_idx_in_memory, pagesize); */

    Tdataframe df_train;
    df_train.read_data("../my_dt/data/test_full_correctd.csv");
    df_train.read_data_type("../my_dt/data/kddcup.names");

    cout << df_train.getjmlrow() << endl;

    field_filter filter;
    filter.idx_opt =2;
    filter.value ="icmp";    

    df_train.add_filter(1,filter);
    
    cout << df_train.getjmlrow() << endl;

    filter.value ="ecr_i";    

    df_train.add_filter(2,filter);
    
    cout << df_train.getjmlrow() << endl;

    df_train.stat_tabel();
    cout << df_train.getjmlrow() << endl;

    df_train.stat_tabel();
    cout << df_train.getjmlrow() << endl;

    return 0;
}
