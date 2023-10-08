#include <string>
#include <vector>
#include <ctime>
#include "Tquery_builder.h"

using namespace std;


#ifndef Included_tb_experiment_H

#define Included_tb_experiment_H

struct train_test_data
{
    int jns_dt_train = -1;
    int id_dt_train = -1;
    string partition_train = "";

    int jns_dt_test = -1;
    int id_dt_test = -1;
    string partition_test = "";
};

class tb_experiment
{
private:
    Tquery_builder global_query_builder;
    
    static time_t _id_experiment;
    static time_t _id_detail_experiment;

    time_t _id_more_detail_experiment;
    int _no_svm; 
    double _gamma; 
    double _nu;

public:
    tb_experiment();
    ~tb_experiment();

    void insert_experiment(time_t id_experiment_dt,time_t id_detail_experiment_dt, double gamma_awal, double gamma_akhir, double gamma_step, double nu_awal, double nu_akhir, double nu_step);
    void insert_detail_experiment(time_t id_experiment_dt,time_t id_detail_experiment_dt, double gamma,double nu);
    void insert_more_detail_experiment(time_t id_experiment_dt,time_t id_detail_experiment_dt,int no_svm, double gamma,double nu);
    
    bool cari_detail_experiment(double gamma,double nu, time_t &id_detail_experiment, time_t &id_experiment, time_t &id_detail_experiment_dt, time_t &id_experiment_dt);

    void end_train_start_test();
    void end_test();
    void end_train_more_detail();
    void start_test_more_detail(int no_svm);
    void end_test_more_detail(int no_svm);

 
    void hsl_svm(int FP, int FN, int TP, int TN, float f1);
    void hsl(int FP, int FN, int TP, int TN, float f1);
    void hsl_more_detail(int FP, int FN, int TP, int TN, float f1,int no_svm);
    void end_experiment();

    time_t get_id_experiment();
    time_t get_id_detail_experiment();
    time_t get_id_more_detail_experiment(int no_svm);

    train_test_data get_train_test_data(time_t id_experiment);
};

#endif