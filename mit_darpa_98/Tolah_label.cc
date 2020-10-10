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

void Tolah_label::isi_vec_src_dst(string &ip_str, vector<string> &vec)
{

	auto isi_vec_by_host = [ = ](vector<string> &vec, string search_field, vector<string> vec_cari, string & ip_host)->bool
	{
		int i = 0;
		size_t found;
		bool ketemu = false;
		while (i < this->vec_host.size())
		{
			for (int j = 0; j < vec_cari.size(); j++)
			{
				if (search_field == "tag")
				{
					found = this->vec_host[i]->tag.find(vec_cari[j]);
				} else {
					if (search_field == "host_name")
					{
						found = this->vec_host[i]->host_name.find(vec_cari[j]);
					}
				}

				if (found != string::npos)
				{
					ketemu = true;
					ip_host = this->vec_host[i]->ip_addr;
					vec.push_back(this->vec_host[i]->ip_addr);
				}
			}
			i++;
		}

		return ketemu;
	};

	auto cek_format_ip = [](vector<string> &vec, string ip_str)->bool
	{
		bool ketemu = false;

		//isIP4
		ketemu = std::regex_match(ip_str, std::regex(("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b")));

		if (!ketemu)
		{
			//isIP4star
			ketemu = std::regex_match(ip_str, std::regex(("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.[*]?")));
		}

		if (!ketemu)
		{
			//isIP4dashstar
			ketemu = std::regex_match(ip_str, std::regex(("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}-\\d{1,3}\\.[*]?")));
		}

		if (ketemu)
		{
			vec.push_back(ip_str);
		}

		return ketemu;
	};



	if (ip_str == "marx") {
		ip_str = "172.016.114.050";
	}

	if (isString(ip_str))
	{
		vector<string> vec_cari;
		vec_cari.push_back(ip_str);
		if (isi_vec_by_host(vec, "host_name", vec_cari, ip_str))
		{

		} else {
			if (ip_str == "allvin")
			{
				vector<string> vec_cari;
				vec_cari.push_back("in");
				string tmp_str;
				isi_vec_by_host(vec, "tag", vec_cari, tmp_str);

			} else {
				//cout << "string src : " << field->src << endl;
			}
		}
	} else {

		if (!cek_format_ip(vec, ip_str))
		{

			if (isQuote(ip_str))
			{
				if ((ip_str == "\"allpcsandlinux\"") || (ip_str == "\"all.pcs.and.linux\"") || (ip_str == "\"allpc|vlinux\"")) {

					vector<string> vec_cari;
					vec_cari.push_back("pc");
					vec_cari.push_back("linux");
					string tmp_str;
					isi_vec_by_host(vec, "host_name", vec_cari, tmp_str);

				} else {
					size_t found = ip_str.find("|");
					if (found != string::npos)
					{
						string tmp_str = ip_str.substr(1, ip_str.length() - 2);
						vector<string> data = tokenizer((char *)tmp_str.c_str(), "|");
						if (data.size() > 0) {
							for (int i = 0; i < data.size(); ++i)
							{
								if (isString(data[i]))
								{
									if (data[i] == "marx") {
										data[i] = "mars";
									}

									vector<string> vec_cari;
									vec_cari.push_back(data[i]);
									string tmp_str;
									isi_vec_by_host(vec, "host_name", vec_cari, tmp_str);

								} else {
									if (isIP43seg(data[i]))
									{
										vec.push_back(data[i] + ".*");
									}
								}
							}
						}

					}
				}
			} else {
				if (ip_str == "10.different")
				{
					vec.push_back("153.107.252.*");
					vec.push_back("205.231.028.*");
					vec.push_back("207.230.054.*");
					vec.push_back("209.001.012.*");
					vec.push_back("209.012.013.*");
					vec.push_back("209.030.070.*");
					vec.push_back("209.030.071.*");
					vec.push_back("209.074.060.*");

				} else {
					if (ip_str == "all.attackers")
					{
						vec.push_back("128.223.199.068");
						vec.push_back("135.008.060.182");
						vec.push_back("139.134.061.042");
						vec.push_back("152.169.215.104");
						vec.push_back("152.204.242.193");
						vec.push_back("153.010.008.174");
						vec.push_back("153.037.134.017");
						vec.push_back("153.107.252.061");
						vec.push_back("166.102.114.043");
						vec.push_back("172.016.112.050");
						vec.push_back("172.016.114.050");
						vec.push_back("192.005.041.239");
						vec.push_back("196.227.033.189");
						vec.push_back("197.182.091.233");
						vec.push_back("197.218.177.069");
						vec.push_back("199.174.194.016");
						vec.push_back("199.227.099.125");
						vec.push_back("200.027.121.118");
						vec.push_back("202.049.244.010");
						vec.push_back("202.072.001.077");
						vec.push_back("202.077.162.213");
						vec.push_back("202.247.224.089");
						vec.push_back("204.071.051.016");
						vec.push_back("204.097.153.043");
						vec.push_back("204.233.047.021");
						vec.push_back("205.160.208.190");
						vec.push_back("205.180.112.036");
						vec.push_back("205.231.028.163");
						vec.push_back("206.047.098.151");
						vec.push_back("206.048.044.018");
						vec.push_back("206.186.080.111");
						vec.push_back("206.222.003.197");
						vec.push_back("206.229.221.082");
						vec.push_back("207.075.239.115");
						vec.push_back("207.103.080.104");
						vec.push_back("207.114.237.057");
						vec.push_back("207.136.086.223");
						vec.push_back("207.181.092.211");
						vec.push_back("207.230.054.203");
						vec.push_back("207.253.084.013");
						vec.push_back("208.239.005.230");
						vec.push_back("208.240.124.083");
						vec.push_back("208.253.077.185");
						vec.push_back("208.254.251.132");
						vec.push_back("209.001.012.046");
						vec.push_back("209.012.013.144");
						vec.push_back("209.017.189.098");
						vec.push_back("209.030.070.014");
						vec.push_back("209.030.071.165");
						vec.push_back("209.074.060.168");
						vec.push_back("209.117.157.183");
						vec.push_back("209.154.098.104");
						vec.push_back("209.167.099.071");
					} else {
						if (ip_str == "all.outside")
						{
							vector<string> vec_cari;
							vec_cari.push_back("out");
							string tmp_str;
							isi_vec_by_host(vec, "tag", vec_cari, tmp_str);

						} else {
							if ((ip_str == "all.vin") || (ip_str == "allvin"))
							{
								vector<string> vec_cari;
								vec_cari.push_back("in");
								string tmp_str;
								isi_vec_by_host(vec, "tag", vec_cari, tmp_str);

							} else {

								if (ip_str == "8subnets")
								{
									vec.push_back("128.223.199.*");
									vec.push_back("139.134.061.*");
									vec.push_back("202.077.162.*");
									vec.push_back("202.247.224.*");
									vec.push_back("204.071.051.*");
									vec.push_back("204.233.047.*");
									vec.push_back("207.114.237.*");
									vec.push_back("209.001.012.*");

								} else {

									if (ip_str != "*")
									{
										//cout << "src : " << field->src << endl;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void Tolah_label::baca_attack_file()
{
	auto datetime_adjusment = [](ttanggal & tanggal, twaktu & waktu)
	{
		if (waktu.detik >= 60)
		{
			waktu.menit += waktu.detik / 60;
			waktu.detik = waktu.detik % 60;
		} else {
			if (waktu.menit >= 60)
			{
				waktu.jam += waktu.menit / 60;
				waktu.menit = waktu.menit % 60;
			} else {
				if (waktu.jam >= 24)
				{
					tanggal.tanggal += 1;
					waktu.jam = waktu.jam % 24;
					if (tanggal.tanggal > 30)
					{
						tanggal.tanggal = tanggal.tanggal % 30;
						tanggal.bulan += 1;
					}
				}
			}
		}
	};


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
				field->duration = row[9];

				isi_vec_src_dst(field->src, field->vec_src);
				isi_vec_src_dst(field->dst, field->vec_dst);

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
				tmp_waktu.jam += 11;

				datetime_adjusment(attack_tgl, tmp_waktu);

				field->tgl_timestamp = ttanggal_to_timestamp(attack_tgl);
				field->waktu_timestamp = datetime_to_timestamp(attack_tgl, tmp_waktu);

				ttanggal tgl_duration;
				tgl_duration.tanggal = attack_tgl.tanggal;
				tgl_duration.bulan = attack_tgl.bulan;
				tgl_duration.tahun = attack_tgl.tahun;

				twaktu waktu_duration = waktu_frag(field->duration);
				waktu_duration.jam += tmp_waktu.jam;
				waktu_duration.menit += tmp_waktu.menit;
				waktu_duration.detik += tmp_waktu.detik;

				datetime_adjusment(tgl_duration, waktu_duration);

				field->duration_timestamp = datetime_to_timestamp(tgl_duration, waktu_duration);

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

ttanggal Tolah_label::date_frag(string date_str)
{
	ttanggal tmp;

	vector<string> data = tokenizer((char *)date_str.c_str(), "/");

	tmp.bulan = stoi(data[0]);
	tmp.tanggal = stoi(data[1]);
	if (data.size() == 3)
	{
		tmp.tahun = stoi(data[2]);
	}

	return tmp;
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


	for (int i = 0; i < vec_attack.size(); ++i)
	{
		auto itr = vec_map.find(vec_attack[i]->tgl_timestamp);
		if (itr == vec_map.end())
		{
			vec_field_filter tmp_vec;
			tmp_vec.push_back(vec_attack[i]);
			map<long, vec_field_filter> tmp_map;
			tmp_map.insert({vec_attack[i]->waktu_timestamp, tmp_vec});
			vec_map.insert({vec_attack[i]->tgl_timestamp, tmp_map});
		} else {
			map<long, vec_field_filter> *tmp_map = &itr->second;
			auto itr1 = tmp_map->find(vec_attack[i]->waktu_timestamp);
			if (itr1 == tmp_map->end())
			{
				vec_field_filter tmp_vec;
				tmp_vec.push_back(vec_attack[i]);
				tmp_map->insert({vec_attack[i]->waktu_timestamp, tmp_vec});
			} else {
				vec_field_filter *tmp_vec = &itr1->second;
				tmp_vec->push_back(vec_attack[i]);
			}
		}
	}

	// field_filter *previous_field = NULL;
	// auto itr = vec_map.begin();
	// while (itr != vec_map.end())
	// {
	// 	map<long, vec_field_filter> *tmp_map = &itr->second;
	// 	auto itr1 = tmp_map->begin();
	// 	while (itr1 != tmp_map->end())
	// 	{
	// 		vec_field_filter *tmp_vec = &itr1->second;
	// 		auto itr2 = tmp_vec->begin();
	// 		while (itr2 != tmp_vec->end())
	// 		{
	// 			field_filter *field = *itr2;
	// 			// cout  << "first " << itr->first << " "
	// 			//       << itr1->first << " "
	// 			//       << field->duration_timestamp << " "
	// 			//       << field->name << " "
	// 			//       << field->week << " "
	// 			//       << field->day << " "
	// 			//       << field->time << " "
	// 			//       << field->src << " " << field->vec_src.size() << " "
	// 			//       << field->dst << " " << field->vec_dst.size() << endl;

	// 			if (previous_field != NULL)
	// 			{
	// 				if (previous_field->tgl_timestamp == field->tgl_timestamp)
	// 				{
	// 					//previous_field->duration_timestamp = field->waktu_timestamp;
	// 					// cout  << "change " << previous_field->tgl_timestamp << " "
	// 					//       << previous_field->waktu_timestamp << " "
	// 					//       << previous_field->duration_timestamp << " "
	// 					//       << previous_field->name << " "
	// 					//       << previous_field->week << " "
	// 					//       << previous_field->day << " "
	// 					//       << previous_field->time << endl;
	// 				}
	// 			}

	// 			previous_field = *itr2;

	// 			itr2++;
	// 		}

	// 		itr1++;
	// 	}

	// 	itr++;
	// }

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
	auto check_aterixdash = [ = ](string part1, string part2)->bool
	{
		bool is_pass = false;
		vector<string> data;

		if (this->is_subs(part2, "*"))
		{
			is_pass = true;
		}
		else
		{
			if (this->is_subs(part2, "-"))
			{
				data = this->tokenizer((char *)part2.c_str(), "-");
				is_pass = (stoi(part1) >= stoi(data[0])) and (stoi(part1) <= stoi(data[1]));
			}
			else
			{
				is_pass = stoi(part1) == stoi(part2);
			}
		}

		return is_pass;
	};


	bool is_pass = false;

	if (stoi(ip1.part1) == stoi(ip2.part1))
	{
		if (stoi(ip1.part2) == stoi(ip2.part2))
		{
			is_pass = check_aterixdash(ip1.part3, ip2.part3);
			if (is_pass)
			{
				is_pass = check_aterixdash(ip1.part4, ip2.part4);
			}
		}
	}

	return is_pass;
}

bool Tolah_label::is_ip_pass(vector<string> &row, field_filter *field)
{
	auto check_vector = [ = ](tip_fragment ip, vector<string>& vec)->bool
	{
		bool ketemu = false;
		int i = 0;
		while ( !ketemu  && (i < vec.size()))
		{
			tip_fragment tmp_ip = this->ip_frag(vec[i]);
			ketemu = this->compare_ip(ip, tmp_ip);
			i++;
		}
		return ketemu;
	};

	bool is_pass = false;
	bool is_pass_src_src = false , is_pass_dst_dst = false, is_pass_src_dst = false, is_pass_dst_src = false;
	tip_fragment tmp_ip1, tmp_ip2;

	tip_fragment tmp_ip_src = ip_frag(row[1]);
	tip_fragment tmp_ip_dst = ip_frag(row[2]);

	tmp_ip_src.protocol = row[4];
	tmp_ip_dst.protocol = row[4];

	if (field->vec_src.size() > 0)
	{
		is_pass_src_src = check_vector(tmp_ip_src, field->vec_src);
		//is_pass_dst_src = check_vector(tmp_ip_dst, field->vec_src);
	} else {
		if (field->src == "*")
		{
			is_pass_src_src = true;
			is_pass_src_dst = true;
		} else {
			if ((field->src == "src.same.as.dst") || ((field->src == "several") && (field->dst == "several") ))
			{
				is_pass_src_src = compare_ip(tmp_ip_src, tmp_ip_dst);
				is_pass_dst_dst = compare_ip(tmp_ip_src, tmp_ip_dst);
			}
		}
	}

	if (field->vec_dst.size() > 0)
	{
		is_pass_dst_dst = check_vector(tmp_ip_dst, field->vec_dst);
		//is_pass_src_dst = check_vector(tmp_ip_src, field->vec_dst);
	} else {
		if (field->dst == "*")
		{
			is_pass_dst_dst = true;
			is_pass_dst_src = true;
		}
	}

	is_pass = (is_pass_src_src && is_pass_dst_dst) || (is_pass_src_dst && is_pass_dst_src);
	return is_pass;
}

string Tolah_label::labeli(vector<string> row)
{
	string label = "normal.";

	string tmp_row = row[0];


	vector<string> data = tokenizer((char *)tmp_row.c_str(), "-");
	ttanggal tmp_tgl = date_frag(data[0]);
	twaktu tmp_waktu = waktu_frag(data[1]);
	tmp_tgl.tahun = 1998;

	long tgl_timestamp = ttanggal_to_timestamp(tmp_tgl);
	long waktu_timestamp = datetime_to_timestamp(tmp_tgl, tmp_waktu);

	auto itr = vec_map.find(tgl_timestamp);
	if (itr != vec_map.end())
	{
		bool ketemu = false;
		map<long, vec_field_filter> *tmp_map = &itr->second;
		auto itr1 = tmp_map->begin();
		while (!ketemu && (itr1 != tmp_map->end()))
		{
			if (waktu_timestamp >= itr1->first)
			{
				vec_field_filter *tmp_vec = &itr1->second;
				auto itr2 = tmp_vec->begin();
				while ( !ketemu && (itr2 != tmp_vec->end()) )
				{
					field_filter *field = *itr2;
					if (waktu_timestamp <= field->duration_timestamp) {

						if (is_ip_pass(row, field))
						{
							ketemu = true;
							label = field->name + ".";
						}
					}

					itr2++;
				}

			}
			itr1++;
		}

	}

	return label;

}