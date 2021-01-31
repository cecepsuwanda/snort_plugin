
#include "global_func.h"
#include "Tbase_dataframe.h"
#include "Trange_holder.h"
#include "Tip_port_holder.h"
#include "Tdatetime_holder.h"
#include <iterator>
#include <regex>
#include <ctime>

#ifndef Included_Tdataframe_list_H

#define Included_Tdataframe_list_H

struct list_item
{
	Tip_port_holder ip_src;
	Tip_port_holder ip_dst;
	string service;
	string label;
	Tdatetime_holder date_start;
	Trange_holder range_holder;
	int swap_flag = 0;
};

typedef vector<Trange_holder> map_list4;
typedef map<Tdatetime_holder, map_list4> map_list3;
typedef map<string, map_list3> map_list2;
typedef map<Tip_port_holder, map_list2> map_list1;
typedef map<Tip_port_holder, map_list1> map_list;

class Tdataframe_list : public Tbase_dataframe 
{

private:
	

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

    bool is_pass(vector<string> &data) override;		
	
	void stat_tabel() override;
	
	string search(string date_start, string hour_start,string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service);

	
};

#endif