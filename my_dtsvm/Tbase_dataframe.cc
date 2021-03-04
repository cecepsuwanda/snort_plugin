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
	_data.setnm_f(nm_f);
	_data.setseparator(",");
}

void Tbase_dataframe::read_data_type(string nm_f)
{
	Tread_file tmp;
	tmp.setnm_f(nm_f);
	tmp.setseparator(": ");

	if (tmp.open_file())
	{
		vector<string> tmp_data;
		tmp.read_file();
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

			tmp.next_record();
			tmp_data.clear();
			tmp_data.shrink_to_fit();
			i++;
		}
		_jml_col = i;
		tmp.close_file();
	} else {
		cout << "Gagal buka " << nm_f << " !!!" << endl;
	}

	stat_tabel();
}

void Tbase_dataframe::write_data(vector<string> &data)
{
	if (data.size())
	{
		if (_data.open_file("w+"))
		{
			for (size_t i = 0; i < data.size(); ++i)
			{
				_data.write_file(data[i]);
			}

			_data.close_file();

		} else {
			cout << "Gagal Buka/Buat File !!!" << endl;
		}
	}
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
			case 0 : {
				pass = tmp2 <= tmp1 ;
				break;
			}
			case 1 : {

				pass = tmp1  < tmp2;
				break;
			}
			case 2 : {

				pass = tmp1 == tmp2;

				break;
			}
			case 3 : {

				pass = tmp1 != tmp2;
				break;
			}
			}


			i++;
		}


	}

	return pass;
}

void Tbase_dataframe::stat_tabel()
{

	_stat_label.clear();

	_data.file_map();

	if (is_index) {
		_data.clear_index();
	}

	int i = 0;
	if (_data.open_file())
	{

		vector<string> tmp_data;
		_data.read_file();
		while (!_data.is_eof())
		{

			tmp_data = _data.get_record();

			if (is_pass(tmp_data))
			{
				if (is_index) {
					_data.add_index();
				}

				_stat_label.add(tmp_data[_idx_label]);

				i++;
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
	return _stat_label.get_map();
}

int Tbase_dataframe::getjmlcol()
{
	return _jml_col;
}

int Tbase_dataframe::getjmlrow()
{
	return _jml_row;
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

void Tbase_dataframe::add_filter(int idx_col, int idx_opt, string value)
{
	_data.index_off();
	is_index = true;
	field_filter f;
	f.idx_col = idx_col;
	f.idx_opt = idx_opt;
	f.value = value;
	_filter.push_back(f);
	stat_tabel();
	is_index = false;
	_data.index_on();
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

void Tbase_dataframe::info()
{
	cout << " Info" << endl;
	cout << " Nama File   : " << _nm_file << endl;
	cout << " Jumlah Data : " << _jml_row << endl;

	cout << _stat_label << endl ;
}

void Tbase_dataframe::clear_memory()
{
	// cetak("{did:");
	// cetak(to_string(_id).c_str());
	// cetak("}");

	_data.clear_memory();
}

void Tbase_dataframe::save_to(string nm_file)
{
	if (_data.open_file())
	{

		Tread_file tmp;
		tmp.setnm_f(nm_file);
		tmp.open_file("w+");

		vector<string> tmp_data;
		_data.read_file();
		while (!_data.is_eof())
		{
			tmp_data = _data.get_record();

			if (is_pass(tmp_data))
			{
				string tmp_str = "";
				for (size_t i = 0; i < (tmp_data.size() - 1); ++i)
				{
					tmp_str = tmp_str + tmp_data[i] + ",";
				}
				tmp_str = tmp_str + tmp_data[tmp_data.size() - 1];

				tmp.write_file(tmp_str);
			}

			tmp_data.clear();
			tmp_data.shrink_to_fit();
			_data.next_record();
		}

		tmp.close_file();
		_data.close_file();
	}
}

void Tbase_dataframe::write_data(string data)
{
	
		if (_data.open_file("a+"))
		{
			_data.write_file(data);
			_data.close_file();
		}

}

