#include "Tdatetime_holder.h"


Tdatetime_holder::Tdatetime_holder()
{
	_timestamp = 0;
}

Tdatetime_holder::Tdatetime_holder(string date, string time)
{
	vector<string> data_date = global_func::tokenizer((char *)date.c_str(), "/");
	vector<string> data_time = global_func::tokenizer((char *)time.c_str(), ":");

	struct tm t;
	time_t t_of_day;

	t.tm_year = stoi(data_date[2]) - 1900; // Year - 1900
	t.tm_mon = stoi(data_date[0]) - 1;         // Month, where 0 = jan
	t.tm_mday = stoi(data_date[1]);          // Day of the month
	t.tm_hour = stoi(data_time[0]);
	t.tm_min = stoi(data_time[1]);
	t.tm_sec = stoi(data_time[2]);
	t.tm_isdst = 0;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
	_timestamp = ((long) t_of_day);
}

Tdatetime_holder::~Tdatetime_holder()
{

}

void Tdatetime_holder::setTime(string ts)
{
	_timestamp = stol(ts);
}

void Tdatetime_holder::add_time(int h, int m, int s)
{
	_timestamp += (h * 60 * 60) + (m * 60) + s;
}

void Tdatetime_holder::add_time(string time_str)
{
	vector<string> data_time = global_func::tokenizer((char *)time_str.c_str(), ":");
	_timestamp += (stoi(data_time[0]) * 60 * 60) + (stoi(data_time[1]) * 60) + stoi(data_time[2]);
}

ostream & operator << (ostream &out, const Tdatetime_holder &tc)
{
	const time_t rawtime = (const time_t)tc._timestamp;

	struct tm * dt;
	char timestr[30];
	char buffer [30];

	dt = localtime(&rawtime);
	// use any strftime format spec here
	strftime(timestr, sizeof(timestr), "%m/%d/%Y %H:%M:%S", dt);
	sprintf(buffer, "%s", timestr);
	string stdBuffer(buffer);
    out << stdBuffer;  
   
	return out;
}

