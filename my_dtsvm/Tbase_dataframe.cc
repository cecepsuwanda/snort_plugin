#include "Tbase_dataframe.h"


Tbase_dataframe::Tbase_dataframe()
{
	_idx_label = -1;
}

Tbase_dataframe::~Tbase_dataframe()
{
	_data_header.clear();
	_data_header.shrink_to_fit();
	_data_type.clear();
	_data_type.shrink_to_fit();
	_filter.clear();
	_filter.shrink_to_fit();
	_stat_label.clear();
}

void Tbase_dataframe::set_id(int id)
{
	// cetak("{cid:");
	// cetak(to_string(id).c_str());
	// cetak("} ");
	_id = id;
}

int Tbase_dataframe::get_id()
{
	return _id;
}


static void cetak_stdout(const char *s)
{
	fputs(s, stdout);
	fflush(stdout);
}

void Tbase_dataframe::cetak ( const char * format, ... )
{
	char buffer[256];
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	//perror (buffer);
	va_end (args);
	cetak_stdout(buffer);
}

void Tbase_dataframe::read_data(string nm_f)
{
	_nm_file = nm_f;
	_data.setnm_f(nm_f, ",");
}

void Tbase_dataframe::read_data_type(string nm_f)
{
	Tread_file tmp;
	tmp.setnm_f(nm_f, ": ");

	vector<string> tmp_data;

	int i = 0;
	while (!tmp.is_eof())
	{
		tmp_data = tmp.get_record();
		_data_header.push_back(tmp_data[0]);
		_data_type.push_back(tmp_data[1]);

		if (tmp_data[0] == "label")
		{
			_idx_label = i;
		}

		if (tmp_data[1] != "continuous.")
		{
			is_non_continuous = true;
		}

		tmp.next_record();
		tmp_data.clear();
		tmp_data.shrink_to_fit();
		i++;
	}
	_jml_col = i;
	tmp.close_file();

	if (is_non_continuous)
	{
		is_42 = _jml_col == 42;
	}

	stat_tabel();
}


bool Tbase_dataframe::is_pass(vector<string> &data)
{

	bool pass = true;
	if (_filter.size() > 0)
	{
		size_t i = 0;
		while ((i < _filter.size()) and pass)
		{
			Tmy_dttype tmp1(_filter[i].value, _data_type[_filter[i].idx_col] == "continuous.");
			Tmy_dttype tmp2(data[_filter[i].idx_col], _data_type[_filter[i].idx_col] == "continuous.");

			switch (_filter[i].idx_opt)
			{
			case 0 :
				pass = tmp2 <= tmp1 ;
				break;
			case 1 :
				pass = tmp1  < tmp2;
				break;
			case 2 :
				pass = tmp1 == tmp2;
				break;
			case 3 :
				pass = tmp1 != tmp2;
				break;
			}
			i++;
		}
	}

	return pass;
}

void Tbase_dataframe::stat_tabel()
{
	_stat_label.clear();

	_data.index_on();
	if (_filter.size() > 0) {
		_data.clear_index();
	} else {
		_data.index_off();
	}

	int i = 0;

	vector<string> tmp_data;

	_data.reset_file();
	while (!_data.is_eof())
	{
		tmp_data = _data.get_record();

		if (is_pass(tmp_data))
		{
			if (_filter.size() > 0) {
				_data.add_index();
			}

			_stat_label.add(tmp_data[_idx_label]);



			i++;
		}

		tmp_data.clear();
		tmp_data.shrink_to_fit();
		_data.next_record();
	}

	if (_filter.size() > 0) {
		_data.clear_memory();
		_data.save_to_memory();
		_data.clear_index();
	}

	_data.index_on();
	_jml_row = i;


}

map<string, int> Tbase_dataframe::get_stat_label()
{
	return _stat_label.get_map();
}

float Tbase_dataframe::get_estimate_error()
{
	return _stat_label.get_estimate_error();
}

map<int, int> Tbase_dataframe::get_unique_attr()
{
	return _unique_attr;
}

bool Tbase_dataframe::is_single_label()
{
	return _stat_label.is_single_label();
}

string Tbase_dataframe::get_max_label()
{
	return _stat_label.get_max_label();
}

int Tbase_dataframe::getjmlcol()
{
	return _jml_col;
}

int Tbase_dataframe::getjmlcol_svm()
{
	return is_non_continuous ? (is_42 ? 46 : 33) : _jml_col;
}

