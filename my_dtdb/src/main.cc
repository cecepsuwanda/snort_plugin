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

int main(int argc, const char **argv)
{
	char *endptr;
	Tconfig config;



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

	//config.f_train = argv[14];
	//config.f_test = argv[15];

	config.id_dt_train = stoi(argv[14]);;
	config.jns_dt_train = stoi(argv[15]);;
	config.partition_train = argv[16];


	config.id_dt_test = stoi(argv[17]);
	config.jns_dt_test = stoi(argv[18]);
	config.partition_test = argv[19];



	cetak("Persiapan Data Latih : \n");
	Tdataframe df_train(&config);
	df_train.set_dataset(config.id_dt_train, config.jns_dt_train, config.partition_train);
	df_train.read_header_type();
	df_train.set_parent(0, 0, 0);
	df_train.set_branch(0, 0, 0);
	df_train.clone_dataset();
	df_train.stat_tabel(true, true, true);
	df_train.setjmltotalrow();

	cetak("Persiapan Data Test : \n");
	Tdataframe df_test(&config);
	df_test.set_dataset(config.id_dt_test, config.jns_dt_test, config.partition_test);
	df_test.read_header_type();
	df_test.set_parent(0, 0, 0);
	df_test.set_branch(0, 0, 0);
	df_test.switch_to_test();
	df_test.clone_dataset();
	df_test.stat_tabel(false, false, false);

	tb_missing_branch missing_branch;

	tb_tree tree;

	cetak("Latih Model : \n");

	for (double l = threshold_awal; l <= threshold_akhir; l += threshold_step)
	{
		config.limited = l != 0;
		config.threshold = l;
		for (int i = min_sample_awal; i <= min_sample_akhir; i += min_sample_step)
		{
			config.min_sample = i;

			tb_experiment experiment;

			if (is_experiment)
			{
               experiment.set_id_experiment(id_experiment);    
			} else {

				experiment.insert_experiment(depth_awal, depth_akhir, depth_step, i, i, min_sample_step, l, l, threshold_step, credal_s_awal, credal_s_akhir, credal_s_step, config.id_dt_train, config.jns_dt_train, config.partition_train, config.id_dt_test, config.jns_dt_test, config.partition_test);
			}
			config.id_experiment = experiment.get_id_experiment();

			for (double k = credal_s_awal; k <= credal_s_akhir; k += credal_s_step)
			{
				config.use_credal = k != 0.0;
				config.credal_s = k;

				int prev_jml_FP = 0;
				int prev_jml_FN = 0;
				int jml_sama = 0;

				df_train.reset_depth_branch();
				df_test.reset_depth_branch();
				missing_branch.clear_table();


				for (int j = depth_awal; j <= depth_akhir; j += depth_step)
				{
					config.depth = j;
					config.prunning = true;

					if (is_detail_experiment)
					{
                       experiment.set_id_detail_experiment(id_detail_experiment);  
					} else {
						experiment.insert_detail_experiment(config.id_dt_train, config.jns_dt_train , config.id_dt_test, config.jns_dt_test, config.depth, config.min_sample, config.threshold, config.credal_s);
					}

					config.id_detail_experiment = experiment.get_id_detail_experiment();

					Tdt_build dec_tree_build(&config);

					bool detail_exist = false;
					bool detail_exist_1 = false;
					bool tree_exist = false;

					time_t tmp_id_detail_experiment;

					detail_exist = experiment.cari_detail_experiment(config.id_dt_train, config.jns_dt_train , config.depth, config.min_sample, config.threshold, config.credal_s, tmp_id_detail_experiment);
					if (!detail_exist) {
						detail_exist_1 = experiment.cari_detail_experiment(config.id_dt_train, config.jns_dt_train , config.depth - 1, config.min_sample, config.threshold, config.credal_s, tmp_id_detail_experiment);
					}


					if (detail_exist or detail_exist_1)
					{
						tree_exist = tree.cari_tree(tmp_id_detail_experiment);
					}

					cetak("Latih model untuk Depth : %d Credal : %f \n", j, k);

					if (tree_exist) //(j > depth_awal) and
					{
						//df_train.reset_depth_branch();
						dec_tree_build.read_tree(tmp_id_detail_experiment, missing_branch);
						dec_tree_build.build_from_prev_tree(df_train, missing_branch, j - 1, detail_exist);
					} else {
						df_train.reset_depth_branch();
						missing_branch.clear_table();
						dec_tree_build.build_tree(df_train, missing_branch);
					}

					experiment.end_train_start_test();

					if ((tree_exist) and (config.id_dt_train == config.id_dt_test) and (config.jns_dt_train == config.jns_dt_test) and (config.partition_train == config.partition_test))
					{
						df_train.train_to_test();
					} else {
						//df_test.reset_depth_branch();
					}

					cetak("Test model untuk Depth : %d Credal : %f \n", j, k);

					Tdec_tree dec_tree_test(&config);
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
						if (jml_sama >= 5)
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

	return 0;
}
