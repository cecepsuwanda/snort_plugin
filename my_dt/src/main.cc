
#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *endptr;
    Tconfig config;

    config.f_datatype = argv[8];
    config.path_model = argv[10];

    config.use_credal = stoi(argv[4]) == 1;
    config.credal_s = strtod(argv[5], &endptr);
    config.limited = stoi(argv[6]) == 1;
    config.threshold = stoi(argv[7]);

    config.depth = stoi(argv[2]);
    config.min_sample = stoi(argv[3]);

    for (int i = 4; i < 101; ++i)
    {
        

        config.depth = i;
      for (int j = 200; j < 201; j+=2)
        {
            
          config.min_sample = j;         

        // if (stoi(argv[1]) == 0)
        // {
        config.f_train = argv[9];
        config.prunning = true;
        Tdt_build dec_tree_build(&config);
        string tmp_str = config.path_model + "/dtsvm_model.csv";
        remove(tmp_str.c_str());
        dec_tree_build.build_tree();
        // } else {

        //     if (stoi(argv[1]) == 1)
        //     {
        config.f_test = argv[9];
        Tdec_tree dec_tree_test(&config);
        dec_tree_test.read_tree();
        dec_tree_test.test();
        //     }
        // }
       }
    }

    return 0;
}
