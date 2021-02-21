#include "Tdataframe_darpa.h"


Tdataframe_darpa::Tdataframe_darpa()
{

}

Tdataframe_darpa::~Tdataframe_darpa()
{

}

void Tdataframe_darpa::read_data(string nm_f)
{
	_nm_file = nm_f;
	_data.setnm_f(nm_f);

	stat_tabel();
}

void Tdataframe_darpa::stat_tabel()
{

	if (open_file())
	{

		vector<string> tmp_data;
		read_file();
		while (!is_eof())
		{

			tmp_data = get_record();

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

			tmp_data.clear();
			tmp_data.shrink_to_fit();

			next_record();
		}
		tmp_data.clear();
		tmp_data.shrink_to_fit();

		close_file();
	} else {
		cout << "stat_tabel, Gagal buka file !!!" << endl;
	}
}


bool Tdataframe_darpa::open_file()
{
	return _data.open_file();
}

void Tdataframe_darpa::read_file()
{
	_data.read_file();
}

void Tdataframe_darpa::close_file()
{
	_data.close_file();
}

bool Tdataframe_darpa::is_eof()
{
	return _data.is_eof();
}

void Tdataframe_darpa::next_record()
{
	_data.next_record();

}

vector<string> Tdataframe_darpa::get_record()
{
	vector<string> tmp_data = _data.get_record();
	string tmp_str;
	int idx;

	for (size_t i = 0; i < tmp_data.size(); ++i)
	{

		if (!global_func::is_subs(tmp_data[i], ":"))
		{
			global_func::remove_cr(tmp_str);

			tmp_data[i] = tmp_data[i].substr(1, tmp_data[i].size());

			tmp_str += tmp_data[i];
			tmp_data[idx] = tmp_str;
		} else {
			tmp_str = tmp_data[i];
			idx = i;
		}

	}

	for (auto itr = tmp_data.begin(); itr != tmp_data.end(); itr++)
	{
		string tmp_str = *itr;
		if (!global_func::is_subs(tmp_str, ":"))
		{
			tmp_data.erase(itr);
			itr--;
		}
	}


	return tmp_data;
}

string Tdataframe_darpa::search(string date_start, string hour_start, string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service)
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

void Tdataframe_darpa::info()
{
	cout << " Info" << endl;
	cout << " Nama File   : " << _nm_file << endl;

	if (vec_map.size() > 0)
	{
		auto itr = vec_map.begin();
		while (itr != vec_map.end())
		{
			auto itr1 = itr->second.begin();
			while (itr1 != itr->second.end())
			{
				itr1->info();
				itr1++;
			}

			itr++;
		}

	}
}
