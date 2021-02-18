#include "Tread_file_darpa.h"

Tread_file_darpa::Tread_file_darpa()
{

}

Tread_file_darpa::~Tread_file_darpa()
{
	clear_data();
}

void Tread_file_darpa::setnm_f(string nm_f)
{
	_nm_f = nm_f;
}

bool Tread_file_darpa::open_file()
{
	_file = fopen(_nm_f.c_str(), "r");
	return !(_file == NULL);
}

void Tread_file_darpa::clear_data()
{
	_data.clear();
	_data.shrink_to_fit();
}

void Tread_file_darpa::read_file()
{
	clear_data();
	char str[1000];
	bool is_read = true;
	while ((!feof(_file)) and is_read )
	{
		if (fgets(str, 1000, _file) != NULL) {
			string tmp_str(str);

			if (tmp_str.length() == 1)
			{
				is_read = false;
			}else{
				_data.push_back(tmp_str);
			}
		}

	}
}

void Tread_file_darpa::close_file()
{
	if (!(_file == NULL))
	{
		fclose(_file);
		_file = NULL;
	}
}

bool Tread_file_darpa::is_eof()
{
	return feof(_file);
}

void Tread_file_darpa::next_record()
{
	read_file();
}

vector<string> Tread_file_darpa::get_record()
{
	return _data;
}