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

time_t tb_experiment::get_id_more_detail_experiment()
{
	return _id_more_detail_experiment;
}

void tb_experiment::insert_experiment(time_t id_experiment_dt, time_t id_detail_experiment_dt, double gamma_awal, double gamma_akhir, double gamma_step, double nu_awal, double nu_akhir, double nu_step)
{
	_id_experiment = time(0);

	string data = to_string(_id_experiment) + "," + to_string(id_experiment_dt) + "," + to_string(id_detail_experiment_dt) + "," + to_string(gamma_awal) + "," + to_string(gamma_akhir) + "," + to_string(gamma_step) + "," + to_string(nu_awal) + "," + to_string(nu_akhir) + "," + to_string(nu_step) + "," + "now()";
	string nm_kolom = "id,id_experiment_dt,id_detail_experiment_dt,gamma_awal,gamma_akhir,gamma_step,nu_awal,nu_akhir,nu_step,mulai";

	global_query_builder.open_connection();
	string sql = "insert into experiment_svm(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::insert_detail_experiment(time_t id_experiment_dt, time_t id_detail_experiment_dt, double gamma, double nu)
{
	_id_detail_experiment = time(0);

	string nm_kolom = "id,id_experiment,id_experiment_dt,id_detail_experiment_dt,gamma,nu,start_train";
	string data = to_string(_id_detail_experiment) + "," + to_string(_id_experiment) + "," + to_string(id_experiment_dt) + "," + to_string(id_detail_experiment_dt) + "," + to_string(gamma) + "," + to_string(nu) + "," + "now()";

	global_query_builder.open_connection();
	string sql = "insert into detail_experiment_svm(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::insert_more_detail_experiment(time_t id_experiment_dt, time_t id_detail_experiment_dt, int no_svm, double gamma, double nu)
{
	_id_more_detail_experiment = time(0);

	string nm_kolom = "id,id_detail_experiment,id_experiment,id_experiment_dt,id_detail_experiment_dt,idx_svm,gamma,nu,start_train";
	string data = to_string(_id_more_detail_experiment) + "," + to_string(_id_detail_experiment) + "," + to_string(_id_experiment) + "," + to_string(id_experiment_dt) + "," + to_string(id_detail_experiment_dt) + "," + to_string(no_svm) + "," + to_string(gamma) + "," + to_string(nu) + "," + "now()";

	global_query_builder.open_connection();
	string sql = "insert into more_detail_experiment_svm(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();

}

bool tb_experiment::cari_detail_experiment(int id_dt_train, int jns_dt_train , int id_dt_test, int jns_dt_test, int depth, int min_sample, int threshold, double credal, time_t &id_detail_experiment)
{
	bool is_exist = false;

	string sql = "select id from detail_experiment where id_dt_train=" + to_string(id_dt_train) + " and jns_dt_train=" + to_string(jns_dt_train) + " and id_dt_test=" + to_string(id_dt_test) + " and jns_dt_test=" + to_string(jns_dt_test) + " and depth=" + to_string(depth) + " and minsample=" + to_string(min_sample) + " and threshold=" + to_string(threshold) + " and credal=" + to_string(credal) + " order by start_train desc limit 1";
	global_query_builder.open_connection();
	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();
			is_exist = jml_row > 0;
			if (is_exist) {
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
	string sql = "update detail_experiment_svm set end_train=now(),start_test=now() where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_test()
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set end_test=now() where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_train_more_detail(time_t id_more_detail_experiment, int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set end_train=now() where id=" + to_string(id_more_detail_experiment) + " and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::start_test_more_detail(int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set start_test=now() where id_detail_experiment=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment) + " and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_test_more_detail(int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set end_test=now() where id_detail_experiment=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment) + " and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl(int FP, int FN, int TP, int TN, float f1)
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set FP=" + to_string(FP) + ",FN=" + to_string(FN) + ",TP=" + to_string(TP) + ",TN=" + to_string(TN) + ",f1=" + to_string(f1) + " where id=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl_more_detail(int FP, int FN, int TP, int TN, float f1,int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set FP=" + to_string(FP) + ",FN=" + to_string(FN) + ",TP=" + to_string(TP) + ",TN=" + to_string(TN) + ",f1=" + to_string(f1) + " where id_detail_experiment=" + to_string(_id_detail_experiment) + " and id_experiment=" + to_string(_id_experiment) + " and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_experiment()
{
	global_query_builder.open_connection();
	string sql = "update experiment_svm set akhir=now() where id=" + to_string(_id_experiment);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

train_test_data tb_experiment::get_train_test_data(time_t id_experiment)
{
	train_test_data data;
	global_query_builder.open_connection();
	string sql = "select * from experiment where id = " + to_string(id_experiment) + " limit 1";

	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			data.jns_dt_train = stoi(tmp[13]);
			data.id_dt_train = stoi(tmp[14]);
			data.partition_train = tmp[15];

			data.jns_dt_test = stoi(tmp[16]);
			data.id_dt_test = stoi(tmp[17]);
			data.partition_test = tmp[18];
		}
	}

	global_query_builder.close_connection();
	return data;
}