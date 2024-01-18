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

time_t tb_experiment::get_id_more_detail_experiment(int no_svm)
{
	time_t id_more_detail_experiment = 0;

	string sql = "select id from more_detail_experiment_svm where id_detail_experiment='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "' and idx_svm=" + to_string(no_svm);
	global_query_builder.open_connection();
	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();
			bool is_exist = jml_row > 0;
			if (is_exist) {
				vector<string> tmp = global_query_builder.fetch_row();
				id_more_detail_experiment = (time_t) atoll(tmp[0].c_str());
			}
		}
	}
	global_query_builder.close_connection();

	return id_more_detail_experiment;
}

void tb_experiment::insert_experiment(time_t id_experiment_dt, time_t id_detail_experiment_dt, double gamma_awal, double gamma_akhir, double gamma_step, double nu_awal, double nu_akhir, double nu_step)
{
	_id_experiment = time(0);

	string data = "'" + to_string(_id_experiment) + "','" + to_string(id_experiment_dt) + "','" + to_string(id_detail_experiment_dt) + "'," + to_string(gamma_awal) + "," + to_string(gamma_akhir) + "," + to_string(gamma_step) + "," + to_string(nu_awal) + "," + to_string(nu_akhir) + "," + to_string(nu_step) + "," + "now()";
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
	string data = "'" + to_string(_id_detail_experiment) + "','" + to_string(_id_experiment) + "','" + to_string(id_experiment_dt) + "','" + to_string(id_detail_experiment_dt) + "'," + to_string(gamma) + "," + to_string(nu) + "," + "now()";

	global_query_builder.open_connection();
	string sql = "insert into detail_experiment_svm(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::insert_more_detail_experiment(time_t id_experiment_dt, time_t id_detail_experiment_dt, int no_svm, double gamma, double nu)
{
	_id_more_detail_experiment = time(0);
	_no_svm = no_svm;
	_gamma = gamma;
	_nu = nu;

	string nm_kolom = "id,id_detail_experiment,id_experiment,id_experiment_dt,id_detail_experiment_dt,idx_svm,gamma,nu,start_train";
	string data = "'" + to_string(_id_more_detail_experiment) + "','" + to_string(_id_detail_experiment) + "','" + to_string(_id_experiment) + "','" + to_string(id_experiment_dt) + "','" + to_string(id_detail_experiment_dt) + "'," + to_string(_no_svm) + "," + to_string(_gamma) + "," + to_string(_nu) + "," + "now()";

	global_query_builder.open_connection();
	string sql = "insert into more_detail_experiment_svm(" + nm_kolom + ") values(" + data + ")";
	global_query_builder.query(sql);
	global_query_builder.close_connection();

}

bool tb_experiment::cari_detail_experiment(double gamma, double nu, time_t &id_detail_experiment, time_t &id_experiment, time_t &id_detail_experiment_dt, time_t &id_experiment_dt)
{

	bool is_exist = false;

	global_query_builder.open_connection();

	string sql = "select id_dt_train,jns_dt_train,depth,minsample,threshold,credal from detail_experiment_svm a inner join detail_experiment b on a.id_experiment_dt = b.id_experiment and a.id_detail_experiment_dt=b.id where a.id='" + to_string(_id_detail_experiment) + "' and a.id_experiment='" + to_string(_id_experiment) + "' limit 1";

	int id_dt_train = -1, jns_dt_train = -1, depth = -1, min_sample = -1;
	double credal = 0.0, threshold = 0.0;

	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			char *endptr;
			vector<string> tmp = global_query_builder.fetch_row();
			id_dt_train = stoi(tmp[0]);
			jns_dt_train = stoi(tmp[1]);
			depth = stoi(tmp[2]);
			min_sample = stoi(tmp[3]);
			threshold = strtod(tmp[4].c_str(), &endptr);
			credal = strtod(tmp[5].c_str(), &endptr);
		}
	}


	sql = "select a.id,a.id_experiment,a.id_experiment_dt,a.id_detail_experiment_dt from detail_experiment_svm a inner join detail_experiment b on a.id_experiment_dt = b.id_experiment and a.id_detail_experiment_dt=b.id where a.id<>'" + to_string(_id_detail_experiment) + "' and a.id_experiment<>'" + to_string(_id_experiment) + "' and id_dt_train=" + to_string(id_dt_train) + " and jns_dt_train=" + to_string(jns_dt_train) + " and depth=" + to_string(depth) + " and minsample=" + to_string(min_sample) + " and threshold=" + to_string(threshold) + " and credal=" + to_string(credal) + " and gamma=" + to_string(gamma) + " and nu=" + to_string(nu) + " order by a.start_train desc limit 1";

	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();
			is_exist = jml_row > 0;
			if (is_exist) {
				vector<string> tmp = global_query_builder.fetch_row();
				id_detail_experiment = stoi(tmp[0]);
				id_experiment = stoi(tmp[1]);
				id_detail_experiment_dt = stoi(tmp[3]);
				id_experiment_dt = stoi(tmp[2]);
			}
		}
	}

	global_query_builder.close_connection();

	return is_exist;
}

