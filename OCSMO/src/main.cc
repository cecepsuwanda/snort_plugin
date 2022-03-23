#include <iostream>
#include <experimental/filesystem>
#include <chrono>
#include "Tconf_metrix.h"
#include "Tmy_svm.h"

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;
using std::experimental::filesystem::directory_iterator;

int main(int argc, char *argv[])
{
  char *endptr;
  Tconfig config;
  config.f_datatype = argv[2];
  config.path_model = argv[1];
  config.svm_path = config.path_model + "/" + argv[3];

  config.gamma = strtod(argv[4], &endptr);
  config.V = strtod(argv[5], &endptr);

  Tconf_metrix conf_metrix_train;
  conf_metrix_train.add_konversi_asli("known","inside");
  conf_metrix_train.add_konversi_asli("normal","inside");
  conf_metrix_train.add_konversi_asli("unknown","outside");

  Tconf_metrix conf_metrix_test;
  conf_metrix_test.add_konversi_asli("known","inside");
  conf_metrix_test.add_konversi_asli("normal","inside");
  conf_metrix_test.add_konversi_asli("unknown","outside");

  int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {
       string str = file.path().filename();       
       if(str!="train_model_36.csv"){  
        
        auto start = chrono::high_resolution_clock::now();
        chrono::time_point<std::chrono::system_clock> now=chrono::system_clock::now();
        time_t t_c = chrono::system_clock::to_time_t(now);

        
        Tdataframe df_train(&config);
        df_train.by_pass_filter_on();
        df_train.read_data(file.path());
        df_train.read_data_type(config.f_datatype);        
        df_train.info();

        vector<string> list_label = df_train.get_list_label();

        jml = jml + df_train.getjmlrow_svm();
        
        Tmy_svm my_svm(&config);        
        my_svm.train(df_train);

        auto finish = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = finish - start;
        cout << "Elapsed time: " << elapsed.count() << " s\n";

        cout << "Mulai : " << put_time(std::localtime(&t_c), "%F %T.\n");
        now = chrono::system_clock::now(); 
        t_c = chrono::system_clock::to_time_t(now);     
        cout << "Selesai : "<< put_time(std::localtime(&t_c), "%F %T.\n");         

        vector<string> hasil = my_svm.test(df_train);

        df_train.clear_memory();
        df_train.close_file();

        Tconf_metrix conf_metrix;
        conf_metrix.add_konversi_asli("known","inside");
        conf_metrix.add_konversi_asli("normal","inside");
        conf_metrix.add_konversi_asli("unknown","outside"); 

        for (int i = 0; i < hasil.size(); ++i)
         {
           conf_metrix.add_jml(list_label[i],hasil[i],1);
           conf_metrix_train.add_jml(list_label[i],hasil[i],1);           
         }

         conf_metrix.kalkulasi();
         cout << conf_metrix << endl;

         str.replace(0, 5, "test");
         string file_test = config.path_model + "/test/"+ str;

         Tdataframe df_test(&config);
         df_test.by_pass_filter_on();
         df_test.read_data(file_test);
         df_test.read_data_type(config.f_datatype);         
         df_test.info();

         list_label = df_test.get_list_label();
         hasil = my_svm.test(df_test);

         df_test.clear_memory();
         df_test.close_file();
   

         Tconf_metrix conf_metrix1;
         conf_metrix1.add_konversi_asli("known","inside");
         conf_metrix1.add_konversi_asli("normal","inside");
         conf_metrix1.add_konversi_asli("unknown","outside"); 

         for (int i = 0; i < hasil.size(); ++i)
         {
           conf_metrix1.add_jml(list_label[i],hasil[i],1);
           conf_metrix_test.add_jml(list_label[i],hasil[i],1);           
         }

         conf_metrix1.kalkulasi();
         cout << conf_metrix1 << endl;

        
       }   
  }

  cout << "jml = " << jml << endl;

  conf_metrix_train.kalkulasi();
  cout << conf_metrix_train << endl;

  conf_metrix_test.kalkulasi();
  cout << conf_metrix_test << endl;

  return 0;
}
