
#include "global_func.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#ifndef Included_Tip_port_holder_H

#define Included_Tip_port_holder_H

class Tip_port_holder
{
private:
	int _part1;
	int _part2;
	int _part3;
	int _part4;
	string _part4_str;
	int _port;
	string _port_str;
	string _protocol;

	bool ip_asterik;
	bool ip_selang;

	bool port_ignore;
	bool port_selang;

	bool cek_protocol;

public:
	Tip_port_holder();
	Tip_port_holder(string ip_port);
	Tip_port_holder(string ip, string port);
	Tip_port_holder(string ip, string port, string protocol);
	~Tip_port_holder();



	Tip_port_holder& operator = (const Tip_port_holder &t)
	{
		this->_part1 = t._part1;
		this->_part2 = t._part2;
		this->_part3 = t._part3;
		this->_part4 = t._part4;
		this->_part4_str = t._part4_str;
		this->_port = t._port;
		this->_port_str = t._port_str;
		this->_protocol = t._protocol;

		this->ip_asterik = t.ip_asterik;
		this->ip_selang = t.ip_selang;

		this->port_ignore = t.port_ignore;
		this->port_selang = t.port_selang;

		this->cek_protocol = t.cek_protocol;

		return *this;
	}

	bool operator ==(const Tip_port_holder& rhs) const
	{

		bool is_pass = false;

		// if (_protocol == rhs._protocol)
		// {
		//bool ip_3part_same = false;

		int ip_int_lhs = ((((_part1 * 255) + _part2) * 255) + _part3);
		int ip_int_rhs = ((((rhs._part1 * 255) + rhs._part2) * 255) + rhs._part3);

		// if (ip_int_lhs == ip_int_rhs)
		// {
		// 	ip_3part_same = true;

		// 	cout << _protocol << " ";

		// 	if (ip_asterik or ip_selang)
		// 	{
		// 		cout << _part4_str << " ";
		// 	} else {
		// 		cout << _part4 << " ";
		// 	}

		// 	if (port_ignore or port_selang)
		// 	{
		// 		cout << _port_str << " ";

		// 	} else {
		// 		cout << _port << " ";
		// 	}

		// 	cout << "==" << rhs._protocol << " " << rhs._part4 << " " << rhs._port << " ";
		// }

		int ip_atas = 0;
		int ip_bawah = 0;


		if (!ip_asterik and !ip_selang)
		{
			ip_int_lhs = ( ip_int_lhs * 255) + _part4;
			ip_int_rhs = ( ip_int_rhs * 255) + rhs._part4;

		} else {
			if (ip_selang)
			{
				ip_int_rhs = ip_int_rhs = ( ip_int_rhs * 255) + rhs._part4;

				string tmp_str = _part4_str;

				vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), "-");

				int bawah = stoi(data[0]);
				int atas = stoi(data[1]);

				ip_bawah = (ip_int_lhs * 255) + bawah;
				ip_atas = (ip_int_lhs * 255) + atas;

			}
		}


		if (!port_selang and !port_ignore)
		{
			if (ip_selang)
			{
				ip_bawah =  (ip_bawah * 255) + _port; //(_port == -1 ? ip_int_lhs : )
				ip_atas =  (ip_atas * 255) + _port; //(_port == -1 ? ip_int_lhs : )
				ip_int_rhs =  (ip_int_rhs * 255) + rhs._port; //(rhs._port == -1 ? ip_int_rhs :)

				is_pass = (ip_bawah <= ip_int_rhs) and (ip_int_rhs <= ip_atas);

			} else {
				ip_int_lhs =  (ip_int_lhs * 255) + _port; //(_port == -1 ? ip_int_lhs : )
				ip_int_rhs =  (ip_int_rhs * 255) + rhs._port; //(rhs._port == -1 ? ip_int_rhs :)

				is_pass = ip_int_lhs == ip_int_rhs;

			}
		} else {
			if (port_ignore)
			{
				if (ip_selang)
				{
					is_pass = (ip_bawah <= ip_int_rhs) and (ip_int_rhs <= ip_atas);
				} else {
					is_pass = ip_int_lhs == ip_int_rhs;
				}
			} else {
				if (port_selang)
				{
					string tmp_str = _port_str;
					vector<string> data = global_func::tokenizer((char *)tmp_str.c_str(), "-");

					int port_bawah = stoi(data[0]);
					int port_atas = stoi(data[1]);


					if (ip_selang)
					{
						is_pass = (ip_bawah <= ip_int_rhs) and (ip_int_rhs <= ip_atas);

						if (is_pass)
						{
							is_pass = (port_bawah <= rhs._port) and (rhs._port <= port_atas);
						}

					} else {
						ip_bawah =  (ip_int_lhs * 255) + port_bawah;
						ip_atas =  (ip_int_lhs * 255) + port_atas;
						ip_int_rhs =  (ip_int_rhs * 255) + rhs._port;

						is_pass = (ip_bawah <= ip_int_rhs) and (ip_int_rhs <= ip_atas);
					}
				}
			}
		}

		if (cek_protocol)
		{
			 is_pass = is_pass and  (_protocol == rhs._protocol);
		}

		// if (ip_3part_same)
		// {
		// 	cout << is_pass << endl;
		// }

		// }



		return is_pass;
	}

	bool operator <(const Tip_port_holder& rhs) const
	{
		int ip_in_int = (((((_part1 * 255) + _part2) * 255) + _part3) * 255) + _part4;
		ip_in_int = (_port == -1 ? ip_in_int : (ip_in_int * 255) + _port);

		int ip_in_int1 = (((((rhs._part1 * 255) + rhs._part2) * 255) + rhs._part3) * 255) + rhs._part4;
		ip_in_int1 = (rhs._port == -1 ? ip_in_int1 : (ip_in_int1 * 255) + rhs._port);

		return ip_in_int < ip_in_int1;
	}

	operator string() const {
		return to_string(_part1) + "." + to_string(_part2) + "." + to_string(_part3) + "." + to_string(_part4) + ":" + to_string(_port);
	}

	friend ostream & operator << (ostream & out, const Tip_port_holder & tc);

};

#endif