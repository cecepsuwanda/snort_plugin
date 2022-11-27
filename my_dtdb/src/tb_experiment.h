#include <string>
#include <vector>
#include <ctime>
#include "Tquery_builder.h"

using namespace std;


#ifndef Included_tb_experiment_H

#define Included_tb_experiment_H

class tb_experiment
{
private:
	Tquery_builder global_query_builder;
    time_t _id_experiment;
    time_t _id_detail_experiment;


public:
	tb_experiment();
	~tb_experiment();

	void insert_experiment(int depth_awal,int depth_akhir,int depth_step,int min_sample_awal,int min_sample_akhir,int min_sample_step,int threshold_awal, int threshold_akhir, int threshold_step, double credal_s_awal,double credal_s_akhir,double credal_s_step,int id_dt_train,int jns_dt_train,string partition_train,int id_dt_test,int jns_dt_test,string partition_test);
    void insert_detail_experiment(int id_dt_train,int jns_dt_train ,int id_dt_test,int jns_dt_test,int depth,int min_sample,int threshold,int credal);
    bool cari_detail_experiment(int id_dt_train,int jns_dt_train ,int id_dt_test,int jns_dt_test,int depth,int min_sample,int threshold,int credal,time_t &id_detail_experiment);       
    
    void end_train_start_test(); 
    void end_test();
    void hsl(int FP,int FN,int TP,int TN,float f1); 
    void end_experiment();

    time_t get_id_experiment();
    time_t get_id_detail_experiment();
};

#endif