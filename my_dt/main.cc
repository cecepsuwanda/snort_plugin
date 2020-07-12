
#include <iostream>
#include "Tdec_tree.h"

using namespace std;

int main(int argc, char *argv[])
{
    Tdataframe df_train, df_test, df_save;
    df_train.read_data(argv[3]);
    df_train.read_data_type(argv[2]);

    df_test.read_data(argv[4]);
    df_test.read_data_type(argv[2]);

    cout << "train " << df_train.getjmlrow() << " test " << df_test.getjmlrow() << endl;

    /*df_save.read_data(argv[5]);

    Tdec_tree dec_tree;

    clock_t start, end;
    start = clock();

    dec_tree.train(df_train,0,0,2,stoi(argv[1]));

    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);

    cout << fixed << time_taken << setprecision(5) << endl;

    dec_tree.save_tree(df_save);
    dec_tree.read_tree(df_save);
    dec_tree.test(df_test);*/
    
    return 0;
}
