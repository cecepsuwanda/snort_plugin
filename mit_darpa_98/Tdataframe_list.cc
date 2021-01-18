#include "Tdataframe_list.h"


Tdataframe_list::Tdataframe_list()
{

}

Tdataframe_list::~Tdataframe_list()
{
	_filter.clear();
	_stat_label.clear();
	_idx_list.clear();
}

void Tdataframe_list::read_data(string nm_f)
{
	_data.setnm_f(nm_f);
	_data.setseparator(" ");

	stat_tabel();
}

string Tdataframe_list::ip_fragment_str(tip_fragment ip_frag)
{
	return to_string(ip_frag.part1) + "." + to_string(ip_frag.part2) + "." + to_string(ip_frag.part3) + "." + to_string(ip_frag.part4) + ":" + to_string(ip_frag.port);
}

list_item Tdataframe_list::preproses_item(vector<string> &data)
{
	list_item tmp_list;

	tmp_list.timestamp = datetime_to_timestamp(data[1], data[2], 11);

	tip_fragment tmp_ip_port_src, tmp_ip_port_dst;
	tmp_ip_port_src = ip_frag(data[7], ((data[5] == "-") ? "0" : data[5]));
	tmp_ip_port_dst = ip_frag(data[8], ((data[6] == "-") ? "0" : data[6]));
	tmp_list.str_ip_dst = ip_fragment_str(tmp_ip_port_dst);
	tmp_list.str_ip_src = ip_fragment_str(tmp_ip_port_src);

	string tmp_str = data[4] ;

	if ((isNumber(tmp_str)) or (tmp_str == "tcpmux") or (tmp_str == "chargen") or (tmp_str == "1/u") )
	{
		tmp_str = "other";
	} else {
		if (tmp_str == "pop3")
		{
			tmp_str = "pop_3";
		} else {
			if (tmp_str == "irc")
			{
				tmp_str = "IRC";
			} else {
				if (tmp_str == "imap")
				{
					tmp_str = "imap4";
				} else {

				}
			}
		}
	}

	if (is_subs(tmp_str, "-"))
	{
		vector<string> vdata;
		vdata = tokenizer((char *)tmp_str.c_str(), "-");

		if (vdata[1] == "data")
		{
			tmp_str = vdata[0] + "_data";
		}

	} else {
		if (is_subs(tmp_str, "ecr/i")) {
			tmp_str = "ecr_i";
		} else {
			if (is_subs(tmp_str, "/"))
			{
				vector<string> vdata;

				vdata = tokenizer((char *)tmp_str.c_str(), "/");
				tmp_str = vdata[0] + "_" + vdata[1];
			} else {



			}
		}
	}

	tmp_list.service = tmp_str;

	tmp_list.label = data[10];

	return tmp_list;
}

void Tdataframe_list::build_idx_list_lvl0(list_item tmp_list)
{
	auto itr = _idx_list.find( tmp_list.str_ip_src);
	if (itr == _idx_list.end())
	{
		map_list4 map4;
		map4.insert(pair<string, int>(tmp_list.label, 1));

		map_list3 map3;
		map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));
		map_list2 map2;
		map2.insert(pair<string, map_list3>(tmp_list.service, map3));
		map_list1 map1;
		map1.insert(pair<string, map_list2>(tmp_list.str_ip_dst, map2));

		_idx_list.insert(pair<string, map_list1>(tmp_list.str_ip_src, map1));
	} else {
		build_idx_list_lvl1(tmp_list, &itr->second);
	}
}

void Tdataframe_list::build_idx_list_lvl1(list_item tmp_list, map_list1* map1)
{
	auto itr = map1->find(tmp_list.str_ip_dst);
	if (itr == map1->end())
	{
		map_list4 map4;
		map4.insert(pair<string, int>(tmp_list.label, 1));

		map_list3 map3;
		map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));
		map_list2 map2;
		map2.insert(pair<string, map_list3>(tmp_list.service, map3));

		map1->insert(pair<string, map_list2>(tmp_list.str_ip_dst, map2));

	} else {
		build_idx_list_lvl2(tmp_list, &itr->second);
	}
}

