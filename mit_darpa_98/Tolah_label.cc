#include "Tolah_label.h"

Tolah_label::Tolah_label()
{
}

Tolah_label::~Tolah_label()
{
}

vector<string> Tolah_label::tokenizer(char *str, const char *separator)
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

void Tolah_label::setnm_f(string nm_attack_f, string nm_host_f)
{
	_attack_file = nm_attack_f;
	_host_file = nm_host_f;
}

long Tolah_label::ttanggal_to_timestamp(ttanggal tgl)
{
	struct tm t;
	time_t t_of_day;

	t.tm_year = tgl.tahun - 1900; // Year - 1900
	t.tm_mon = tgl.bulan - 1;         // Month, where 0 = jan
	t.tm_mday = tgl.tanggal;          // Day of the month
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	return (long) t_of_day;
}

long Tolah_label::datetime_to_timestamp(ttanggal tgl, twaktu waktu)
{
	struct tm t;
	time_t t_of_day;

	t.tm_year = tgl.tahun - 1900; // Year - 1900
	t.tm_mon = tgl.bulan - 1;         // Month, where 0 = jan
	t.tm_mday = tgl.tanggal;          // Day of the month
	t.tm_hour = waktu.jam;
	t.tm_min = waktu.menit;
	t.tm_sec = waktu.detik;
	t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	return (long) t_of_day;
}


