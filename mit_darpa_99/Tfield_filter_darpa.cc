#include "Tfield_filter_darpa.h"


Tfield_filter_darpa::Tfield_filter_darpa()
{

}

Tfield_filter_darpa::~Tfield_filter_darpa()
{
	_Attacker.clear();
	_Attacker.shrink_to_fit();
	_Victim.clear();
	_Victim.shrink_to_fit();
	_At_Attacker.clear();
	_At_Attacker.shrink_to_fit();
	_At_Victim.clear();
	_At_Victim.shrink_to_fit();
}

void Tfield_filter_darpa::isi_ip_port_vec(vector<string>& vec_ip, vector<string>& vec_port, vector<Tip_port_holder>& vec_ip_port)
{
	if (vec_ip.size() > 0)
	{
		for (int i = 0; i < vec_ip.size(); ++i)
		{
			if (vec_port.size() > 0)
			{
				for (int j = 0; j < vec_port.size(); ++j)
				{

					if ((vec_ip[i] != "console") and (vec_ip[i] != "login.session.x.x") and (vec_ip[i] != "hobbes_console")) {
						//cout << _Attacker[i] << " " << _At_Attacker[j] << endl;
						Tip_port_holder tmp_ip(vec_ip[i], vec_port[j]);
						//cout << tmp_ip << endl;
						vec_ip_port.push_back(tmp_ip);

					}

				}
			} else {

				/*if ((vec_ip[i] != "console") and (vec_ip[i] != "login.session.x.x") and (vec_ip[i] != "hobbes_console")) {
					//cout << _Attacker[i] << " - "<< endl;
					Tip_port_holder tmp_ip(vec_ip[i], "-");
					//cout << tmp_ip << endl;
					vec_ip_port.push_back(tmp_ip);
				}*/
			}
		}
	}
}

void Tfield_filter_darpa::parser(vector<string> _data)
{
	if (global_func::is_subs(_data[0], "ID: "))
	{
		_ID = _data[0].substr(4);
		global_func::remove_cr(_ID);
		//cout << _ID << endl;

	}

	if (global_func::is_subs(_data[1], "Date: "))
	{
		_Date = Tdatetime_holder(_data[1].substr(6), "00:00:00");
	}

	if (global_func::is_subs(_data[2], "Name: "))
	{
		_Name = _data[2].substr(6);
		global_func::remove_cr(_Name);
	}

	if (global_func::is_subs(_data[3], "Category: "))
	{
		_Category = _data[3].substr(10);
		global_func::remove_cr(_Category);
	}

	if (global_func::is_subs(_data[4], "Start_Time: "))
	{
		_Start_Time =  Tdatetime_holder(_data[1].substr(6), _data[4].substr(12));
		_Start_Time.add_time(11, 0, 0);

		Tdatetime_holder tmp_datetime(_data[1].substr(6), "23:59:59");
		if (tmp_datetime < _Start_Time)
		{
			_Date.add_time(24, 0, 0);
		}
	}

	if (global_func::is_subs(_data[5], "Duration: "))
	{
		_Duration =  Tdatetime_holder(_data[1].substr(6), _data[4].substr(12));
		_Duration.add_time(11, 0, 0);
		_Duration.add_time(_data[5].substr(10));
	}

	if (global_func::is_subs(_data[6], "Attacker: "))
	{
		string tmp_str = _data[6].substr(10);
		vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), ", ");

		if (data.size() == 0)
		{
			if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
			{
				_Attacker.push_back(tmp_str);
			}
		}
		else
		{
			for (int i = 0; i < data.size(); ++i)
			{
				tmp_str = data[i];
				if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
				{
					_Attacker.push_back(tmp_str);
				}
			}
		}
	}

	if (global_func::is_subs(_data[7], "Victim: "))
	{
		string tmp_str = _data[7].substr(8);
		vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), ", ");

		if (data.size() == 0)
		{
			if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
			{
				_Victim.push_back(tmp_str);
			}
		}
		else
		{
			for (int i = 0; i < data.size(); ++i)
			{
				tmp_str = data[i];
				if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != "") )
				{
					_Victim.push_back(tmp_str);
				}
			}
		}
	}

	if (global_func::is_subs(_data[8], "Username: "))
	{
		_Username = _data[8].substr(10);
		global_func::remove_cr(_Username);
	}

	size_t posisi = _data[10].find("At_Attacker: ");
	if (posisi != string::npos)
	{
		string tmp_str = _data[10].substr(posisi + 13);
		if (global_func::is_subs(tmp_str, "{"))
		{
			vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), ", ");

			if (data.size() == 0)
			{
				if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
				{
					_At_Attacker.push_back(tmp_str);
				}
			}
			else
			{
				for (int i = 0; i < data.size(); ++i)
				{
					tmp_str = data[i];
					if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
					{
						_At_Attacker.push_back(tmp_str);
					}
				}
			}
		}
	}

	posisi = _data[11].find("At_Victim: ");
	if (posisi != string::npos)
	{
		string tmp_str = _data[11].substr(posisi + 11);
		if (global_func::is_subs(tmp_str, "{"))
		{
			vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), ", ");

			if (data.size() == 0)
			{
				if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
				{
					_At_Victim.push_back(tmp_str);
				}
			}
			else
			{
				for (int i = 0; i < data.size(); ++i)
				{
					tmp_str = data[i];
					if ((tmp_str.length() > 0) and (tmp_str != "\0") and (tmp_str != ""))
					{
						_At_Victim.push_back(tmp_str);
					}
				}
			}
		}
	}


	if (_At_Attacker.size() > 0)
	{
		string tmp_str;
		for (int i = 0; i < _At_Attacker.size(); ++i)
		{
			tmp_str = _At_Attacker[i];
			size_t posisi = tmp_str.find("{");
			if (posisi != string::npos)
			{
				tmp_str = tmp_str.substr(0, posisi);
				_At_Attacker[i] = tmp_str;
			}
		}

	}

	if (_At_Victim.size() > 0)
	{
		string tmp_str;
		for (int i = 0; i < _At_Victim.size(); ++i)
		{
			tmp_str = _At_Victim[i];
			size_t posisi = tmp_str.find("{");
			if (posisi != string::npos)
			{
				tmp_str = tmp_str.substr(0, posisi);
				_At_Victim[i] = tmp_str;
			}
		}

	}


	isi_ip_port_vec(_Attacker, _At_Attacker, ip_port_src);
	isi_ip_port_vec(_Victim, _At_Victim, ip_port_dst);

	isi_ip_port_vec(_Attacker, _At_Victim, ip_port_src);
	isi_ip_port_vec(_Victim, _At_Attacker, ip_port_dst);

	isi_ip_port_vec(_Victim, _At_Attacker, ip_port_src);
	isi_ip_port_vec(_Attacker, _At_Victim, ip_port_dst);

}

