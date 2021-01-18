#include "Tread_file.h"
#include <map>
#include <iterator>
#include <string>
#include <regex>
#include <ctime>

using namespace std;


#ifndef Included_Tdataframe_list_H

#define Included_Tdataframe_list_H


struct list_item
{
   long timestamp;
   string str_ip_src;
   string str_ip_dst;
   string service;
   string label;
};

struct field_filter
{
	int idx_col;
	int idx_opt;
	string value;
};


struct tip_fragment
{
	int part1;
	int part2;
	int part3;
	int part4;
	int port;
};


typedef map<long, vector<string>> map_list3;
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

	tip_fragment ip_frag(string ip, string port);
	long datetime_to_timestamp(string date, string hour, int tmb);
	bool compare_ip(tip_fragment ip1, tip_fragment ip2);
	string ip_fragment_str(tip_fragment ip_frag);

	bool is_subs(string stack, string needle);
	bool isNumber(string token);

	vector<string> tokenizer(char *str, const char *separator);

	map_list _idx_list;

	void build_idx_list_lvl0(list_item tmp_list);
	void build_idx_list_lvl1(list_item tmp_list,map_list1* map1);
	void build_idx_list_lvl2(list_item tmp_list,map_list2* map2);
	void build_idx_list_lvl3(list_item tmp_list,map_list3* map3);

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