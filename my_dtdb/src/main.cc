#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"
#include "tb_experiment.h"
#include "tb_tree.h"
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

	tb_experiment experiment;
	experiment.insert_experiment(depth_awal, depth_akhir, depth_step, min_sample_awal, min_sample_akhir, min_sample_step, threshold_awal, threshold_akhir, threshold_step, credal_s_awal, credal_s_akhir, credal_s_step, config.id_dt_train, config.jns_dt_train, config.partition_train, config.id_dt_test, config.jns_dt_test, config.partition_test);

	config.id_experiment = experiment.get_id_experiment();

	tb_tree tree;

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

					experiment.insert_detail_experiment(config.id_dt_train, config.jns_dt_train , config.id_dt_test, config.jns_dt_test, config.depth, config.min_sample, config.threshold, config.credal_s);

					config.id_detail_experiment = experiment.get_id_detail_experiment();

					Tdt_build dec_tree_build(&config);

					bool file_exist = false;

					time_t tmp_id_detail_experiment;
					file_exist = experiment.cari_detail_experiment(config.id_dt_train, config.jns_dt_train , config.id_dt_test, config.jns_dt_test, config.depth - 1, config.min_sample, config.threshold, config.credal_s, tmp_id_detail_experiment);

					if (file_exist)
					{
						file_exist = tree.cari_tree(tmp_id_detail_experiment);
					}


					if (file_exist) //(j > depth_awal) and
					{						
						dec_tree_build.read_tree(tmp_id_detail_experiment);
						dec_tree_build.build_from_prev_tree( j - 1);
					} else {
						dec_tree_build.build_tree();
					}

					experiment.end_train_start_test();

					Tdec_tree dec_tree_test(&config);
					dec_tree_test.read_tree();
					Tconf_metrix dt_conf_metrix;
					dec_tree_test.test(dt_conf_metrix);

					experiment.end_test();

					int jml_FN = dt_conf_metrix.get_FN("known");
					int jml_FP = dt_conf_metrix.get_FP("known");
					int jml_TN = dt_conf_metrix.get_TN("known");
					int jml_TP = dt_conf_metrix.get_TP("known");
					float f1 = dt_conf_metrix.get_F1();

					experiment.hsl(jml_FP, jml_FN, jml_TP, jml_TN, f1);

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

	experiment.end_experiment();


	return 0;
}
