#include "Tolah_label.h"

Tolah_label::Tolah_label()
{
}

Tolah_label::~Tolah_label()
{
}

void Tolah_label::setnm_f(string nm_attack_f,string nm_host_f)
{
	_attack_file = nm_attack_f;
	_host_file = nm_host_f;
}


void Tolah_label::baca_attack_file()
{
   Tread_file f;

	f.setnm_f(_attack_file);
	f.setseparator(',');

	if (f.open_file())
	{
		vector<string> row;
		string tmp_str;
		while (!f.is_eof())
		{
			row = f.get_record();
			if (row.size() > 0) {
				
				for (int l = 0; l < row.size(); l++)
				 {
				     cout << row[l] << endl;
				     
				 }
				 
			}

			f.next_record();
		}
		f.close_file();
	} else {
		cout << "Gagal Buka File !!!" << endl;
	}
}

void Tolah_label::baca_host_file()
{
   Tread_file f;

	f.setnm_f(_host_file);
	f.setseparator(',');

	if (f.open_file())
	{
		vector<string> row;
		string tmp_str;
		while (!f.is_eof())
		{
			row = f.get_record();
			if (row.size() > 0) {
				
				for (int l = 0; l < row.size(); l++)
				 {
				     cout << row[l] << endl;
				     
				 }
				 
			}

			f.next_record();
		}
		f.close_file();
	} else {
		cout << "Gagal Buka File !!!" << endl;
	}
}

void Tolah_label::baca_file()
{
	baca_attack_file();
	baca_host_file();
}


string Tolah_label::labeli(vector<string> row)
{
	string label = "normal.";

	return label;

}