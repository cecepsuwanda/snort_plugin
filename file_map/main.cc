
#include <iostream>
#include "Tdataframe.h"

using namespace std;


int main()
{
   
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
