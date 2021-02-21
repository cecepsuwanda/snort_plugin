#include "Tip_port_holder.h"


Tip_port_holder::Tip_port_holder()
{
	ip_asterik = false;
	ip_selang = false;

	port_ignore = false;
	port_selang = false;

	cek_protocol = false;
}

Tip_port_holder::Tip_port_holder(string ip_port)
{
    _protocol = "tcp";

	vector<string> data;
	data = global_func::tokenizer((char *)ip_port.c_str(), ":");
	if (data[1] == "-")
	{
		_port = -1;
	} else {
		_port = stoi(data[1]);
	}

	_port_str = "-";

	data = global_func::tokenizer((char *)data[0].c_str(), ".");
	_part1 = stoi(data[0]);
	_part2 = stoi(data[1]);
	_part3 = stoi(data[2]);
	_part4 = stoi(data[3]);
	_part4_str = "-";

	ip_asterik = false;
	ip_selang = false;

	port_ignore = false;
	port_selang = false;

	cek_protocol = false;

}

Tip_port_holder::Tip_port_holder(string ip, string port)
{
    _protocol = "tcp";
	
	cek_protocol = false;

	vector<string> data;

	ip_asterik = false;
	ip_selang = false;

	port_ignore = false;
	port_selang = false;



	data = global_func::tokenizer((char *)ip.c_str(), ".");
	_part1 = stoi(data[0]);
	_part2 = stoi(data[1]);
	_part3 = stoi(data[2]);

	if (global_func::isNumber(data[3]))
	{
		_part4 = stoi(data[3]);
		_part4_str = "-";
	} else {
		_part4_str = data[3];

		ip_asterik = data[3] == "*";
		ip_selang = global_func::is_subs(data[3], "-");

	}



	if (port == "-") {
		port_ignore = true;
		_port = -1;
		_port_str = "-";
	}
	else {

		if (port == "i")
		{
			_protocol = "icmp";
			port_ignore = true;
			_port = -1;
			_port_str = "i";
			cek_protocol = true;

		} else {

			string tmp_port1, tmp_port2, tmp_port3, tmp_port4, tmp_port5, tmp_port6;

			if (global_func::is_subs(port, "-"))
			{
				_port_str = port;
				port_selang = true;
				_port = -1;

				data = global_func::tokenizer((char *)port.c_str(), "-");
				tmp_port1 = data[0];
				tmp_port2 = data[1];

				if (global_func::is_subs(tmp_port1, "/"))
				{
					data = global_func::tokenizer((char *)tmp_port1.c_str(), "/");
					tmp_port3 = data[0];
					tmp_port4 = data[1];

					tmp_port1 = tmp_port3;
				}

				if (global_func::is_subs(tmp_port2, "/"))
				{
					data = global_func::tokenizer((char *)tmp_port2.c_str(), "/");
					tmp_port5 = data[0];
					tmp_port6 = data[1];

					tmp_port2 = tmp_port5;
				}

				if ((tmp_port3 == tmp_port5) and (tmp_port5 == "u"))
				{
					_protocol = "udp";
					cek_protocol = true;
				}

			} else {

				if (global_func::is_subs(port, "/"))
				{
					_port_str = port;

					data = global_func::tokenizer((char *)port.c_str(), "/");
					tmp_port1 = data[0];
					tmp_port2 = data[1];

					_port = stoi(tmp_port1);
					port_selang = false;
					port_ignore = false;


					if (tmp_port2 == "u")
					{
						_protocol = "udp";
						cek_protocol = true;
					}


				} else {

					if (global_func::isNumber(port))
					{
						_port = stoi(port);
						_port_str = "-";
						port_selang = false;
						port_ignore = false;
					} else {
						_port_str = port;
						_port = -1;
						port_ignore = true;
					}
				}
			}
		}
	}

}

Tip_port_holder::Tip_port_holder(string ip, string port, string protocol)
{

	vector<string> data;

	data = global_func::tokenizer((char *)ip.c_str(), ".");
	_part1 = stoi(data[0]);
	_part2 = stoi(data[1]);
	_part3 = stoi(data[2]);
	_part4 = stoi(data[3]);
	_part4_str = "-";
	_port = stoi(port);
	_port_str = "-";

	_protocol = protocol;

	ip_asterik = false;
	ip_selang = false;

	port_ignore = false;
	port_selang = false;

	cek_protocol = false;

}


Tip_port_holder::~Tip_port_holder() {

}



ostream & operator << (ostream &out, const Tip_port_holder &tc)
{
	if (!tc.port_selang and !tc.port_ignore and !tc.ip_selang and !tc.ip_asterik) {
		out << tc._protocol << " " << tc._part1 << "." << tc._part2 << "." << tc._part3 << "." << tc._part4 << ":" << tc._port;
	} else {
		if ((tc.port_selang or tc.port_ignore) and !tc.ip_selang and !tc.ip_asterik) {
			out << tc._protocol << " " << tc._part1 << "." << tc._part2 << "." << tc._part3 << "." << tc._part4 << ":" << tc._port_str;
		} else {
			if (!tc.port_selang and !tc.port_ignore and (tc.ip_selang or tc.ip_asterik)) {
				out << tc._protocol << " " << tc._part1 << "." << tc._part2 << "." << tc._part3 << "." << tc._part4_str << ":" << tc._port;
			} else {
				if ((tc.port_selang or tc.port_ignore) and (tc.ip_selang or tc.ip_asterik)) {
					out << tc._protocol << " " << tc._part1 << "." << tc._part2 << "." << tc._part3 << "." << tc._part4_str << ":" << tc._port_str;
				}
			}

		}
	}

	return out;
}