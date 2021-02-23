#include "Tdataframe_darpa2.h"




void Tdataframe_darpa2::stat_tabel()
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

					Tfield_filter_darpa _field;
					_field.parser(tmp_data);

					Tdatetime_holder _date = _field.get_date();

					auto itr = vec_map.find(_date);
					if (itr == vec_map.end())
					{
						vec_field_filter vec_field;
						vec_field.push_back(_field);
						vec_map.insert(pair<Tdatetime_holder, vec_field_filter>(_date, vec_field));
					} else {
						itr->second.push_back(_field);
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


string Tdataframe_darpa2::search(string date_start, string hour_start, string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service)
{
	string label = "dfs_failed.";

	Tdatetime_holder tmp_date(date_start, "00:00:00");

    auto itr = vec_map.find(tmp_date);
	if (itr != vec_map.end())
	{
		vec_field_filter vec_field = itr->second;

		Tdatetime_holder start(date_start, hour_start);
		Tdatetime_holder end(date_end, hour_end);

		label = "normal";
		for (auto itr = vec_field.begin(); itr != vec_field.end(); itr++)
		{
			if (itr->is_pass(date_start, hour_start, date_end, hour_end, ip_src, port_src, ip_dst, port_dst, service))
			{
				label = "attack";
				break;
			}
		}
	} else {
		label = "normal";
	}

	return label;

}