int Tbase_dataframe::getjmlrow()
{
	return _jml_row;
}

void Tbase_dataframe::setjmltotalrow()
{
  _jml_total_row = _jml_row;  
}

int Tbase_dataframe::getjmltotalrow()
{
	return _jml_total_row;
}

void Tbase_dataframe::reset_file()
{
	_data.reset_file();
}



void Tbase_dataframe::close_file()
{
	//_data.close_file();
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

vector<string> Tbase_dataframe::get_record_svm()
{
	if (!is_non_continuous)
	{
		return _data.get_record();
	} else {
		vector<string> vec, tmp_data = _data.get_record();

		for (int i = 0; i < tmp_data.size(); ++i)
		{
			if (_data_type[i] == "continuous.")
			{
				vec.push_back(tmp_data[i]);
			} else {
				switch (i) {
				case 1:
					vec.push_back((tmp_data[i] == "tcp" ? "1" : "0" ));
					vec.push_back((tmp_data[i] == "udp" ? "1" : "0" ));
					vec.push_back((tmp_data[i] == "icmp" ? "1" : "0" ));
					break;
				case 2:
					vec.push_back(((tmp_data[i] == "private") or (tmp_data[i] == "ecri") or (tmp_data[i] == "http")) ? "0" : "1");
					vec.push_back(((tmp_data[i] == "private") or (tmp_data[i] == "ecri") or (tmp_data[i] == "http")) ? "1" : "0");
					break;
				case 3:
					vec.push_back((tmp_data[i] == "SF") ? "0" : "1");
					vec.push_back((tmp_data[i] == "SF") ? "1" : "0");
					break;
				default:
					vec.push_back(tmp_data[i]);
					break;
				}
			}
		}

		return vec;
	}
}

void Tbase_dataframe::add_filter(int idx_col, int idx_opt, string value)
{
	field_filter f;
	f.idx_col = idx_col;
	f.idx_opt = idx_opt;
	f.value = value;
	_filter.push_back(f);

	auto itr = _unique_attr.find(idx_col);
	if (itr == _unique_attr.end()) {
		_unique_attr.insert(pair<int, int>(idx_col, 1));
	} else {
		itr->second += 1;
	}

	stat_tabel();
}

void Tbase_dataframe::add_filter(field_filter filter)
{
	_filter.push_back(filter);

	auto itr = _unique_attr.find(filter.idx_col);
	if (itr == _unique_attr.end()) {
		_unique_attr.insert(pair<int, int>(filter.idx_col, 1));
	} else {
		itr->second += 1;
	}

	stat_tabel();
}

void Tbase_dataframe::ReFilter()
{
	stat_tabel();
}

vector<field_filter> Tbase_dataframe::get_filter()
{
	return _filter;
}

void Tbase_dataframe::info()
{
	cout << " Info" << endl;
	cout << " Nama File   : " << _nm_file << endl;
	cout << " Jumlah Data : " << _jml_row << endl;

	cout << _stat_label << endl ;
}

void Tbase_dataframe::save_to(string nm_file)
{
	Tread_file tmp;
	tmp.setnm_f(nm_file, ",");
	tmp.open_file("w+");

	vector<string> tmp_data;
	_data.reset_file();
	while (!_data.is_eof())
	{
		tmp_data = _data.get_record();

		string tmp_str = "";
		for (size_t i = 0; i < (tmp_data.size() - 1); ++i)
		{
			tmp_str = tmp_str + tmp_data[i] + ",";
		}
		tmp_str = tmp_str + tmp_data[tmp_data.size() - 1];

		tmp.write_file(tmp_str);


		tmp_data.clear();
		tmp_data.shrink_to_fit();
		_data.next_record();
	}
}

string Tbase_dataframe::get_data_type(int idx)
{
	return _data_type[idx];
}

void Tbase_dataframe::head()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	cout << " Head" << endl;
	cout << " Nama File   : " << _nm_file << endl;

	if (_jml_row > 0) {
		int i = 0;

		_data.reset_file();
		while ((!_data.is_eof()) and (i < 5))
		{
			tmp_data = _data.get_record();
			for (int idx = 0; idx < tmp_data.size(); ++idx)
			{
				cout << setw(tmp_data[idx].length() + 2) << tmp_data[idx];
			}
			cout << endl;
			i++;

			tmp_data.clear();
			tmp_data.shrink_to_fit();
			_data.next_record();
		}

	}
}

void Tbase_dataframe::clear_memory()
{
	_data.clear_memory();
}