void Tdataframe_list::build_idx_list_lvl2(list_item tmp_list, map_list2* map2)
{
	auto itr = map2->find(tmp_list.service);
	if (itr == map2->end())
	{
		map_list4 map4;
		map4.insert(pair<string, int>(tmp_list.label, 1));

		map_list3 map3;
		map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));

		map2->insert(pair<string, map_list3>(tmp_list.service, map3));

	} else {
		build_idx_list_lvl3(tmp_list, &itr->second);
	}
}

void Tdataframe_list::build_idx_list_lvl3(list_item tmp_list, map_list3* map3)
{
	auto itr = map3->find(tmp_list.timestamp);
	if (itr == map3->end())
	{
		map_list4 map4;
		map4.insert(pair<string, int>(tmp_list.label, 1));

		map3->insert(pair<long, map_list4>(tmp_list.timestamp, map4));
	} else {
		map_list4* map4 = &itr->second;
		auto itr = map4->find(tmp_list.label);
		if (itr == map4->end())
		{
			map4->insert(pair<string, int>(tmp_list.label, 1));
		} else {
			(*itr).second += 1;
		}


	}
}

void Tdataframe_list::stat_tabel()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	_stat_label.clear();
	_idx_list.clear();

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


				list_item tmp_list = preproses_item(tmp_data);
				build_idx_list_lvl0(tmp_list);

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

map<string, int> Tdataframe_list::get_stat_label()
{
	return _stat_label;
}

vector<string> Tdataframe_list::tokenizer(char *str, const char *separator)
{
	char *token;
	vector<string> vec;
	int len;

	len = strlen(str);
	if (str[len - 1] == '\n')
		str[len - 1] = 0;


	token = strtok(str, separator);
	while (token != 0)
	{
		vec.push_back(token);
		token = strtok(0, separator);
	}

	return vec;
}

tip_fragment Tdataframe_list::ip_frag(string ip, string port)
{
	tip_fragment tmp;
	vector<string> data;

	data = tokenizer((char *)ip.c_str(), ".");
	tmp.part1 = stoi(data[0]);
	tmp.part2 = stoi(data[1]);
	tmp.part3 = stoi(data[2]);
	tmp.part4 = stoi(data[3]);
	tmp.port = stoi(port);

	return tmp;
}

long Tdataframe_list::datetime_to_timestamp(string date, string hour, int tmb)
{
	vector<string> data_date = tokenizer((char *)date.c_str(), "/");
	vector<string> data_hour = tokenizer((char *)hour.c_str(), ":");

	struct tm t;
	time_t t_of_day;

	t.tm_year = stoi(data_date[2]) - 1900; // Year - 1900
	t.tm_mon = stoi(data_date[0]) - 1;         // Month, where 0 = jan
	t.tm_mday = stoi(data_date[1]);          // Day of the month
	t.tm_hour = stoi(data_hour[0]);
	t.tm_min = stoi(data_hour[1]);
	t.tm_sec = stoi(data_hour[2]);
	t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	return ((long) t_of_day) + (tmb * 60 * 60);
}

bool Tdataframe_list::compare_ip(tip_fragment ip1, tip_fragment ip2)
{
	bool is_pass = false;

	if (ip1.part1 == ip2.part1)
	{
		if (ip1.part2 == ip2.part2)
		{
			if (ip1.part3 == ip2.part3)
			{
				if (ip1.part4 == ip2.part4)
				{
					if (ip1.port == ip2.port)
					{
						is_pass = true;
					}
				}

			}
		}
	}

	return is_pass;
}

string Tdataframe_list::search_idx_list_lvl0(list_item tmp_list)
{
	string label = "dfs_failed.";

	auto itr = _idx_list.find(tmp_list.str_ip_src);
	if (itr != _idx_list.end())
	{
		label = search_idx_list_lvl1(tmp_list, &itr->second);
	} else {
		label = "normal";
	}

	return label;

}

