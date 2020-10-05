#include "Tread_file.h"
#include "Tolah_label.h"
#include "Tconf_metrix.h"

#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	Tolah_label olah_label;
	olah_label.setnm_f(argv[2],argv[3]);
	olah_label.baca_file();


	Tread_file f;
	Tread_file g;

	f.setnm_f(argv[1]);
	f.setseparator(',');
	cout << argv[1] << endl;

	if (f.open_file())
	{
		g.setnm_f(argv[4]);
        g.setseparator(',');

        g.open_file("w+");

		Tconf_metrix conf_metrix;
		vector<string> row;
		string tmp_str,label;
		while (!f.is_eof())
		{
			row = f.get_record();
			if (row.size() > 30) {
				tmp_str = "";
				for (int l = 0; l < row.size(); ++l)
				{
					tmp_str += row[l] + ",";
				}

				label = olah_label.labeli(row);

				conf_metrix.add_jml(label, row[row.size() - 1], 1);

				tmp_str += label ;

				g.write_file(tmp_str);

				//cout << tmp_str << endl;
			}

			f.next_record();
			row.clear();
			row.shrink_to_fit();
		}
		f.close_file();
		g.close_file();
		conf_metrix.kalkulasi();
		cout << conf_metrix << endl;
	} else {
		cout << "Gagal Buka File !!!" << endl;
	}

	return 0;
}