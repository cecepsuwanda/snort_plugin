
#include "global_func.h"
#include <vector>
#include <string>


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
	int _port;

public:
	Tip_port_holder();
	Tip_port_holder(string ip_port);
	Tip_port_holder(string ip, string port);
	~Tip_port_holder();

	

	Tip_port_holder& operator = (const Tip_port_holder &t)
	{
		this->_part1 = t._part1;
		this->_part2 = t._part2;
		this->_part3 = t._part3;
		this->_part4 = t._part4;
		this->_port = t._port;

		return *this;
	}

	bool operator ==(const Tip_port_holder& rhs) const
	{
		

		int ip_in_int = (((((_part1 * 255) + _part2) * 255) + _part3) * 255) + _part4;
		int ip_in_int1 = (((((rhs._part1 * 255) + rhs._part2) * 255) + rhs._part3) * 255) + rhs._part4;
		
		if((_port!=-1) and (rhs._port!=-1)){
         ip_in_int = (_port == -1 ? ip_in_int : (ip_in_int * 255) + _port);
		 ip_in_int1 = (rhs._port == -1 ? ip_in_int1 : (ip_in_int1 * 255) + rhs._port);
        }

		return ip_in_int==ip_in_int1;
	}

	bool operator <(const Tip_port_holder& rhs) const
	{
		int ip_in_int = (((((_part1 * 255) + _part2) * 255) + _part3) * 255) + _part4;
		ip_in_int = (_port == -1 ? ip_in_int : (ip_in_int * 255) + _port);

		int ip_in_int1 = (((((rhs._part1 * 255) + rhs._part2) * 255) + rhs._part3) * 255) + rhs._part4;
		ip_in_int1 = (rhs._port == -1 ? ip_in_int1 : (ip_in_int1 * 255) + rhs._port);

        return ip_in_int<ip_in_int1;
	}

	operator string() const {
		return to_string(_part1) + "." + to_string(_part2) + "." + to_string(_part3) + "." + to_string(_part4) + ":" + to_string(_port);
	}

	friend ostream & operator << (ostream & out, const Tip_port_holder & tc);

};

#endif