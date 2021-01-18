#include "Tdataframe_label.h"


Tdataframe_label::Tdataframe_label()
{

}

Tdataframe_label::~Tdataframe_label()
{

}

void Tdataframe_label::read_data(string nm_f)
{
	_data.setnm_f(nm_f);
	_data.setseparator(",");

	stat_tabel();
}

void Tdataframe_label::stat_tabel()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	_stat_label.clear();

	_data.file_map();

	if (is_index) {
		_data.clear_index();
	}

	if (_data.open_file())
	{
		_data.read_file();
		while (!_data.is_eof())
		{
			tmp_data = _data.get_record();
            if (tmp_data.size()>1)
            { 	
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
}

map<string, int> Tdataframe_label::get_stat_label()
{
	return _stat_label;
}


bool Tdataframe_label::open_file()
{
  return _data.open_file();
}

void Tdataframe_label::read_file()
{
   _data.read_file();
}

void Tdataframe_label::close_file()
{
   _data.close_file();
}

bool Tdataframe_label::is_eof()
{
   return _data.is_eof();
}

void Tdataframe_label::next_record()
{
  _data.next_record();
}

vector<string> Tdataframe_label::get_record()
{
  return _data.get_record();
}

void Tdataframe_label::clear_memory()
{
	_data.clear_memory();
}