#include "global_func.h"
#include "Tread_file_darpa.h"
#include "Tfield_filter_darpa.h"
#include "Tdatetime_holder.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;


#ifndef Included_Tdataframe_darpa_H

#define Included_Tdataframe_darpa_H

typedef vector<Tfield_filter_darpa> vec_field_filter;

class Tdataframe_darpa
{
private:
	Tread_file_darpa _data;
	string _nm_file;

	map<Tdatetime_holder, vec_field_filter> vec_map;



public:
	Tdataframe_darpa();
	~Tdataframe_darpa();

	void read_data(string nm_f);
	void stat_tabel();

	bool open_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

	string search(string date_start, string hour_start,string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service);
    
    void info();
};


#endif