void tb_experiment::end_train_start_test()
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set end_train=now(),start_test=now() where id='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "'";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_test()
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set end_test=now() where id='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "'";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_train_more_detail()
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set end_train=now() where id='" + to_string(_id_more_detail_experiment) + "' and idx_svm=" + to_string(_no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::start_test_more_detail(int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set start_test=now() where id_detail_experiment='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "' and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_test_more_detail(int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set end_test=now() where id_detail_experiment='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "' and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl_svm(int FP, int FN, int TP, int TN, float f1)
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set FP_svm=" + to_string(FP) + ",FN_svm=" + to_string(FN) + ",TP_svm=" + to_string(TP) + ",TN_svm=" + to_string(TN) + ",f1_svm=" + to_string(f1) + " where id='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "'";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl(int FP, int FN, int TP, int TN, float f1)
{
	global_query_builder.open_connection();
	string sql = "update detail_experiment_svm set FP=" + to_string(FP) + ",FN=" + to_string(FN) + ",TP=" + to_string(TP) + ",TN=" + to_string(TN) + ",f1=" + to_string(f1) + " where id='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "'";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::hsl_more_detail(int FP, int FN, int TP, int TN, float f1, int no_svm)
{
	global_query_builder.open_connection();
	string sql = "update more_detail_experiment_svm set FP=" + to_string(FP) + ",FN=" + to_string(FN) + ",TP=" + to_string(TP) + ",TN=" + to_string(TN) + ",f1=" + to_string(f1) + " where id_detail_experiment='" + to_string(_id_detail_experiment) + "' and id_experiment='" + to_string(_id_experiment) + "' and idx_svm=" + to_string(no_svm);
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

void tb_experiment::end_experiment()
{
	global_query_builder.open_connection();
	string sql = "update experiment_svm set akhir=now() where id='" + to_string(_id_experiment) + "'";
	global_query_builder.query(sql);
	global_query_builder.close_connection();
}

train_test_data tb_experiment::get_train_test_data_old(time_t id_experiment)
{
	train_test_data data;
	global_query_builder.open_connection();
	string sql = "select * from experiment where id = '" + to_string(id_experiment) + "' limit 1";

	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			data.id_dt_train = stoi(tmp[13]);
			data.jns_dt_train = stoi(tmp[14]);
			data.partition_train = tmp[15];

			data.id_dt_test = stoi(tmp[16]);
			data.jns_dt_test = stoi(tmp[17]);
			data.partition_test = tmp[18];
		}
	}

	global_query_builder.close_connection();
	return data;
}

train_test_data tb_experiment::get_train_test_data(int id_dt_train,int jns_dt_train,string partition_train,int id_dt_test,int jns_dt_test,string partition_test,int depth,int sample,double threshold,double credal)
{
	train_test_data data;
	
    data.id_dt_train = id_dt_train;
	data.jns_dt_train = jns_dt_train;
	data.partition_train = partition_train;
    
	data.id_dt_test = id_dt_test;
	data.jns_dt_test = jns_dt_test;
	data.partition_test = partition_test;

	global_query_builder.open_connection();
	string sql = "select * from detail_experiment where (id_dt_train="+to_string(id_dt_train)+" and jns_dt_train="+to_string(jns_dt_train)+") and (id_dt_test="+to_string(id_dt_test)+" and jns_dt_test="+to_string(jns_dt_test)+") and (depth="+to_string(depth)+" and minsample="+to_string(sample)+" and threshold="+to_string(threshold)+" and credal="+to_string(credal)+") limit 1";

	if (global_query_builder.query(sql))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			data.id_experiment_dt=(time_t) atoll(tmp[1].c_str());
            data.id_detail_experiment_dt=(time_t) atoll(tmp[0].c_str());
		}
	}

	global_query_builder.close_connection();
	return data;
}