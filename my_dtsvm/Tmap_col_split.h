
#include <map>
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"

using namespace std;

#ifndef Included_Tmap_col_split_H

#define Included_Tmap_col_split_H

class Tmap_col_split
{

map<int, map<Tmy_dttype, Tlabel_stat>> _pot_split;

public:
	Tmap_col_split();
	~Tmap_col_split();
	void add_data(int idx_col,string split_value,string tipe_data,string label,double credal_s);
	map<Tmy_dttype, Tlabel_stat> get_pot_split(int idx);
	void clear();
	
};

#endif