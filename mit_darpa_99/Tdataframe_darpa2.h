#include "global_func.h"
#include "Tbase_dataframe.h"
#include "Tfield_filter_darpa.h"
#include "Tdatetime_holder.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;


#ifndef Included_Tdataframe_darpa2_H

#define Included_Tdataframe_darpa2_H

typedef vector<Tfield_filter_darpa> vec_field_filter;

class Tdataframe_darpa2 : public Tbase_dataframe 
{
private:
	
	map<Tdatetime_holder, vec_field_filter> vec_map;

public:
	
	
	void stat_tabel() override;
	
	string search(string date_start, string hour_start,string date_end, string hour_end, string ip_src, string port_src, string ip_dst, string port_dst, string service);
    
    
};


#endif