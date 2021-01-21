#include "global_func.h"
#include <vector>
#include <string>

using namespace std;

#ifndef Included_Tdatetime_holder_H

#define Included_Tdatetime_holder_H

class Tdatetime_holder
{
private:
	long _timestamp;

public:
	Tdatetime_holder();
	Tdatetime_holder(string date, string time);
	~Tdatetime_holder();

	void add_time(int h,int m,int s);
	void add_time(string time_str);

	Tdatetime_holder& operator = (const Tdatetime_holder &t)
	{
		this->_timestamp = t._timestamp;

		return *this;
	}

	bool operator ==(const Tdatetime_holder& rhs) const
	{

		return _timestamp == rhs._timestamp;
	}

	bool operator <(const Tdatetime_holder& rhs) const
	{

		return _timestamp < rhs._timestamp;
	}

	operator string() const {

		const time_t rawtime = (const time_t)_timestamp;

		struct tm * dt;
		char timestr[30];
		char buffer [30];

		dt = localtime(&rawtime);
		// use any strftime format spec here
		strftime(timestr, sizeof(timestr), "%m/%d/%Y %H:%M:%S", dt);
		sprintf(buffer, "%s", timestr);
		string stdBuffer(buffer);


		return stdBuffer;
	}

	friend ostream & operator << (ostream &out, const Tdatetime_holder &tc);

};

#endif