void Tolah_label::baca_attack_file()
{
	Tread_file f;

	f.setnm_f(_attack_file);
	f.setseparator(',');

	if (f.open_file())
	{
		map<string, int> map_day;
		map_day.insert({"Mon", 0});
		map_day.insert({"Tues", 1});
		map_day.insert({"Wed", 2});
		map_day.insert({"Thurs", 3});
		map_day.insert({"Fri", 4});

		vector<string> row;
		while (!f.is_eof())
		{
			row = f.get_record();
			if (row.size() > 0) {
				field_filter *field = new field_filter;

				field->week = row[0];
				field->day  = row[1];
				field->name = row[2];
				field->time = row[3];
				field->src  = row[4];
				field->dst  = row[5];
				field->user = row[6];
				field->where = row[7];
				field->variant = row[8];

				if (field->src == "marx") {
					field->src = "mars";
				}
				if (field->dst == "marx") {
					field->dst = "mars";
				}

				if (isString(field->src))
				{
					string hsl = search_host(field->src);
					if (hsl != "-")
					{
						field->src = hsl;
						field->vec_src.push_back(hsl);
					} else {
						if (field->src == "allvin")
						{
							int i = 0;
							while (i < vec_host.size())
							{
								size_t found = vec_host[i]->tag.find("in");
								if (found != string::npos)
								{
									field->vec_src.push_back(vec_host[i]->ip_addr);
								}
								i++;
							}
						} else {
							//cout << "string src : " << field->src << endl;
						}
					}
				} else {
					if (isIP4(field->src)) {
						field->vec_src.push_back(field->src);
					} else {
						if (isIP4star(field->src))
						{
							field->vec_src.push_back(field->src);
						} else {
							if (isQuote(field->src))
							{
								if ((field->src == "\"allpcsandlinux\"") || (field->src == "\"all.pcs.and.linux\"") || (field->src == "\"allpc|vlinux\"")) {
									int i = 0;
									while (i < vec_host.size())
									{
										size_t found = vec_host[i]->host_name.find("pc");
										if (found != string::npos)
										{
											field->vec_src.push_back(vec_host[i]->ip_addr);
										}

										found = vec_host[i]->host_name.find("linux");
										if (found != string::npos)
										{
											field->vec_src.push_back(vec_host[i]->ip_addr);
										}

										i++;
									}

								} else {
									size_t found = field->src.find("|");
									if (found != string::npos)
									{
										string tmp_str = field->src.substr(1, field->src.length() - 2);
										vector<string> data = tokenizer((char *)tmp_str.c_str(), "|");
										if (data.size() > 0) {
											for (int i = 0; i < data.size(); ++i)
											{
												if (isString(data[i]))
												{
													if (data[i] == "marx") {
														data[i] = "mars";
													}

													string hsl = search_host(data[i]);
													if (hsl != "-")
													{
														data[i] = hsl;
														field->vec_src.push_back(hsl);
													}
												} else {
													if (isIP43seg(data[i]))
													{
														field->vec_src.push_back(data[i] + ".*");
													}
												}
											}
										}

									}
								}
							} else {
								if (field->src == "10.different")
								{
									field->vec_src.push_back("10.*.*.*");
								} else {
									if (field->src == "all.attackers")
									{
										int i = 0;
										while (i < vec_host.size())
										{
											size_t found = vec_host[i]->tag.find("in");
											if (found != string::npos)
											{
												field->vec_src.push_back(vec_host[i]->ip_addr);
											}

											found = vec_host[i]->tag.find("out");
											if (found != string::npos)
											{
												field->vec_src.push_back(vec_host[i]->ip_addr);
											}

											i++;
										}
									} else {
										if (field->src == "all.outside")
										{
											int i = 0;
											while (i < vec_host.size())
											{
												size_t found = vec_host[i]->tag.find("out");
												if (found != string::npos)
												{
													field->vec_src.push_back(vec_host[i]->ip_addr);
												}
												i++;
											}

										} else {
											if ((field->src == "all.vin") || (field->src == "allvin"))
											{
												int i = 0;
												while (i < vec_host.size())
												{
													size_t found = vec_host[i]->tag.find("in");
													if (found != string::npos)
													{
														field->vec_src.push_back(vec_host[i]->ip_addr);
													}
													i++;
												}

											} else {
												//cout << "src : " << field->src << endl;
											}
										}
									}
								}
							}
						}
					}
				}

				if (isString(field->dst))
				{
					string hsl = search_host(field->dst);
					if (hsl != "-")
					{
						field->dst = hsl;
						field->vec_dst.push_back(hsl);
					} else {
						if (field->dst == "allvin")
						{
							int i = 0;
							while (i < vec_host.size())
							{
								size_t found = vec_host[i]->tag.find("in");
								if (found != string::npos)
								{
									field->vec_dst.push_back(vec_host[i]->ip_addr);
								}
								i++;
							}
						} else {
							//cout << "string dst : " << field->dst << endl;
						}
					}
				} else {
					if (isIP4(field->dst)) {
						field->vec_dst.push_back(field->dst);
					} else {
						if (isIP4star(field->dst))
						{
							field->vec_dst.push_back(field->dst);
						} else {
							if (isQuote(field->dst))
							{
								if ((field->dst == "\"allpcsandlinux\"") || (field->dst == "\"all.pcs.and.linux\"") || (field->dst == "\"allpc|vlinux\"")) {
									int i = 0;
									while (i < vec_host.size())
									{
										size_t found = vec_host[i]->host_name.find("pc");
										if (found != string::npos)
										{
											field->vec_dst.push_back(vec_host[i]->ip_addr);
										}

										found = vec_host[i]->host_name.find("linux");
										if (found != string::npos)
										{
											field->vec_dst.push_back(vec_host[i]->ip_addr);
										}

										i++;
									}
								} else {
									size_t found = field->dst.find("|");
									if (found != string::npos)
									{
										string tmp_str = field->dst.substr(1, field->dst.length() - 2);
										vector<string> data = tokenizer((char *)tmp_str.c_str(), "|");
										if (data.size() > 0) {
											for (int i = 0; i < data.size(); ++i)
											{

												if (isString(data[i]))
												{
													if (data[i] == "marx") {
														data[i] = "mars";
													}

													string hsl = search_host(data[i]);
													if (hsl != "-")
													{
														data[i] = hsl;
														field->vec_dst.push_back(hsl);
													}
												} else {
													if (isIP43seg(data[i]))
													{
														field->vec_dst.push_back(data[i] + ".*");
													}
												}
											}
										}

									}
								}
							} else {
								if (field->dst == "10.different")
								{
									field->vec_dst.push_back("10.*.*.*");
								} else {
									if (field->dst == "all.attackers")
									{
										int i = 0;
										while (i < vec_host.size())
										{
											size_t found = vec_host[i]->tag.find("in");
											if (found != string::npos)
											{
												field->vec_dst.push_back(vec_host[i]->ip_addr);
											}

											found = vec_host[i]->tag.find("out");
											if (found != string::npos)
											{
												field->vec_dst.push_back(vec_host[i]->ip_addr);
											}

											i++;
										}
									} else {
										if (field->dst == "all.outside")
										{
											int i = 0;
											while (i < vec_host.size())
											{
												size_t found = vec_host[i]->tag.find("out");
												if (found != string::npos)
												{
													field->vec_dst.push_back(vec_host[i]->ip_addr);
												}
												i++;
											}
										} else {
											if ((field->dst == "all.vin") ||  (field->dst == "allvin"))
											{
												int i = 0;
												while (i < vec_host.size())
												{
													size_t found = vec_host[i]->tag.find("in");
													if (found != string::npos)
													{
														field->vec_dst.push_back(vec_host[i]->ip_addr);
													}
													i++;
												}
											} else {
												//cout << "dst : " << field->dst << endl;
											}
										}
									}
								}
							}
						}
					}
				}

				ttanggal attack_tgl;

				attack_tgl.tanggal = 0;
				attack_tgl.bulan = 0;
				attack_tgl.tahun = 1998;
				if (stoi(row[0]) < 6)
				{
					attack_tgl.tanggal = 1 + ((stoi(row[0]) - 1) * 7);
					attack_tgl.bulan = 6;
				} else {
					attack_tgl.tanggal = 6 + ((stoi(row[0]) - 6) * 7);
					attack_tgl.bulan = 7;
				}

				auto it = map_day.find(row[1]);
				if (it != map_day.end())
				{
					attack_tgl.tanggal += it->second;
					if (attack_tgl.tanggal > 30)
					{
						attack_tgl.tanggal = attack_tgl.tanggal % 30;
						attack_tgl.bulan += 1;
					}
				}

				twaktu tmp_waktu = waktu_frag(field->time);
				tmp_waktu.jam += 12;

				if (tmp_waktu.jam >= 24)
				{
					attack_tgl.tanggal += 1;
					tmp_waktu.jam = tmp_waktu.jam % 24;
				}

				field->tgl_timestamp = ttanggal_to_timestamp(attack_tgl);
				field->waktu_timestamp = datetime_to_timestamp(attack_tgl, tmp_waktu);

				vec_attack.push_back(field);

				// if (field->src == "src.same.as.dst")
				// {
				// 	cout << "Tanggal       : " << attack_tgl.tanggal << "/" << attack_tgl.bulan << "/" << attack_tgl.tahun << endl ;
				// 	cout << "Jam           : " << tmp_waktu.jam << ":" << tmp_waktu.menit << ":" << tmp_waktu.detik << endl;
				// 	// cout << "tgl timestamp : " << field->tgl_timestamp << endl;
				// 	// cout << "waktu timestamp : " << field->waktu_timestamp << endl;
				// 	cout << "src     : " << field->src << endl;
				// 	cout << "dst     : " << field->dst << endl;




				// 	string tmp_str = "";
				// 	for (int i = 0; i < row.size(); ++i)
				// 	{
				// 		tmp_str += row[i] + ",";
				// 	}
				// 	cout << tmp_str << endl;
				// }

			}

			f.next_record();
			row.clear();
			row.shrink_to_fit();
		}
		f.close_file();

	} else {
		cout << "Gagal Buka File !!!" << endl;
	}
}

