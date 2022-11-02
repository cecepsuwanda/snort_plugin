#include <chrono>
#include <iostream>
#include <experimental/filesystem>
#include "global.h"
#include "Tdec_tree.h"
#include "Tdt_build.h"

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

	config.id_dt_train = 1;
	config.jns_dt_train = 1;

	config.id_dt_test = 1;
	config.jns_dt_test = 1;

	config.search_uniqe_val = false;

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

					Tdt_build dec_tree_build(&config);

					bool file_exist = false;
					{
						Tread_file rf;
						rf.setnm_f("tree", config.id_dt_train, config.jns_dt_train);
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

					if ((j > depth_awal) and file_exist)
					{
						dec_tree_build.read_tree();
						dec_tree_build.build_from_prev_tree( j - 1);
					} else {
						cout<<"Train:"<<endl;
						dec_tree_build.build_tree();
					}


					Tdec_tree dec_tree_test(&config);
					dec_tree_test.read_tree();
					Tconf_metrix dt_conf_metrix;
					cout<<"Test:"<<endl;
					dec_tree_test.test(dt_conf_metrix);


					int jml_FN = dt_conf_metrix.get_FN("known");
					int jml_FP = dt_conf_metrix.get_FP("known");

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


	return 0;
}
