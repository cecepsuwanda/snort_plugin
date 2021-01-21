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



bool Tdataframe_list::service_not_in(string service)
{
	string service_name[72] = {
		// General
		"other",
		"private",

		// ICMP
		"ecr_i",
		"urp_i",
		"urh_i",
		"red_i",
		"eco_i",
		"tim_i",
		"oth_i",

		// UDP
		"domain_u",
		"tftp_u",
		"ntp_u",

		// TCP
		"IRC",
		"X11",
		"Z39_50",
		"aol",
		"auth",
		"bgp",
		"courier",
		"csnet_ns",
		"ctf",
		"daytime",
		"discard",
		"domain",
		"echo",
		"efs",
		"exec",
		"finger",
		"ftp",
		"ftp_data",
		"gopher",
		"harvest",
		"hostnames",
		"http",
		"http_2784",
		"http_443",
		"http_8001",
		"icmp",
		"imap4",
		"iso_tsap",
		"klogin",
		"kshell",
		"ldap",
		"link",
		"login",
		"mtp",
		"name",
		"netbios_dgm",
		"netbios_ns",
		"netbios_ssn",
		"netstat",
		"nnsp",
		"nntp",
		"pm_dump",
		"pop_2",
		"pop_3",
		"printer",
		"remote_job",
		"rje",
		"shell",
		"smtp",
		"sql_net",
		"ssh",
		"sunrpc",
		"supdup",
		"systat",
		"telnet",
		"time",
		"uucp",
		"uucp_path",
		"vmnet",
		"whois"
	};
	bool in_list = true;

	int i = 0;
	while ((i < 72) and in_list)
	{
		if (service == service_name[i])
		{
			in_list = false;
		}

		i++;
	}

	// if (in_list)
	// {
	// 	cout << service << endl;
	// }

	return in_list;
}

list_item Tdataframe_list::preproses_item(vector<string> &data)
{
	list_item tmp_list;

	tmp_list.datetime_holder.set_property(data[1] + " " + data[2],data[3],11,data[10]); 
	//= datetime_to_timestamp(data[1], data[2], 11);
	//tmp_list.datetime = data[1] + " " + data[2];

	Tip_port_holder tmp_ip_port_src(data[7], ((data[5] == "-") ? "0" : data[5])), tmp_ip_port_dst(data[8], ((data[6] == "-") ? "0" : data[6]));
	tmp_list.str_ip_dst = string(tmp_ip_port_dst);
	tmp_list.str_ip_src = string(tmp_ip_port_src);

	string tmp_str = data[4] ;


	if ((global_func::isNumber(tmp_str)) or (tmp_str == "ident") or (tmp_str == "1/u") or (tmp_str == "frag/i") or (tmp_str == "frag/u") or (tmp_str == "tcpmux") or (tmp_str == "chargen") ) 
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

	if (global_func::is_subs(tmp_str, "-"))
	{
		vector<string> vdata;
		vdata = global_func::tokenizer((char *)tmp_str.c_str(), "-");

		if (vdata[1] == "data")
		{
			tmp_str = vdata[0] + "_data";
		}

	} else {
		if (global_func::is_subs(tmp_str, "ecr/i")) {
			tmp_str = "ecr_i";
		} else {
			if (global_func::is_subs(tmp_str, "/"))
			{
				vector<string> vdata;

				vdata = global_func::tokenizer((char *)tmp_str.c_str(), "/");
				tmp_str = vdata[0] + "_" + vdata[1];
			} else {



			}
		}
	}

	if (service_not_in(tmp_str))
	{
		tmp_str = "other";
	}

	tmp_list.service = tmp_str;

	tmp_list.label = data[10];

	return tmp_list;
}

void Tdataframe_list::build_idx_list_lvl0(list_item tmp_list)
{
	// auto itr = _idx_list.find( tmp_list.str_ip_src);
	// if (itr == _idx_list.end())
	// {
	// 	map_list4 map4;
	// 	map4.insert(pair<string, int>(tmp_list.label, 1));

	// 	map_list3 map3;
	// 	map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));
	// 	map_list2 map2;
	// 	map2.insert(pair<string, map_list3>(tmp_list.service, map3));
	// 	map_list1 map1;
	// 	map1.insert(pair<string, map_list2>(tmp_list.str_ip_dst, map2));

	// 	_idx_list.insert(pair<string, map_list1>(tmp_list.str_ip_src, map1));
	// } else {
	// 	build_idx_list_lvl1(tmp_list, &itr->second);
	// }
}

void Tdataframe_list::build_idx_list_lvl1(list_item tmp_list, map_list1* map1)
{
	// auto itr = map1->find(tmp_list.str_ip_dst);
	// if (itr == map1->end())
	// {
	// 	map_list4 map4;
	// 	map4.insert(pair<string, int>(tmp_list.label, 1));

	// 	map_list3 map3;
	// 	map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));
	// 	map_list2 map2;
	// 	map2.insert(pair<string, map_list3>(tmp_list.service, map3));

	// 	map1->insert(pair<string, map_list2>(tmp_list.str_ip_dst, map2));

	// } else {
	// 	build_idx_list_lvl2(tmp_list, &itr->second);
	// }
}

