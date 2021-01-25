#include "Tdataframe_list.h"
#include "Tdataframe_label.h"
#include "Tdataframe_gure.h"
#include "Tconf_metrix.h"
#include <iostream>
using namespace std;

void labeling_1(Tdataframe_list &df_list, Tdataframe_label &df_label)
{
	if (df_label.open_file())
	{
		Tconf_metrix conf_metrix;
		string label;
		df_label.read_file();
		vector<string> tmp_data;
		int i = 0;
		while (!df_label.is_eof())
		{
			tmp_data = df_label.get_record();

			if (tmp_data.size() > 1)
			{
				//cout << tmp_data[tmp_data.size()-3] << " " << tmp_data[tmp_data.size()-2] << " " << tmp_data[tmp_data.size()-5] << " " << tmp_data[tmp_data.size()-4] << " " << tmp_data[tmp_data.size() - 9] << " " << tmp_data[tmp_data.size() - 8] << " " << tmp_data[tmp_data.size() - 7] << " " << tmp_data[tmp_data.size() - 6] << " " << tmp_data[2] <<endl;
				label = df_list.search(tmp_data[tmp_data.size() - 3], tmp_data[tmp_data.size() - 2], tmp_data[tmp_data.size() - 5], tmp_data[tmp_data.size() - 4], tmp_data[tmp_data.size() - 9], tmp_data[tmp_data.size() - 8], tmp_data[tmp_data.size() - 7], tmp_data[tmp_data.size() - 6], tmp_data[2]);
				conf_metrix.add_jml(label, tmp_data[tmp_data.size() - 1], 1);
			} else {
				//cout << tmp_data[0] << endl;
			}


			tmp_data.clear();
			tmp_data.shrink_to_fit();
			df_label.next_record();
			i++;
			if ((i % 1000) == 0)
			{
				//cout << ".";
			}
		}
		df_label.close_file();

		cout << endl;

		conf_metrix.kalkulasi();
		cout << conf_metrix << endl;
	} else {
		cout << "Gagal Buka File !!!" << endl;
	}


}

void labeling_2(Tdataframe_list &df_gure, Tdataframe_label &df_label)
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

				if (df_gure.getjmlrow() > 1)
				{
					cout << tmp_data[tmp_data.size() - 3] << " " << tmp_data[tmp_data.size() - 2] << " " << tmp_data[tmp_data.size() - 5] << " " << tmp_data[tmp_data.size() - 4] << " " << tmp_data[tmp_data.size() - 9] << " " << tmp_data[tmp_data.size() - 8] << " " << tmp_data[tmp_data.size() - 7] << " " << tmp_data[tmp_data.size() - 6] << " " << tmp_data[2] << endl;
					for (int j = 0; j < (tmp_data.size() - 9); ++j)
					{
						cout << tmp_data[j] << " ";
					}
					cout << endl;
					df_gure.head();
					cout << "--------------------------------------------------------------" << endl;
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

int main(int argc, char const *argv[])
{

	Tdataframe_list df_list;

	df_list.read_data(argv[1], " ");
	df_list.info();

	Tdataframe_label df_label;
	df_label.read_data(argv[2], ",");
	//df_label.info();
	//df_label.head();

	// field_filter f1, f2;
	// f1.idx_col = 28;
	// f1.idx_opt = 2;
	// f1.value = "192.168.001.010:123";
	// df_label.add_filter(f1);

	// f2.idx_col = 30;
	// f2.idx_opt = 2;
	// f2.value = "172.016.112.020:123";
	// df_label.add_filter(f2);

	// f.idx_col = 36;
	// f.idx_opt = 2;
	// f.value = "dfs_failed.";
	// df_label.add_filter(f);

	df_label.info();
	df_label.head();

	labeling_1(df_list,df_label);

	// Tdataframe_gure df_gure;
	// df_gure.read_data(argv[3], " ");
	// df_gure.info();
    
 //    labeling_2(df_gure,df_label); 
	
	// df_gure.clear_memory();
	df_label.clear_memory();
	df_list.clear_memory();



	return 0;
}