
#include <map>
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"
#include "global.h"

using namespace std;

#ifndef Included_Tmap_col_split_H

#define Included_Tmap_col_split_H

class Tmap_col_split
{

map<int, map<Tmy_dttype, Tlabel_stat>> _pot_split;

Tconfig config;

public:
	Tmap_col_split();
	~Tmap_col_split();
	void add_data(int idx_col,string split_value,string tipe_data,string label);
	map<Tmy_dttype, Tlabel_stat> get_pot_split(int idx);
	void clear();

    void set_config(Tconfig v_config);
	
};

#endif