void Tdataframe_list::build_idx_list_lvl2(list_item tmp_list, map_list2* map2)
{
	// auto itr = map2->find(tmp_list.service);
	// if (itr == map2->end())
	// {
	// 	map_list4 map4;
	// 	map4.insert(pair<string, int>(tmp_list.label, 1));

	// 	map_list3 map3;
	// 	map3.insert(pair<long,  map_list4>(tmp_list.timestamp, map4));

	// 	map2->insert(pair<string, map_list3>(tmp_list.service, map3));

	// } else {
	// 	build_idx_list_lvl3(tmp_list, &itr->second);
	// }
}

void Tdataframe_list::build_idx_list_lvl3(list_item tmp_list, map_list3* map3)
{
	// auto itr = map3->find(tmp_list.timestamp);
	// if (itr == map3->end())
	// {
	// 	map_list4 map4;
	// 	map4.insert(pair<string, int>(tmp_list.label, 1));

	// 	map3->insert(pair<long, map_list4>(tmp_list.timestamp, map4));
	// } else {
	// 	map_list4* map4 = &itr->second;
	// 	auto itr = map4->find(tmp_list.label);
	// 	if (itr == map4->end())
	// 	{
	// 		map4->insert(pair<string, int>(tmp_list.label, 1));
	// 	} else {
	// 		(*itr).second += 1;
	// 	}


	// }
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



string Tdataframe_list::search_idx_list_lvl0(list_item tmp_list)
{
	string label = "dfs_failed.";

	// auto itr = _idx_list.find(tmp_list.str_ip_src);
	// if (itr != _idx_list.end())
	// {
	// 	label = search_idx_list_lvl1(tmp_list, &itr->second);
	// } else {
	// 	label = "normal";
	// }

	return label;

}

string Tdataframe_list::search_idx_list_lvl1(list_item tmp_list, map_list1* map1)
{
	string label = "dfs_failed.";

	// auto itr = map1->find(tmp_list.str_ip_dst);
	// if (itr != map1->end())
	// {
	// 	label = search_idx_list_lvl2(tmp_list, &itr->second);
	// } else {
	// 	label = "normal";
	// }

	return label;
}

string Tdataframe_list::search_idx_list_lvl2(list_item tmp_list, map_list2* map2)
{
	string label = "dfs_failed.";
	// auto itr = map2->find(tmp_list.service);
	// if (itr != map2->end())
	// {
	// 	label = search_idx_list_lvl3(tmp_list, &itr->second);
	// } else {
	// 	label = "normal";

	// 	// cout << tmp_list.service << endl;
	// 	// for (auto it = map2->begin(); it != map2->end(); ++it) {
	// 	// 	cout << "{" << (*it).first << "}\n";
	// 	// }
	// }

	return label;
}

string Tdataframe_list::search_idx_list_lvl3(list_item tmp_list, map_list3* map3)
{
	string label = "dfs_failed.";

	// auto itr = map3->find(tmp_list.timestamp);
	// if (itr != map3->end())
	// {
	// 	map_list4* map4 = &itr->second;
	// 	if (map4->size() > 1)
	// 	{
	// 		cout << "Bingung" << endl;


	// 	} else {

	// 		auto itr = map4->begin();

	// 		if ((*itr).first == "-")
	// 		{
	// 			label = "normal";
	// 		} else {
	// 			label = "attack";
	// 		}
	// 	}
	// } else {
	// 	label = "normal";
	// 	cout << tmp_list.str_ip_src << " " << tmp_list.str_ip_dst << " " << tmp_list.datetime <<endl;
	// }

	return label;
}


string Tdataframe_list::search(string date, string hour, string ip_src, string port_src, string ip_dst, string port_dst, string service)
{
	string label = "dfs_failed.";

	// list_item list_label;

	// Tip_port_holder search_ip_port_src(ip_src, port_src),search_ip_port_dst(ip_dst, port_dst);
	
	// list_label.timestamp = datetime_to_timestamp(date, hour, 0);
	// list_label.datetime =  date + " " + hour;

	// list_label.str_ip_dst = string(search_ip_port_dst);
	// list_label.str_ip_src = string(search_ip_port_src);

	// list_label.service = service;

	// label = search_idx_list_lvl0(list_label);

	return label;

}

void Tdataframe_list::clear_memory()
{
	_data.clear_memory();
}



bool Tdataframe_list::is_pass(vector<string> &data)
{
	bool pass = true;
	if (_filter.size() > 0)
	{
		vector<field_filter>::iterator it = _filter.begin();
		while ((it != _filter.end()) and pass)
		{
			// switch (it->idx_col)
			// {
			// case 1 : {

			// 	string tmp_str = it->value;
			// 	vector<string> data_filter;
			// 	data_filter = tokenizer((char *) tmp_str.c_str(), " ");
			// 	long search_timestamp = datetime_to_timestamp(data_filter[0], data_filter[1], 0);
			// 	long tmp_timestamp = datetime_to_timestamp(data[1], data[2], 11);

			// 	switch (it->idx_opt)
			// 	{
			// 	case 2 : {
			// 		pass = search_timestamp == tmp_timestamp;
			// 		break;
			// 	}
			// 	case 3 : {
			// 		pass = search_timestamp != tmp_timestamp;
			// 		break;
			// 	}
			// 	}

			// 	break;
			// }
			// case 4 : {

			// 	switch (it->idx_opt)
			// 	{
			// 	case 2 : {
			// 		break;
			// 	}
			// 	case 3 : {
			// 		break;
			// 	}
			// 	}

			// 	break;
			// }
			// case 7 : {

			// 	break;
			// }
			// case 8 : {

			// 	break;
			// }
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
			// }
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

void Tmy_datetime_holder::set_property(string start,string end,int tmb,string label)
{
    
}