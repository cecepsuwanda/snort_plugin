#include "global_func.h"
#include "Tdatetime_holder.h"
#include "Tip_port_holder.h"
#include <iomanip>
#include <vector>
#include <iostream>
#include <string>

using namespace std;


#ifndef Included_Tfield_filter_darpa_H

#define Included_Tfield_filter_darpa_H

class Tfield_filter_darpa
{
private:
	string _ID;
	Tdatetime_holder _Date;
	string _Name;
	string _Category;
	Tdatetime_holder _Start_Time;
	Tdatetime_holder _Duration;
	vector<string> _Attacker;
	vector<string> _Victim;
	string _Username;
	vector<string> _At_Attacker;
	vector<string> _At_Victim;

	vector<Tip_port_holder> ip_port_src;
	vector<Tip_port_holder> ip_port_dst;

	bool cari_ip_port_src(Tip_port_holder ip_port);
	bool cari_ip_port_dst(Tip_port_holder ip_port);

	void isi_ip_port_vec(vector<string>& vec_ip, vector<string>& vec_port, vector<Tip_port_holder>& vec_ip_port);

public:
	Tfield_filter_darpa();
	~Tfield_filter_darpa();

	void parser(vector<string> _data);
	Tdatetime_holder get_date();

	bool is_pass(string date_start, string hour_start, string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service);

	void info();
};


#endif