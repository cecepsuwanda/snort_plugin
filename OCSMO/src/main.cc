#include <iostream>
#include <experimental/filesystem>
#include <chrono>
#include "Tconf_metrix.h"
#include "Tmy_svm.h"

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;
using std::experimental::filesystem::directory_iterator;



void baca_data(Tdataframe &df,string sumber_data,string tipe_data)
{
  df.by_pass_filter_on();
  df.read_data(sumber_data);
  df.read_data_type(tipe_data);        
  df.info();
}

void isi_conf_matrix(Tconf_metrix &conf_metrix,vector<string> label,vector<string> prediksi)
{
   conf_metrix.add_konversi_asli("known","inside");
   conf_metrix.add_konversi_asli("normal","inside");
   conf_metrix.add_konversi_asli("unknown","outside");

   for (int i = 0; i < prediksi.size(); ++i)
   {
      conf_metrix.add_jml(label[i],prediksi[i],1);                
   }
}

void cetak_conf_matrix(Tconf_metrix &conf_metrix)
{
  conf_metrix.kalkulasi();
  cout << conf_metrix << endl;
}

time_t ambil_tanggal()
{
  chrono::time_point<std::chrono::system_clock> now=chrono::system_clock::now();
  time_t t_c = chrono::system_clock::to_time_t(now);
  return t_c;
}


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
  Tconf_metrix conf_metrix_test;  

  int jml = 0;

  for (const auto & file : directory_iterator(config.path_model + "/" + argv[6]))
  {
       string str = file.path().filename();       
       if(str=="train_model_1.csv"){  
        
        auto start = chrono::high_resolution_clock::now();        
        time_t tanggal_mulai=ambil_tanggal();
        
        Tdataframe df_train(&config);
        baca_data(df_train,file.path(),config.f_datatype);
        vector<string> list_label = df_train.get_list_label();       
        Tmy_svm my_svm(&config);
                
        my_svm.train(df_train);

        auto finish = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = finish - start;
        cout << "Elapsed time: " << elapsed.count() << " s\n";

        cout << "Mulai : " << put_time(std::localtime(&tanggal_mulai), "%F %T.\n");
        time_t tanggal_selesai=ambil_tanggal();        
        cout << "Selesai : "<< put_time(std::localtime(&tanggal_selesai), "%F %T.\n");         

        vector<string> hasil = my_svm.test(df_train);

        df_train.clear_memory();
        df_train.close_file();

        Tconf_metrix conf_metrix;                
        isi_conf_matrix(conf_metrix,list_label,hasil);
        isi_conf_matrix(conf_metrix_train,list_label,hasil);
        cetak_conf_matrix(conf_metrix);        

         str.replace(0, 5, "test");
         string file_test = config.path_model + "/test/"+ str;

         Tdataframe df_test(&config);
         baca_data(df_test,file_test,config.f_datatype);         
         list_label = df_test.get_list_label();

         hasil = my_svm.test(df_test);

         df_test.clear_memory();
         df_test.close_file();

         Tconf_metrix conf_metrix1;         
         isi_conf_matrix(conf_metrix1,list_label,hasil);
         isi_conf_matrix(conf_metrix_test,list_label,hasil);
         cetak_conf_matrix(conf_metrix1);      
        
       }   
  }
  cetak_conf_matrix(conf_metrix_train);  
  cetak_conf_matrix(conf_metrix_test);

  return 0;
}
