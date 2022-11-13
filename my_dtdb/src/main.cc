#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"
#include <ctime>

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;

int main(int argc, const char **argv)
{
	char *endptr;
	Tconfig config;

	config.f_datatype = argv[13];
	config.path_model = argv[16];

	double credal_s_awal  = strtod(argv[7], &endptr);
	double credal_s_akhir = strtod(argv[8], &endptr);
	double credal_s_step  = strtod(argv[9], &endptr);

	int depth_awal = stoi(argv[1]);
	int depth_akhir = stoi(argv[2]);
	int depth_step = stoi(argv[3]);

	int min_sample_awal  = stoi(argv[4]);
	int min_sample_akhir = stoi(argv[5]);
	int min_sample_step  = stoi(argv[6]);

	int threshold_awal  = stoi(argv[10]);
	int threshold_akhir = stoi(argv[11]);
	int threshold_step  = stoi(argv[12]);

	config.f_train = argv[14];
	config.f_test = argv[15];

	config.id_dt_train = 2;
	config.jns_dt_train = 1;
	config.partition_train = "p21";


	config.id_dt_test = 2;
	config.jns_dt_test = 1;
	config.partition_test = "p21";

	config.search_uniqe_val = false;

	Twrite_file tb_experiment;
	tb_experiment.setnm_f("experiment");

	Twrite_file tb_detail_experiment;
	tb_detail_experiment.setnm_f("detail_experiment");

	time_t id_experiment = time(0);

	string data = to_string(id_experiment) + "," + to_string(depth_awal) + "," + to_string(depth_akhir) + "," + to_string(depth_step) + "," + to_string(min_sample_awal) + "," + to_string(min_sample_akhir) + "," + to_string(min_sample_step) + "," + to_string(threshold_awal) + "," + to_string(threshold_akhir) + "," + to_string(threshold_step) + "," + to_string(credal_s_awal) + "," + to_string(credal_s_akhir) + "," + to_string(credal_s_step) + "," + to_string(config.id_dt_train) + "," + to_string(config.jns_dt_train) + "," + "'" + config.partition_train + "'" + "," + to_string(config.id_dt_test) + "," + to_string(config.jns_dt_test) + "," + "'" + config.partition_test + "'" + "," + "now()";
	string nm_kolom = "id,depth_awal,depth_akhir,depth_step,min_sample_awal,min_sample_akhir,min_sample_step,threshold_awal,threshold_akhir,threshold_step,credal_awal,credal_akhir,credal_step,id_dt_train,jns_dt_train,partition_train,id_dt_test,jns_dt_test,partition_test,mulai";
	tb_experiment.write_file(nm_kolom, data);

	for (int l = threshold_awal; l <= threshold_akhir; l += threshold_step)
	{
		config.limited = l != 0;
		config.threshold = l;
		for (int i = min_sample_awal; i <= min_sample_akhir; i += min_sample_step)
		{
			config.min_sample = i;
			for (double k = credal_s_awal; k <= credal_s_akhir; k += credal_s_step)
			{
				config.use_credal = k != 0.0;
				config.credal_s = k;

				int prev_jml_FP = 0;
				int prev_jml_FN = 0;
				int jml_sama = 0;

				for (int j = depth_awal; j <= depth_akhir; j += depth_step)
				{
					config.depth = j;
					config.prunning = true;

					time_t id_detail_experiment = time(0);

					nm_kolom = "id,id_experiment,id_dt_train,jns_dt_train,id_dt_test,jns_dt_test,depth,minsample,threshold,credal,start_train";
					data = to_string(id_detail_experiment) + "," + to_string(id_experiment) + "," + to_string(config.id_dt_train) + "," + to_string(config.jns_dt_train) + "," + to_string(config.id_dt_test) + "," + to_string(config.jns_dt_test)  + "," + to_string(config.depth) + "," + to_string(config.min_sample) + "," + to_string(config.threshold) + "," + to_string(config.use_credal) + "," + "now()";
					tb_detail_experiment.write_file(nm_kolom, data);

					Tdt_build dec_tree_build(&config);

					bool file_exist = false;
					{
						Tread_file rf;
						rf.setnm_f("tree", config.id_dt_train, config.jns_dt_train, config.partition_train);
						string sql = "and depth=" + to_string(config.depth) + " and minsample=" + to_string(config.min_sample) + " and threshold=" + to_string(config.threshold) + " and credal=" + to_string(config.credal_s);
						rf.filter(sql, false);
						if (rf.get_jml_row() > 0)
						{
							rf.delete_data(sql);
						}

						sql = "and depth=" + to_string(config.depth - 1) + " and minsample=" + to_string(config.min_sample) + " and threshold=" + to_string(config.threshold) + " and credal=" + to_string(config.credal_s);
						rf.filter(sql, false);
						file_exist = rf.get_jml_row() > 0;
						rf.close_file();
					}

					if (file_exist) //(j > depth_awal) and
					{
						dec_tree_build.read_tree();
						dec_tree_build.build_from_prev_tree( j - 1);
					} else {
						dec_tree_build.build_tree();
					}

					tb_detail_experiment.update_file("end_train", "now()", "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));
					tb_detail_experiment.update_file("start_test", "now()", "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));

					Tdec_tree dec_tree_test(&config);
					dec_tree_test.read_tree();
					Tconf_metrix dt_conf_metrix;
					dec_tree_test.test(dt_conf_metrix);

					tb_detail_experiment.update_file("end_test", "now()", "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));


					int jml_FN = dt_conf_metrix.get_FN("known");
					int jml_FP = dt_conf_metrix.get_FP("known");
					int jml_TN = dt_conf_metrix.get_TN("known");
					int jml_TP = dt_conf_metrix.get_TP("known");
					float f1 = dt_conf_metrix.get_F1();

					tb_detail_experiment.update_file("FN", to_string(jml_FN), "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));
					tb_detail_experiment.update_file("FP", to_string(jml_FP), "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));
					tb_detail_experiment.update_file("TN", to_string(jml_TN), "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));
					tb_detail_experiment.update_file("TP", to_string(jml_TP), "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));
					tb_detail_experiment.update_file("f1", to_string(f1), "id=" + to_string(id_detail_experiment) + " and id_experiment=" + to_string(id_experiment));

					jml_sama++;
					if ((prev_jml_FN != jml_FN) or (prev_jml_FP != jml_FP))
					{
						jml_sama = 0;
						prev_jml_FP = jml_FP;
						prev_jml_FN = jml_FN;
					}

					if (jml_sama >= 5)
					{
						break;
					}
				}

			}
		}
	}

	tb_experiment.update_file("akhir", "now()", "id=" + to_string(id_experiment));


	return 0;
}