void Tolah_label::baca_host_file()
{
	Tread_file f;

	f.setnm_f(_host_file);
	f.setseparator(',');

	if (f.open_file())
	{
		vector<string> row;
		while (!f.is_eof())
		{
			row = f.get_record();
			if (row.size() > 0) {
				thost *tmp_host = new thost;
				tmp_host->ip_addr = row[0];
				tmp_host->host_name = row[1];
				tmp_host->tag = row[4];
				vec_host.push_back(tmp_host);
			}

			f.next_record();
			row.clear();
			row.shrink_to_fit();
		}
		f.close_file();
	} else {
		cout << "Gagal Buka File !!!" << endl;
	}
}

string Tolah_label::search_host(string token)
{
	string hasil = "-";
	bool ketemu = false;

	int i = 0;
	while ( !ketemu  && (i < vec_host.size()))
	{
		size_t found = vec_host[i]->host_name.find(token);
		if (found != string::npos)
		{
			hasil = vec_host[i]->ip_addr;
			ketemu = true;
		}

		i++;
	}
	return hasil;
}

bool Tolah_label::isIP4(string token)
{
	return std::regex_match(token, std::regex(("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b")));
}

bool Tolah_label::isIP4star(string token)
{
	return std::regex_match(token, std::regex(("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.[*]?")));
}

bool Tolah_label::isIP43seg(string token)
{
	return std::regex_match(token, std::regex(("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}")));
}

bool Tolah_label::isQuote(string token)
{
	return std::regex_match(token, std::regex(("\"(.*?)\"")));
}

bool Tolah_label::isString(string token)
{
	return std::regex_match(token, std::regex(("[a-z0-9]+")));
}

bool Tolah_label::is_subs(string stack, string needle)
{
	bool is_pass = false;

	size_t posisi = stack.find(needle);
	is_pass = posisi != string::npos;

	return is_pass;
}

