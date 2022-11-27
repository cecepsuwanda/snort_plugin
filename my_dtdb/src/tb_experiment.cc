#include "tb_experiment.h"



tb_experiment::tb_experiment()
{

}

tb_experiment::~tb_experiment()
{

}

time_t tb_experiment::get_id_experiment()
{
	return _id_experiment;
}

time_t tb_experiment::get_id_detail_experiment()
{
	return _id_detail_experiment;
}

void tb_experiment::insert_experiment(int depth_awal, int depth_akhir, int depth_step, int min_sample_awal, int min_sample_akhir, int min_sample_step, int threshold_awal, int threshold_akhir, int threshold_step, double credal_s_awal, double credal_s_akhir, double credal_s_step, int id_dt_train, int jns_dt_train, string partition_train, int id_dt_test, int jns_dt_test, string partition_test)
{
	_id_experiment = time(0);

	string data = to_string(_id_experiment) + "," + to_string(depth_awal) + "," + to_string(depth_akhir) + "," + to_string(depth_step) + "," + to_string(min_sample_awal) + "," + to_string(min_sample_akhir) + "," + to_string(min_sample_step) + "," + to_string(threshold_awal) + "," + to_string(threshold_akhir) + "," + to_string(threshold_step) + "," + to_string(credal_s_awal) + "," + to_string(credal_s_akhir) + "," + to_string(credal_s_step) + "," + to_string(id_dt_train) + "," + to_string(jns_dt_train) + "," + "'" + partition_train + "'" + "," + to_string(id_dt_test) + "," + to_string(jns_dt_test) + "," + "'" + partition_test + "'" + "," + "now()";
	string nm_kolom = "id,depth_awal,depth_akhir,depth_step,min_sample_awal,min_sample_akhir,min_sample_step,threshold_awal,threshold_akhir,threshold_step,credal_awal,credal_akhir,credal_step,id_dt_train,jns_dt_train,partition_train,id_dt_test,jns_dt_test,partition_test,mulai";

	global_query_builder.open_connection();
	string sql = "insert into experiment(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::insert_detail_experiment(int id_dt_train, int jns_dt_train , int id_dt_test, int jns_dt_test, int depth, int min_sample, int threshold, int credal)
{
	_id_detail_experiment = time(0);

	string nm_kolom = "id,id_experiment,id_dt_train,jns_dt_train,id_dt_test,jns_dt_test,depth,minsample,threshold,credal,start_train";
	string data = to_string(_id_detail_experiment) + "," + to_string(_id_experiment) + "," + to_string(id_dt_train) + "," + to_string(jns_dt_train) + "," + to_string(id_dt_test) + "," + to_string(jns_dt_test)  + "," + to_string(depth) + "," + to_string(min_sample) + "," + to_string(threshold) + "," + to_string(credal) + "," + "now()";

	global_query_builder.open_connection();
	string sql = "insert into detail_experiment(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

bool tb_experiment::cari_detail_experiment(int id_dt_train, int jns_dt_train , int id_dt_test, int jns_dt_test, int depth, int min_sample, int threshold, int credal, time_t &id_detail_experiment)
{
	bool is_exist = false;

	string sql = "select id from detail_experiment where id_dt_train=" + to_string(id_dt_train) + " and jns_dt_train=" + to_string(jns_dt_train) + " and id_dt_test=" + to_string(id_dt_test) + " and jns_dt_test=" + to_string(jns_dt_test) + " and depth=" + to_string(depth) + " and minsample=" + to_string(min_sample) + " and threshold=" + to_string(threshold) + " and credal=" + to_string(credal);
	global_query_builder.open_connection();
	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();
			is_exist=jml_row>0;
            if(is_exist){   
			  vector<string> tmp = global_query_builder.fetch_row();
			  id_detail_experiment = stoi(tmp[0]);
			}  
		}
	}


	global_query_builder.close_connection();
	return is_exist;
}

void tb_experiment::end_train_start_test()
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment set end_train=now(),start_test=now() where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_test()
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment set end_test=now() where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl(int FP, int FN, int TP, int TN, float f1)
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment set FP=" + to_string(FP) + ",FN=" + to_string(FN) + ",TP=" + to_string(TP) + ",TN=" + to_string(TN) + ",f1=" + to_string(f1) + " where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_experiment()
{
	global_query_builder.open_connection();
	string sql = "update experiment set akhir=now() where id=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}