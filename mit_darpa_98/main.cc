#include "Tdataframe_list.h"
#include "Tdataframe_label.h"
#include "Tconf_metrix.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    
	cout << argv[1] << endl ;

	Tdataframe_list df_list;

	df_list.read_data(argv[1]);
	cout << df_list.getjmlrow() << endl ;
	
	cout << argv[2] << endl ;

	Tdataframe_label df_label;

	df_label.read_data(argv[2]);

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
				//cout <<  << " " <<  << " " <<  << " " <<  << " " <<  << " " <<   << " " << tmp_data[tmp_data.size()-1] <<endl;
				label = df_list.search(tmp_data[tmp_data.size()-3],tmp_data[tmp_data.size()-2],tmp_data[tmp_data.size()-9],tmp_data[tmp_data.size()-8],tmp_data[tmp_data.size()-7],tmp_data[tmp_data.size()-6],tmp_data[2]);				
				conf_metrix.add_jml(label, tmp_data[tmp_data.size()-1], 1);
			}


			tmp_data.clear();
			tmp_data.shrink_to_fit();
			df_label.next_record();
			i++;
			if ((i % 1000) == 0)
			{
				cout << ".";
			}
		}
		df_label.close_file();

		cout << endl;

		conf_metrix.kalkulasi();
        cout << conf_metrix << endl;
	}

	df_label.clear_memory();
	df_list.clear_memory();


	return 0;
}