Tdatetime_holder Tfield_filter_darpa::get_date()
{
	return _Date;
}

bool Tfield_filter_darpa::cari_ip_port_src(Tip_port_holder ip_port)
{
	bool is_lolos = false;

	if (ip_port_src.size() > 0)
	{
		size_t i = 0;
		while ((i < ip_port_src.size()) and !is_lolos)
		{
			//cout << _ID << " " << ip_port_src[i] << "==" << ip_port << " " << is_lolos << endl;
			is_lolos = ip_port_src[i] == ip_port;

			i++;
		}
	}

	return is_lolos;
}

bool Tfield_filter_darpa::cari_ip_port_dst(Tip_port_holder ip_port)
{
	bool is_lolos = false;

	if (ip_port_dst.size() > 0) {
		size_t j = 0;
		while ((j < ip_port_dst.size()) and !is_lolos)
		{
			//cout << _ID <<" " << ip_port_dst[j] << "==" << ip_port << " " << is_lolos << endl;
			is_lolos = ip_port_dst[j] == ip_port;

			j++;
		}
	}

	return is_lolos;
}

bool Tfield_filter_darpa::is_pass(string date_start, string hour_start, string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service)
{
	bool is_lolos = false;

	Tdatetime_holder start(date_start, hour_start);
	Tdatetime_holder end(date_end, hour_end);

	Tip_port_holder cari_ip_src(ip_src, port_src, service);
	Tip_port_holder cari_ip_dst(ip_dst, port_dst, service);


	if (((_Start_Time <= start) and (end < _Duration)) or ((_Start_Time <= start) and (_Duration < end)) or ((_Start_Time > start) and (end < _Duration)))
	{
		bool is_lolos1 = cari_ip_port_src(cari_ip_src);
		bool is_lolos2 = cari_ip_port_dst(cari_ip_dst);
		is_lolos = is_lolos1 and is_lolos2;

		if (!is_lolos)
		{
			is_lolos1 = cari_ip_port_src(cari_ip_dst);
			is_lolos2 = cari_ip_port_dst(cari_ip_src);
			is_lolos = is_lolos1 and is_lolos2;
		}

	}


	return is_lolos;
}

void Tfield_filter_darpa::info()
{
	cout << setw(30) << _ID << setw(10) << _Attacker.size() << setw(10) << _At_Attacker.size() << setw(10) << _Victim.size() << setw(10) << _At_Victim.size() << endl;
}