
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
		bool is_pass = false;

		if (_part1 == rhs._part1)
		{
			if (_part2 == rhs._part2)
			{
				if (_part3 == rhs._part3)
				{
					if (_part4 == rhs._part4)
					{
						if (_port == rhs._port)
						{
							is_pass = true;
						}
					}

				}
			}
		}

		return is_pass;
	}

	operator string() const {
		return to_string(_part1) + "." + to_string(_part2) + "." + to_string(_part3) + "." + to_string(_part4) + ":" + to_string(_port);
	}

	friend ostream & operator << (ostream &out, const Tip_port_holder &tc);

};

#endif