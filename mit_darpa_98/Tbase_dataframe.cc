#include "Tbase_dataframe.h"


Tbase_dataframe::Tbase_dataframe()
{

}

Tbase_dataframe::~Tbase_dataframe()
{
	_filter.clear();
	_stat_label.clear();
}

void Tbase_dataframe::read_data(string nm_f, string separator)
{
	_nm_file = nm_f;
	_data.setnm_f(nm_f);
	_data.setseparator((char *) separator.c_str());

	stat_tabel();
}

int Tbase_dataframe::getjmlcol() {
	return _jml_col;
}

int Tbase_dataframe::getjmlrow() {
	return _jml_row;
}

void Tbase_dataframe::add_filter(field_filter filter)
{
	_data.index_off();
	is_index = true;
	_filter.push_back(filter);
	stat_tabel();
	is_index = false;
	_data.index_on();
}

void Tbase_dataframe::ReFilter()
{
	_data.index_off();
	is_index = true;
	stat_tabel();
	is_index = false;
	_data.index_on();
}

vector<field_filter> Tbase_dataframe::get_filter()
{
	return _filter;
}

bool Tbase_dataframe::is_pass(vector<string> &data)
{
	bool pass = true;

	return pass;
}

void Tbase_dataframe::stat_tabel()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	_stat_label.clear();

	_data.file_map();

	if (is_index) {
		_data.clear_index();
	}

	int i = 0;
	if (_data.open_file())
	{
		_data.read_file();
		while (!_data.is_eof())
		{
			tmp_data = _data.get_record();

			if (tmp_data.size() > 1)
			{

				if (is_pass(tmp_data))
				{
					if (is_index) {
						_data.add_index();
					}

					if (_stat_label.size() > 0)
					{
						it = _stat_label.find(tmp_data[tmp_data.size() - 1]);
						if (it == _stat_label.end())
						{
							_stat_label.insert(pair<string, int>(tmp_data[tmp_data.size() - 1], 1));
						} else {
							it->second += 1;
						}
					} else {
						_stat_label.insert(pair<string, int>(tmp_data[tmp_data.size() - 1], 1));
					}

					i++;
				}
			}

			tmp_data.clear();
			tmp_data.shrink_to_fit();

			_data.next_record();
		}


		if (is_index) {
			_data.save_to_memory();
			_data.clear_index();
		}

		_data.close_file();

	} else {
		cout << "stat_tabel, Gagal buka file !!!" << endl;
	}
	_jml_row = i;

}

map<string, int> Tbase_dataframe::get_stat_label()
{
	return _stat_label;
}

void Tbase_dataframe::write_data(vector<string> &data)
{
	if (data.size())
	{
		if (_data.open_file("w+"))
		{
			for (int i = 0; i < data.size(); ++i)
			{
				_data.write_file(data[i]);
			}

			_data.close_file();

		} else {
			cout << "Gagal Buka/Buat File !!!" << endl;
		}
	}
}

bool Tbase_dataframe::open_file()
{
	return _data.open_file();
}

void Tbase_dataframe::read_file()
{
	_data.read_file();
}

void Tbase_dataframe::close_file()
{
	_data.close_file();
}

bool Tbase_dataframe::is_eof()
{
	return _data.is_eof();
}

void Tbase_dataframe::next_record()
{
	_data.next_record();
}

vector<string> Tbase_dataframe::get_record()
{
	return _data.get_record();
}

void Tbase_dataframe::clear_memory()
{
	_data.clear_memory();
}

void Tbase_dataframe::info()
{
	cout << " Info" << endl;
	cout << " Nama File   : " << _nm_file << endl;
	cout << " Jumlah Data : " << _jml_row << endl;
	cout << " Statistik   : " << endl;
	if (_stat_label.size() > 0)
	{
		cout << setw(30) << "label" << setw(10) << "Jumlah" << endl;
		for (auto it = _stat_label.begin(); it != _stat_label.end(); ++it) {
			cout << setw(30) << (*it).first << setw(10) << (*it).second << endl;
		}
	}
}

void Tbase_dataframe::head()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	cout << " Head" << endl;
	cout << " Nama File   : " << _nm_file << endl;

	if (_jml_row > 0) {
		int i = 0;
		if (_data.open_file())
		{
			_data.read_file();
			while ((!_data.is_eof()) and (i < 5))
			{
				tmp_data = _data.get_record();

				if (tmp_data.size() > 1)
				{
					if (is_pass(tmp_data))
					{
						for (int idx = 0; idx < tmp_data.size(); ++idx)
						{

							cout << setw(tmp_data[idx].length() + 2) << tmp_data[idx];
						}

						cout << endl;

						i++;
					}
				}

				tmp_data.clear();
				tmp_data.shrink_to_fit();
				_data.next_record();
			}
			_data.close_file();
		} else {
			cout << "head, Gagal buka file !!!" << endl;
		}
	}
}


void Tbase_dataframe::clear_filter()
{
	_filter.clear();
}