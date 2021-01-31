#include "Tdataframe_list.h"
#include "Tdataframe_label.h"
#include "Tdataframe_gure.h"
#include "Tconf_metrix.h"
#include <iostream>
using namespace std;

void labeling_1(Tdataframe_list &df_list, Tdataframe_label &df_label, Tbase_dataframe &df_save)
{
	vector<string> vec;

	if (df_label.getjmlrow() > 0)
	{
		if (df_label.open_file())
		{
			//Tconf_metrix conf_metrix;
			string label;
			df_label.read_file();
			vector<string> tmp_data;
			int i = 0;
			while (!df_label.is_eof())
			{
				tmp_data = df_label.get_record();
				string tmp_str = "";
				if (tmp_data.size() > 1)
				{
					//cout << tmp_data[tmp_data.size()-3] << " " << tmp_data[tmp_data.size()-2] << " " << tmp_data[tmp_data.size()-5] << " " << tmp_data[tmp_data.size()-4] << " " << tmp_data[tmp_data.size() - 9] << " " << tmp_data[tmp_data.size() - 8] << " " << tmp_data[tmp_data.size() - 7] << " " << tmp_data[tmp_data.size() - 6] << " " << tmp_data[2] <<endl;
					label = df_list.search(tmp_data[tmp_data.size() - 3], tmp_data[tmp_data.size() - 2], tmp_data[tmp_data.size() - 5], tmp_data[tmp_data.size() - 4], tmp_data[tmp_data.size() - 9], tmp_data[tmp_data.size() - 8], tmp_data[tmp_data.size() - 7], tmp_data[tmp_data.size() - 6], tmp_data[2]);
					//conf_metrix.add_jml(label, tmp_data[tmp_data.size() - 1], 1);
					for (int j = 0; j < 28; ++j)
					{
						tmp_str += tmp_data[j] + ",";
					}

					tmp_str += label;
					vec.push_back(tmp_str);

				} else {
					//cout << tmp_data[0] << endl;
				}


				tmp_data.clear();
				tmp_data.shrink_to_fit();
				df_label.next_record();
				i++;
				if ((i % 10000) == 0)
				{
					cout << ".";
				}
			}
			df_label.close_file();

			cout << endl;
			df_save.write_data(vec);

			vec.clear();
			vec.shrink_to_fit();

			//conf_metrix.kalkulasi();
			//cout << conf_metrix << endl;
		} else {
			cout << "Gagal Buka File !!!" << endl;
		}
	}

}

void proses_labeling1(char const *argv[])
{
	cout << "Labeling : " << argv[2] << endl;

	Tdataframe_list df_list;
	df_list.read_data(argv[1], " ");
	df_list.info();

	Tdataframe_label df_label;
	df_label.read_data(argv[2], ",");

	// field_filter f1, f2;
	// f1.idx_col = 28;
	// f1.idx_opt = 2;
	// f1.value = "207.075.239.115:3225";
	// df_label.add_filter(f1);

	// f2.idx_col = 30;
	// f2.idx_opt = 2;
	// f2.value = "172.016.114.050:80";
	// df_label.add_filter(f2);

	// f.idx_col = 36;
	// f.idx_opt = 2;
	// f.value = "dfs_failed.";
	// df_label.add_filter(f);

	df_label.info();
	//df_label.head();

	Tbase_dataframe df_save;
	df_save.read_data(argv[4]);

	labeling_1(df_list, df_label, df_save);

	df_label.clear_memory();
	df_list.clear_memory();
}

void cek_ricek(char const *argv[])
{
	// cout << "Labeling : " << argv[2] << endl;

	// Tdataframe_list df_list;
	// df_list.read_data(argv[1], " ");
	// df_list.info();

	// Tdataframe_label df_label;
	// df_label.read_data(argv[2], ",");
	// df_label.info();

	Tbase_dataframe df_save;
	df_save.read_data(argv[4], ",");
	df_save.info();

	df_save.clear_memory();
	// df_label.clear_memory();
	// df_list.clear_memory();
	cout << "----------------------------------------------------------" << endl;
}

void labeling_2(Tdataframe_gure &df_gure, Tdataframe_label &df_label)
{
	if (df_label.open_file())
	{
		vector<string> tmp_data;
		df_label.read_file();
		int i = 0;
		while ((!df_label.is_eof()))
		{
			tmp_data = df_label.get_record();

			if (tmp_data.size() > 1)
			{

				df_gure.clear_filter();
				field_filter f1, f2, f3, f4, f5;

				f1.idx_col = 4;
				f1.idx_opt = 2;
				f1.value = tmp_data[tmp_data.size() - 9] + ":" + tmp_data[tmp_data.size() - 8];
				df_gure.add_filter(f1);


				f2.idx_col = 5;
				f2.idx_opt = 2;
				f2.value = tmp_data[tmp_data.size() - 7] + ":" + tmp_data[tmp_data.size() - 6];
				df_gure.add_filter(f2);

				// f3.idx_col = 1;
				// f3.idx_opt = 2;
				// f3.value = tmp_data[tmp_data.size() - 3] + " " + tmp_data[tmp_data.size() - 2];
				// df_gure.add_filter(f3);

				// f4.idx_col = 7;
				// f4.idx_opt = 2;
				// f4.value = tmp_data[1];
				// df_gure.add_filter(f4);

				// f5.idx_col = 9;
				// f5.idx_opt = 2;
				// f5.value = tmp_data[3];
				// df_gure.add_filter(f5);



				if (df_gure.getjmlrow() == 0)
				{
					cout << tmp_data[tmp_data.size() - 3] << " " << tmp_data[tmp_data.size() - 2] << " " << tmp_data[tmp_data.size() - 5] << " " << tmp_data[tmp_data.size() - 4] << " " << tmp_data[tmp_data.size() - 9] << " " << tmp_data[tmp_data.size() - 8] << " " << tmp_data[tmp_data.size() - 7] << " " << tmp_data[tmp_data.size() - 6] << " " << tmp_data[2] << endl;
					for (int j = 0; j < (tmp_data.size() - 9); ++j)
					{
						cout << tmp_data[j] << " ";
					}
					cout << endl;
					cout << endl;
					df_gure.head();
					cout << "--------------------------------------------------------------" << endl;
					cout << endl;


				}


			}

			tmp_data.clear();
			tmp_data.shrink_to_fit();
			df_label.next_record();
			i++;
		}
		df_label.close_file();

	} else {
		cout << "Gagal Buka File !!!" << endl;
	}
}

void proses_labeling2(char const *argv[])
{
	Tdataframe_label df_label;
	df_label.read_data(argv[2], ",");

	// field_filter f1, f2;
	// f1.idx_col = 28;
	// f1.idx_opt = 2;
	// f1.value = "135.013.216.191:2938";
	// df_label.add_filter(f1);

	// f2.idx_col = 30;
	// f2.idx_opt = 2;
	// f2.value = "172.016.114.168:45";
	// df_label.add_filter(f2);

	// f.idx_col = 36;
	// f.idx_opt = 2;
	// f.value = "dfs_failed.";
	// df_label.add_filter(f);

	df_label.info();

	Tdataframe_gure df_gure;
	df_gure.read_data(argv[3], " ");
	df_gure.info();

	labeling_2(df_gure, df_label);

	df_gure.clear_memory();
	df_label.clear_memory();
}

int main(int argc, char const *argv[])
{

	proses_labeling1(argv);
	cek_ricek(argv);

	//proses_labeling2(argv);

	return 0;
}