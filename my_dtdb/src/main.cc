#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"
#include "tb_experiment.h"
#include "tb_missing_branch.h"
#include "tb_tree.h"
#include <ctime>

using namespace std;
using std::experimental::filesystem::exists;
using std::experimental::filesystem::path;

int Tglobal_config::jns_dt_train;
int Tglobal_config::id_dt_train;
string Tglobal_config::partition_train;

int Tglobal_config::jns_dt_test;
int Tglobal_config::id_dt_test;
string Tglobal_config::partition_test;

bool Tglobal_config::use_credal;
double Tglobal_config::credal_s;
bool Tglobal_config::limited;
bool Tglobal_config::prunning;

int Tglobal_config::depth;
int Tglobal_config::min_sample;
double Tglobal_config::threshold;

time_t Tglobal_config::id_experiment;
time_t Tglobal_config::id_detail_experiment;

double Tglobal_config::ratio_valid_attr;

int main(int argc, const char **argv)
{
	char *endptr;
	Tpesan pesan;
	Tglobal_config global_config;
	global_config.init();
		
	double credal_s_awal  = strtod(argv[7], &endptr);
	double credal_s_akhir = strtod(argv[8], &endptr);
	double credal_s_step  = strtod(argv[9], &endptr);

	int depth_awal = stoi(argv[1]);
	int depth_akhir = stoi(argv[2]);
	int depth_step = stoi(argv[3]);

	int min_sample_awal  = stoi(argv[4]);
	int min_sample_akhir = stoi(argv[5]);
	int min_sample_step  = stoi(argv[6]);

	double threshold_awal  = strtod(argv[10], &endptr);
	double threshold_akhir = strtod(argv[11], &endptr);
	double threshold_step  = strtod(argv[12], &endptr);

	bool is_experiment = stoi(argv[20]) == 1;
	time_t id_experiment = (time_t) atoll(argv[21]);
	bool is_detail_experiment = stoi(argv[22]) == 1;
	time_t id_detail_experiment = (time_t) atoll(argv[23]);
	bool is_break = stoi(argv[24]) == 1;
	
	global_config.id_dt_train = stoi(argv[14]);
	global_config.jns_dt_train = stoi(argv[15]);
	global_config.partition_train = argv[16];	

	global_config.id_dt_test = stoi(argv[17]);
	global_config.jns_dt_test = stoi(argv[18]);
	global_config.partition_test = argv[19];

	pesan.cetak("Persiapan Data Latih : \n");
	Tdataframe df_train;
	df_train.set_dataset(global_config.id_dt_train, global_config.jns_dt_train, global_config.partition_train);
	df_train.read_header_type();
	df_train.set_parent(0, 0, 0);
	df_train.set_branch(0, 0, 0);
	df_train.clone_dataset();
	df_train.stat_tabel(true, true, true);
	df_train.setjmltotalrow();

	pesan.cetak("Persiapan Data Test : \n");
	Tdataframe df_test;
	df_test.set_dataset(global_config.id_dt_test, global_config.jns_dt_test, global_config.partition_test);
	df_test.read_header_type();
	df_test.set_parent(0, 0, 0);
	df_test.set_branch(0, 0, 0);
	df_test.switch_to_test();
	df_test.clone_dataset();
	df_test.stat_tabel(false, false, false);

	tb_missing_branch missing_branch;
	missing_branch.open_connection();

	tb_tree tree;

	pesan.cetak("Latih Model : \n");

	for (double l = threshold_awal; l <= threshold_akhir; l += threshold_step)
	{
		global_config.limited = l != 0;
		global_config.threshold = l;
		for (int i = min_sample_awal; i <= min_sample_akhir; i += min_sample_step)
		{
			global_config.min_sample = i;

			tb_experiment experiment;

			if (is_experiment)
			{
               experiment.set_id_experiment(id_experiment);    
			} else {

				experiment.insert_experiment(depth_awal, depth_akhir, depth_step, i, i, min_sample_step, l, l, threshold_step, credal_s_awal, credal_s_akhir, credal_s_step, global_config.id_dt_train, global_config.jns_dt_train, global_config.partition_train, global_config.id_dt_test, global_config.jns_dt_test, global_config.partition_test);
			}
			global_config.id_experiment = experiment.get_id_experiment();

			for (double k = credal_s_awal; k <= credal_s_akhir; k += credal_s_step)
			{
				global_config.use_credal = k != 0.0;
				global_config.credal_s = k;

				int prev_jml_FP = 0;
				int prev_jml_FN = 0;
				int jml_sama = 0;

				df_train.reset_depth_branch();
				df_test.reset_depth_branch();
				missing_branch.clear_table();


				for (int j = depth_awal; j <= depth_akhir; j += depth_step)
				{
					global_config.depth = j;
					global_config.prunning = true;

					if (is_detail_experiment)
					{
                       experiment.set_id_detail_experiment(id_detail_experiment);  
					} else {
						experiment.insert_detail_experiment(global_config.id_dt_train, global_config.jns_dt_train , global_config.id_dt_test, global_config.jns_dt_test, global_config.depth, global_config.min_sample, global_config.threshold, global_config.credal_s);
					}

					global_config.id_detail_experiment = experiment.get_id_detail_experiment();

					Tdt_build dec_tree_build;

					bool detail_exist = false;
					bool detail_exist_1 = false;
					bool tree_exist = false;

					time_t tmp_id_detail_experiment;

					detail_exist = experiment.cari_detail_experiment(global_config.id_dt_train, global_config.jns_dt_train , global_config.depth, global_config.min_sample, global_config.threshold, global_config.credal_s, tmp_id_detail_experiment);
					if (!detail_exist) {
						detail_exist_1 = experiment.cari_detail_experiment(global_config.id_dt_train, global_config.jns_dt_train , global_config.depth - 1, global_config.min_sample, global_config.threshold, global_config.credal_s, tmp_id_detail_experiment);
					}


					if (detail_exist or detail_exist_1)
					{
						tree_exist = tree.cari_tree(tmp_id_detail_experiment);
					}

					pesan.cetak("Latih model untuk Depth : %d Credal : %f \n", j, k);

					if (tree_exist) //(j > depth_awal) and
					{
						//df_train.reset_depth_branch();
						dec_tree_build.read_tree(tmp_id_detail_experiment);
						dec_tree_build.build_from_prev_tree(df_train, j - 1, detail_exist);
					} else {
						df_train.reset_depth_branch();
						missing_branch.clear_table();
						dec_tree_build.build_tree(df_train);
					}

					experiment.end_train_start_test();

					if ((tree_exist) and (global_config.id_dt_train == global_config.id_dt_test) and (global_config.jns_dt_train == global_config.jns_dt_test) and (global_config.partition_train == global_config.partition_test))
					{
						df_train.train_to_test();
					} else {
						//df_test.reset_depth_branch();
					}

					pesan.cetak("Test model untuk Depth : %d Credal : %f \n", j, k);

					Tdec_tree dec_tree_test;
					dec_tree_test.read_tree();
					Tconf_metrix dt_conf_metrix;
					dec_tree_test.test(df_test, dt_conf_metrix);

					experiment.end_test();

					int jml_FN = dt_conf_metrix.get_FN("normal");
					int jml_FP = dt_conf_metrix.get_FP("normal");
					int jml_TN = dt_conf_metrix.get_TN("normal");
					int jml_TP = dt_conf_metrix.get_TP("normal");
					float f1 = dt_conf_metrix.get_F1("normal");

					experiment.hsl(jml_FP, jml_FN, jml_TP, jml_TN, f1);

					jml_sama++;
					if ((prev_jml_FN != jml_FN) or (prev_jml_FP != jml_FP))
					{
						jml_sama = 0;
						prev_jml_FP = jml_FP;
						prev_jml_FN = jml_FN;
					}


					if (is_break) {
						if (jml_sama >= 10)
						{
							break;
						}
					}


				}

			}

			experiment.end_experiment();

		}
	}


	df_train.close_file();
	df_test.close_file();
	missing_branch.close_connection();

	return 0;
}
