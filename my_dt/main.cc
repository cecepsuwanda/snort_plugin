
#include <iostream>
#include "Tdec_tree.h"

using namespace std;






int main()
{
    Tdataframe df_train, df_test, df_save;
    df_train.read_data("data/test_icmp_dataset.csv");
    df_train.read_data_type("data/test_icmp.names");

    df_test.read_data("data/train_icmp_dataset.csv");
    df_test.read_data_type("data/test_icmp.names");

    df_save.read_data("data/icmp_dc_tree.csv");

    Tdec_tree dec_tree;
    dec_tree.train(df_train,0,0,2,6);
    dec_tree.save_tree(df_save);
    dec_tree.read_tree(df_save);
    dec_tree.test(df_test);
    
    return 0;
}