string Tdataframe_list::search_idx_list_lvl1(list_item tmp_list, map_list1* map1)
{
	string label = "dfs_failed.";

	auto itr = map1->find(tmp_list.str_ip_dst);
	if (itr != map1->end())
	{
		label = search_idx_list_lvl2(tmp_list, &itr->second);
	} else {
		label = "normal";
	}

	return label;
}

string Tdataframe_list::search_idx_list_lvl2(list_item tmp_list, map_list2* map2)
{
	string label = "dfs_failed.";
	auto itr = map2->find(tmp_list.service);
	if (itr != map2->end())
	{
		label = search_idx_list_lvl3(tmp_list, &itr->second);
	} else {
		label = "normal";

		// cout << service << endl;
		// for (auto it = map2->begin(); it != map2->end(); ++it) {
		// 	cout << "{" << (*it).first << "}\n";
		// }
	}

	return label;
}

string Tdataframe_list::search_idx_list_lvl3(list_item tmp_list, map_list3* map3)
{
	string label = "dfs_failed.";

	auto itr = map3->find(tmp_list.timestamp);
	if (itr != map3->end())
	{
		map_list4* map4 = &itr->second;
		if (map4->size() > 1)
		{
			cout << "Bingung" << endl;


		} else {

			auto itr = map4->begin();

			if ((*itr).first == "-")
			{
				label = "normal";
			} else {
				label = "attack";
			}
		}
	} else {
		label = "normal";
	}

	return label;
}


string Tdataframe_list::search(string date, string hour, string ip_src, string port_src, string ip_dst, string port_dst, string service)
{
	string label = "dfs_failed.";

	list_item list_label;

	tip_fragment search_ip_port_src = ip_frag(ip_src, port_src);
	tip_fragment search_ip_port_dst = ip_frag(ip_dst, port_dst);
	list_label.timestamp = datetime_to_timestamp(date, hour, 0);

	list_label.str_ip_dst = ip_fragment_str(search_ip_port_dst);
	list_label.str_ip_src = ip_fragment_str(search_ip_port_src);

	list_label.service = service;

    label = search_idx_list_lvl0(list_label);

	return label;

}

void Tdataframe_list::clear_memory()
{
	_data.clear_memory();
}

bool Tdataframe_list::is_subs(string stack, string needle)
{
	bool is_pass = false;

	size_t posisi = stack.find(needle);
	is_pass = posisi != string::npos;

	return is_pass;
}

bool Tdataframe_list::is_pass(vector<string> &data)
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

				string tmp_str = it->value;
				vector<string> data_filter;
				data_filter = tokenizer((char *) tmp_str.c_str(), " ");
				long search_timestamp = datetime_to_timestamp(data_filter[0], data_filter[1], 0);
				long tmp_timestamp = datetime_to_timestamp(data[1], data[2], 11);

				switch (it->idx_opt)
				{
				case 2 : {
					pass = search_timestamp == tmp_timestamp;
					break;
				}
				case 3 : {
					pass = search_timestamp != tmp_timestamp;
					break;
				}
				}

				break;
			}
			case 4 : {

				switch (it->idx_opt)
				{
				case 2 : {
					break;
				}
				case 3 : {
					break;
				}
				}

				break;
			}
			case 7 : {

				break;
			}
			case 8 : {

				break;
			}
			case 10 : {
				switch (it->idx_opt)
				{
				case 2 : {
					pass = data[10] == it->value;
					break;
				}
				case 3 : {
					pass = data[10] != it->value;
					break;
				}
				}
				break;
			}
			}
			++it;
		}
	}

	return pass;
}

bool Tdataframe_list::isNumber(string token)
{
	return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

void Tdataframe_list::add_filter(field_filter filter)
{
	_data.index_off();
	is_index = true;
	_filter.push_back(filter);
	stat_tabel();
	is_index = false;
	_data.index_on();
}

int Tdataframe_list::getjmlrow()
{
	return _jml_row;
}

void Tdataframe_list::clear_filter()
{
	_filter.clear();
}