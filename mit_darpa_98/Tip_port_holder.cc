#include "Tip_port_holder.h"


Tip_port_holder::Tip_port_holder()
{

}

Tip_port_holder::Tip_port_holder(string ip_port)
{

	vector<string> data;
	data = global_func::tokenizer((char *)ip_port.c_str(), ":");
	_port = stoi(data[1]);

	data = global_func::tokenizer((char *)data[0].c_str(), ".");
	_part1 = stoi(data[0]);
	_part2 = stoi(data[1]);
	_part3 = stoi(data[2]);
	_part4 = stoi(data[3]);

}

Tip_port_holder::Tip_port_holder(string ip, string port)
{

	vector<string> data;

	data = global_func::tokenizer((char *)ip.c_str(), ".");
	_part1 = stoi(data[0]);
	_part2 = stoi(data[1]);
	_part3 = stoi(data[2]);
	_part4 = stoi(data[3]);
	_port = stoi(port);
}
Tip_port_holder::~Tip_port_holder() {

}

ostream & operator << (ostream &out, const Tip_port_holder &tc)
{
	out << tc._part1 << "." << tc._part2 << "." << tc._part3 << "." << tc._part4 << ":" << tc._port;
	return out;
}