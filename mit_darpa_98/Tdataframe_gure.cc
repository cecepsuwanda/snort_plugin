#include "Tdataframe_gure.h"


void Tdataframe_gure::stat_tabel()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;
	string label;

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


			if (tmp_data.size() == 47)
			{
				label = "-";
			} else {
				label = tmp_data[tmp_data.size() - 1];
			}



			if (is_pass(tmp_data))
			{
				if (is_index) {
					_data.add_index();
				}

				if (_stat_label.size() > 0)
				{
					it = _stat_label.find(label);
					if (it == _stat_label.end())
					{

						_stat_label.insert(pair<string, int>(label, 1));
					} else {
						it->second += 1;
					}
				} else {

					_stat_label.insert(pair<string, int>(label, 1));
				}

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



bool Tdataframe_gure::is_pass(vector<string> &data)
{
	bool pass = true;
	if (_filter.size() > 0)
	{
		vector<field_filter>::iterator it = _filter.begin();
		while ((it != _filter.end()) and pass)
		{
			switch (it->idx_col)
			{
			case 1 : {

				Tdatetime_holder time_gure;
				std::vector<string> v;
				v = global_func::tokenizer((char *) data[1].c_str(), ".");
				time_gure.setTime(v[0]);
				time_gure.add_time(6, 0, 0);

				string tmp_str = it->value;
				v = global_func::tokenizer((char *) tmp_str.c_str(), " ");
				//cout << v[0] << " | " << v[1] << endl;
				Tdatetime_holder time_label(v[0], v[1]);

				switch (it->idx_opt)
				{
				case 2 : {
					pass = time_gure == time_label;
					break;
				}
				case 3 : {
					pass = time_gure != time_label;
					break;
				}
				}

				break;
			}
			case 4 : {

				Tip_port_holder ip_port_gure(data[4], data[2]);

				std::vector<string> v;
				string tmp_str = it->value;
				v = global_func::tokenizer((char *) tmp_str.c_str(), ":");
				Tip_port_holder ip_port_label(v[0], v[1]);

				switch (it->idx_opt)
				{
				case 2 : {
					pass = ip_port_gure == ip_port_label;
					break;
				}
				case 3 : {
					break;
				}
				}

				break;
			}
			case 5 : {
				Tip_port_holder ip_port_gure(data[5], data[3]);

				std::vector<string> v;
				string tmp_str = it->value;
				v = global_func::tokenizer((char *) tmp_str.c_str(), ":");
				Tip_port_holder ip_port_label(v[0], v[1]);

				switch (it->idx_opt)
				{
				case 2 : {
					pass = ip_port_gure == ip_port_label;
					break;
				}
				case 3 : {
					break;
				}
				}

				break;
			}
			case 7 : {

				switch (it->idx_opt)
				{
				case 2 : {
                      pass = data[7] == it->value;
					break;
				}
				case 3 : {
					break;
				}
				}

				break;
			}
			case 9 : {
				switch (it->idx_opt)
				{
				case 2 : {
					  pass = data[9] == it->value;
					break;
				}
				case 3 : {
					break;
				}
				}
				break;
			}

				// case 10 : {
				// 	switch (it->idx_opt)
				// 	{
				// 	case 2 : {
				// 		pass = data[10] == it->value;
				// 		break;
				// 	}
				// 	case 3 : {
				// 		pass = data[10] != it->value;
				// 		break;
				// 	}
				// 	}
				// 	break;
				// }
			}
			++it;
		}
	}

	return pass;
}

void Tdataframe_gure::head()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	cout << " Head" << endl;
	cout << " Nama File   : " << _nm_file << endl;

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
						if (idx == 1)
						{
							Tdatetime_holder time_gure;
							std::vector<string> v;
							v = global_func::tokenizer((char *) tmp_data[idx].c_str(), ".");
							time_gure.setTime(v[0]);
							time_gure.add_time(6, 0, 0);

							cout << setw((string(time_gure)).length() + 2) << time_gure;

						}
						else {
							cout << setw(tmp_data[idx].length() + 2) << tmp_data[idx];
						}
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