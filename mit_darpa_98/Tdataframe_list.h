#include "Tread_file.h"
#include "global_func.h"
#include "Tdatetime_holder.h"
#include "Tip_port_holder.h"
#include <map>
#include <iterator>
#include <string>
#include <regex>
#include <ctime>

using namespace std;


#ifndef Included_Tdataframe_list_H

#define Included_Tdataframe_list_H



struct field_filter
{
	int idx_col;
	int idx_opt;
	string value;
};




class Tmy_datetime_holder
{
private:
	string _start;
	string _label;
	long _start_timestamp;
	long _end_timestamp;

public:
	void set_property(string start, string end, int tmb, string label);

	bool operator <(const Tmy_datetime_holder& rhs) const
	{
		return _start_timestamp < rhs._start_timestamp;
	}

	bool operator ==(const Tmy_datetime_holder& rhs) const
	{
		return _start_timestamp == rhs._start_timestamp;
	}

};

struct list_item
{
	string str_ip_src;
	string str_ip_dst;
	string service;
	string label;
	Tmy_datetime_holder datetime_holder;
};

typedef map<string, int> map_list4;
typedef map<Tmy_datetime_holder, map_list4> map_list3;
typedef map<string, map_list3> map_list2;
typedef map<string, map_list2> map_list1;
typedef map<string, map_list1> map_list;

class Tdataframe_list
{

private:
	Tread_file _data;
	vector<field_filter> _filter;
	map<string, int> _stat_label;

	bool is_index = false;

	int _jml_row = 0;





	map_list _idx_list;

	void build_idx_list_lvl0(list_item tmp_list);
	void build_idx_list_lvl1(list_item tmp_list, map_list1* map1);
	void build_idx_list_lvl2(list_item tmp_list, map_list2* map2);
	void build_idx_list_lvl3(list_item tmp_list, map_list3* map3);

	string search_idx_list_lvl0(list_item tmp_list);
	string search_idx_list_lvl1(list_item tmp_list, map_list1* map1);
	string search_idx_list_lvl2(list_item tmp_list, map_list2* map2);
	string search_idx_list_lvl3(list_item tmp_list, map_list3* map3);

	bool service_not_in(string service);

	list_item preproses_item(vector<string> &data);

public:
	Tdataframe_list();
	~Tdataframe_list();

	bool is_pass(vector<string> &data);
	int getjmlrow();
	void read_data(string nm_f);
	void add_filter(field_filter filter);
	void clear_filter();
	void stat_tabel();
	map<string, int> get_stat_label();
	string search(string date, string hour, string ip_src, string port_src, string ip_dst, string port_dst, string service);

	void clear_memory();
};

#endif