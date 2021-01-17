#include "Tdataframe_list.h"


Tdataframe_list::Tdataframe_list()
{

}

Tdataframe_list::~Tdataframe_list()
{
	_filter.clear();
	_stat_label.clear();
}

void Tdataframe_list::read_data(string nm_f)
{
	_data.setnm_f(nm_f);
	_data.setseparator(" ");

	stat_tabel();
}

void Tdataframe_list::stat_tabel()
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

				tip_fragment tmp_ip_port_src, tmp_ip_port_dst;
				long tmp_timestamp = datetime_to_timestamp(tmp_data[1], tmp_data[2], 11);

				if ((tmp_data[5] == "-") and (tmp_data[6] == "-") )
				{
					tmp_ip_port_src = ip_frag(tmp_data[7], "0");
					tmp_ip_port_dst = ip_frag(tmp_data[8], "0");
				} else {
					tmp_ip_port_src = ip_frag(tmp_data[7], tmp_data[5]);
					tmp_ip_port_dst = ip_frag(tmp_data[8], tmp_data[6]);
				}

				string tmp_str = tmp_data[4] ;

				if (is_subs(tmp_str, "/"))
				{
					vector<string> data;

					data = tokenizer((char *)tmp_str.c_str(), "/");
					tmp_str = data[0] + "_" + data[1];
				}

				string label = tmp_data[10];

				string str_ip_port_dst = to_string(tmp_ip_port_dst.part1) + "." + to_string(tmp_ip_port_dst.part2) + "." + to_string(tmp_ip_port_dst.part3) + "." + to_string(tmp_ip_port_dst.part4) + ":" + to_string(tmp_ip_port_dst.port);
				string str_ip_port_src = to_string(tmp_ip_port_src.part1) + "." + to_string(tmp_ip_port_src.part2) + "." + to_string(tmp_ip_port_src.part3) + "." + to_string(tmp_ip_port_src.part4) + ":" + to_string(tmp_ip_port_src.port);

				if (_idx_list.size() > 0)
				{
					auto itr = _idx_list.find(str_ip_port_src);
					if (itr == _idx_list.end())
					{
						vector<string> tmp_vec;
						tmp_vec.push_back(label);

						map_list3 map3;
						map3.insert(pair<long,  vector<string>>(tmp_timestamp, tmp_vec));
						map_list2 map2;
						map2.insert(pair<string, map_list3>(tmp_str, map3));
						map_list1 map1;
						map1.insert(pair<string, map_list2>(str_ip_port_dst, map2));

						_idx_list.insert(pair<string, map_list1>(str_ip_port_src, map1));
					} else {
						map_list1* map1 = &itr->second;
						auto itr = map1->find(str_ip_port_dst);
						if (itr == map1->end())
						{
							vector<string> tmp_vec;
							tmp_vec.push_back(label);

							map_list3 map3;
							map3.insert(pair<long,  vector<string>>(tmp_timestamp, tmp_vec));
							map_list2 map2;
							map2.insert(pair<string, map_list3>(tmp_str, map3));

							map1->insert(pair<string, map_list2>(str_ip_port_dst, map2));

						} else {
							map_list2* map2 = &itr->second;
							auto itr = map2->find(tmp_str);
							if (itr == map2->end())
							{
								vector<string> tmp_vec;
								tmp_vec.push_back(label);

								map_list3 map3;
								map3.insert(pair<long,  vector<string>>(tmp_timestamp, tmp_vec));

								map2->insert(pair<string, map_list3>(tmp_str, map3));

							} else {
								map_list3* map3 = &itr->second;
								auto itr = map3->find(tmp_timestamp);
								if (itr == map3->end())
								{
									vector<string> tmp_vec;
									tmp_vec.push_back(label);

									map3->insert(pair<long, vector<string>>(tmp_timestamp, tmp_vec));
								} else {
									vector<string>* tmp_vec = &itr->second;
									tmp_vec->push_back(label);
								}
							}
						}


					}

				} else {

					vector<string> tmp_vec;
					tmp_vec.push_back(label);

					map_list3 map3;
					map3.insert(pair<long,  vector<string>>(tmp_timestamp, tmp_vec));
					map_list2 map2;
					map2.insert(pair<string, map_list3>(tmp_str, map3));
					map_list1 map1;
					map1.insert(pair<string, map_list2>(str_ip_port_dst, map2));

					_idx_list.insert(pair<string, map_list1>(str_ip_port_src, map1));

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
		//cout << token << endl;
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

string Tdataframe_list::search(string date, string hour, string ip_src, string port_src, string ip_dst, string port_dst, string service)
{
	string label = "dfs_failed.";

	tip_fragment search_ip_port_src = ip_frag(ip_src, port_src);
	tip_fragment search_ip_port_dst = ip_frag(ip_dst, port_dst);
	long search_timestamp = datetime_to_timestamp(date, hour, 0);

	string str_ip_port_dst = to_string(search_ip_port_dst.part1) + "." + to_string(search_ip_port_dst.part2) + "." + to_string(search_ip_port_dst.part3) + "." + to_string(search_ip_port_dst.part4) + ":" + to_string(search_ip_port_dst.port);
	string str_ip_port_src = to_string(search_ip_port_src.part1) + "." + to_string(search_ip_port_src.part2) + "." + to_string(search_ip_port_src.part3) + "." + to_string(search_ip_port_src.part4) + ":" + to_string(search_ip_port_src.port);

	auto itr = _idx_list.find(str_ip_port_src);
	if (itr != _idx_list.end())
	{
		map_list1* map1 = &itr->second;
		auto itr = map1->find(str_ip_port_dst);
		if (itr != map1->end())
		{
			map_list2* map2 = &itr->second;
			auto itr = map2->find(service);
			if (itr != map2->end())
			{
				map_list3* map3 = &itr->second;
				auto itr = map3->find(search_timestamp);
				if (itr != map3->end())
				{ 
                   vector<string>* tmp_vec = &itr->second;
                   if(tmp_vec->size()>1)
                   {
                      //cout << "Bingung : " << endl;
                      label = "normal";
                      for (int i = 0; i < tmp_vec->size(); ++i)
                      {
                          if((*tmp_vec)[i]!="-")
                          {
                            label = "attack";      
                          }  	
                      }

                   }else{
                      label = (*tmp_vec)[0];
                      if(label=="-")
                      {
                        label = "normal"; 
                      }else{
                      	label = "attack";
                      }
                   }
				}
			}

		} else {

		}
	} else {

	}

	// if (_data.open_file())
	// {
	// 	vector<string> tmp_data;
	// 	bool ketemu = false;

	// 	_data.read_file();
	// 	while ((!_data.is_eof()) and (!ketemu))
	// 	{
	// 		tmp_data = _data.get_record();

	// 		tip_fragment tmp_ip_port_src, tmp_ip_port_dst;
	// 		long tmp_timestamp = datetime_to_timestamp(tmp_data[1], tmp_data[2], 11);

	// 		if ((tmp_data[5] == "-") and (tmp_data[6] == "-") )
	// 		{
	// 			tmp_ip_port_src = ip_frag(tmp_data[7], "0");
	// 			tmp_ip_port_dst = ip_frag(tmp_data[8], "0");
	// 		} else {
	// 			tmp_ip_port_src = ip_frag(tmp_data[7], tmp_data[5]);
	// 			tmp_ip_port_dst = ip_frag(tmp_data[8], tmp_data[6]);
	// 		}

	// 		if (search_timestamp == tmp_timestamp)
	// 		{
	// 			if (compare_ip(search_ip_port_src, tmp_ip_port_src)) {
	// 				if (compare_ip(search_ip_port_dst, tmp_ip_port_dst)) {

	// 					string tmp_str = tmp_data[4] ;

	// 					if (is_subs(tmp_str, "/"))
	// 					{
	// 						vector<string> data;

	// 						data = tokenizer((char *)tmp_str.c_str(), "/");
	// 						tmp_str = data[0] + "_" + data[1];
	// 					}

	// 					if (tmp_str == service) {

	// 						ketemu = true;
	// 						label = tmp_data[10];

	// 						if (label == "-")
	// 						{
	// 							label = "normal";
	// 						} else {
	// 							label = "attack";
	// 							//cout << tmp_data[10] << " " << label << " " << service << " " << tmp_data[1] << " " << tmp_data[2] << " " << date << " " << hour << " " << ip_src << " " << ip_dst << endl;
	// 						}


	// 					}
	// 				}
	// 			}
	// 		}

	// 		tmp_data.clear();
	// 		tmp_data.shrink_to_fit();
	// 		_data.next_record();
	// 	}


	// 	_data.close_file();
	// } else {
	// 	cout << "search, Gagal buka file !!!" << endl;
	// }

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