bool Tolah_label::isNumber(std::string token)
{
	return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

twaktu Tolah_label::waktu_frag(string waktu_str)
{
	twaktu tmp;

	vector<string> data = tokenizer((char *)waktu_str.c_str(), ":");

	tmp.jam = stoi(data[0]);
	tmp.menit = stoi(data[1]);

	string tmp_detik = data[2];
	size_t posisi = tmp_detik.find(".");
	if (posisi == string::npos)
	{
		tmp.detik = stoi(tmp_detik);
	}
	else
	{
		data = tokenizer((char *)tmp_detik.c_str(), ".");
		tmp.detik = stoi(data[0]);
	}

	return tmp;
}

tip_fragment Tolah_label::ip_frag(string ip_port)
{
	tip_fragment tmp;
	vector<string> data;

	if (is_subs(ip_port, ":"))
	{
		data = tokenizer((char *)ip_port.c_str(), ":");
		tmp.port = stoi(data[1]);
		string ip = data[0];
		data = tokenizer((char *)ip.c_str(), ".");
		tmp.part1 = isNumber(data[0]) ? data[0] : "0";
		tmp.part2 = isNumber(data[1]) ? data[1] : "0";
		tmp.part3 = isNumber(data[2]) ? data[2] : "0";
		tmp.part4 = isNumber(data[3]) ? data[3] : "0";
	}
	else
	{
		if (is_subs(ip_port, "."))
		{
			data = tokenizer((char *)ip_port.c_str(), ".");
			tmp.part1 = isNumber(data[0]) ? data[0] : "0";
			tmp.part2 = isNumber(data[1]) ? data[1] : "0";
			tmp.part3 = isNumber(data[2]) ? data[2] : "0";
			tmp.part4 = isNumber(data[3]) ? data[3] : "0";
		}
		else
		{
			tmp.part1 = "0";
			tmp.part2 = "0";
			tmp.part3 = "0";
			tmp.part4 = "0";
		}
	}

	return tmp;
}

int Tolah_label::waktu_to_sec(twaktu waktu)
{
	return (waktu.jam * 3600) + (waktu.menit * 60) + waktu.detik;
}

void Tolah_label::baca_file()
{
	baca_host_file();
	baca_attack_file();


	// auto itr = vec_map.begin();

	// while (itr != vec_map.end())
	// {
	// 	auto itr1 = itr->second.begin();
	// 	while (itr1 != itr->second.end())
	// 	{
	// 		vec_field_filter *tmp_vec = &itr1->second;
	// 		auto itr2 = tmp_vec->begin();
	// 		while (itr2 != tmp_vec->end())
	// 		{
	// 			field_filter *field = *itr2;


	// 			twaktu tmp = waktu_frag(field->time);

	// 			tmp.jam += 12;
	// 			if (tmp.jam >= 24)
	// 			{
	// 				tmp.jam = tmp.jam % 24;
	// 			}

	// 			field->time =  to_string(tmp.jam) + ":" + to_string(tmp.menit) + ":" + to_string(tmp.detik);


	// 			//cout << "src : " << field->src << endl;
	// 			//cout << "dst : " << field->dst << endl;


	// 			if (!isQuote(field->src) && !isQuote(field->dst))
	// 			{
	// 				if (isString(field->src) && isString(field->dst)) {

	// 					if(field->src == "marx"){
//                           field->src = "mars";
	// 					}
	// 					if(field->dst == "marx"){
//                           field->dst = "mars";
	// 					}

	// 					string hsl = search_host(field->src);
	// 					if (hsl != "-")
	// 					{
	// 						field->src = hsl;
	// 					}

	// 					hsl = search_host(field->dst);
	// 					if (hsl != "-")
	// 					{
	// 						field->dst = hsl;
	// 					}

	// 					//cout << "----src : " << field->src << endl;
	// 					//cout << "----dst : " << field->dst << endl;
	// 				} else {
	// 					if (isString(field->src)) {
	// 						string hsl = search_host(field->src);
	// 						if (hsl != "-")
	// 						{
	// 							field->src = hsl;
	// 						}

	// 						//cout << "----src : " << field->src << endl;
	// 						//cout << "----dst : " << field->dst << endl;
	// 					} else {
	// 						if (isString(field->dst)) {
	// 							string hsl = search_host(field->dst);
	// 							if (hsl != "-")
	// 							{
	// 								field->dst = hsl;
	// 							}

	// 							//cout << "----src : " << field->src << endl;
	// 							//cout << "----dst : " << field->dst << endl;
	// 						}
	// 					}
	// 				}

	// 			} else {

	// 			}


	// 			itr2++;
	// 		}

	// 		itr1++;
	// 	}

	// 	itr++;
	// }

	/*for (int i = 0; i < vec_host.size(); ++i)
	{
		cout << vec_host[i]->ip_addr << "  " << vec_host[i]->host_name << endl;
	}*/
}

bool Tolah_label::is_waktu_pass(vector<string> &row, field_filter *field)
{
	bool is_pass = false;

	string tmp_row = row[0];

	vector<string> data = tokenizer((char *)tmp_row.c_str(), "-");
	twaktu tmp = waktu_frag(data[1]);
	twaktu tmp1 = waktu_frag(field->time);

	int tmp_sec = waktu_to_sec(tmp);
	int start_time_sec = waktu_to_sec(tmp1);

	if (tmp_sec >= start_time_sec)
	{
		is_pass = true;
	}


	return is_pass;
}

bool Tolah_label::compare_ip(tip_fragment ip1, tip_fragment ip2)
{
	bool is_pass = false;
	vector<string> data;

	if (stoi(ip1.part1) == stoi(ip2.part1))
	{
		if (stoi(ip1.part2) == stoi(ip2.part2))
		{
			if (stoi(ip1.part3) == stoi(ip2.part3))
			{
				if (is_subs(ip2.part4, "*"))
				{
					is_pass = true;
				}
				else
				{
					if (is_subs(ip2.part4, "-"))
					{
						data = tokenizer((char *)ip2.part4.c_str(), "-");
						//cout << data[0] << "<=" << ip1.part4 << "<=" << data[1] << endl;
						is_pass = (stoi(ip1.part4) >= stoi(data[0])) and (stoi(ip1.part4) <= stoi(data[1]));
					}
					else
					{
						is_pass = stoi(ip1.part4) == stoi(ip2.part4);
					}
				}
			}
		}
	}

	return is_pass;
}

bool Tolah_label::is_ip_pass(vector<string> &row, field_filter *field)
{
	bool is_pass = false;
	bool is_pass1, is_pass2, is_pass3, is_pass4;
	tip_fragment tmp_ip1, tmp_ip2;

	tip_fragment tmp_ip_src = ip_frag(row[1]);
	tip_fragment tmp_ip_dst = ip_frag(row[2]);

	tmp_ip_src.protocol = row[4];
	tmp_ip_dst.protocol = row[4];



	if (!isQuote(field->src) && !isQuote(field->dst))
	{
		if (isIP4(field->src) && isIP4(field->dst))
		{
			tmp_ip1 = ip_frag(field->src);
			tmp_ip2 = ip_frag(field->dst);

			is_pass1 =  compare_ip(tmp_ip_src, tmp_ip1);
			is_pass2 =  compare_ip(tmp_ip_dst, tmp_ip2);
			is_pass3 =  compare_ip(tmp_ip_dst, tmp_ip1);
			is_pass4 =  compare_ip(tmp_ip_src, tmp_ip2);

			is_pass = (is_pass1 && is_pass2) || (is_pass3 && is_pass4);
			// if(is_pass2){
			//   cout << field->name << endl;
			//   cout << row[1] << " " << row[2]  << endl;
			//            cout << field->src << " " << field->dst  << endl;
			//            cout << "------------------------------" << endl;
			//          }
			if (is_pass)
			{
				// cout << row[0] << endl;
			}

		}
	}



	return is_pass;
}

string Tolah_label::labeli(vector<string> row, string week, string day)
{
	string label = "normal.";

	auto itr = vec_map.find(week);
	if (itr != vec_map.end())
	{
		map<string, vec_field_filter> *tmp_map = &itr->second;
		auto itr1 = tmp_map->find(day);
		if (itr1 != tmp_map->end())
		{
			vec_field_filter *tmp_vec = &itr1->second;
			bool ketemu = false;
			auto itr2 = tmp_vec->begin();
			while ( !ketemu && (itr2 != tmp_vec->end()) )
			{
				field_filter *field = *itr2;
				if (is_waktu_pass(row, field))
				{

					if (is_ip_pass(row, field))
					{
						ketemu = true;
						label = field->name + ".";
					}
				}


				//cout << field->name << endl;
				//cout << field->time << endl;
				//cout << field->src << endl;
				//cout << field->dst << endl;
				itr2++;
			}
		}
	}

	return label;

}