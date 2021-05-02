#include "Tmap_col_split.h"


Tmap_col_split::Tmap_col_split()
{

}

Tmap_col_split::~Tmap_col_split()
{

}

void Tmap_col_split::add_data(int idx_col, string split_value, string tipe_data, string label, double credal_s)
{

	Tmy_dttype tmp_split_value(split_value, tipe_data == "continuous.");
	auto itr = _pot_split.find(idx_col);
	if (itr == _pot_split.end())
	{
		Tlabel_stat p(credal_s);
		p.add(label);
		map<Tmy_dttype, Tlabel_stat> col_pot_split;
		col_pot_split.insert(pair<Tmy_dttype, Tlabel_stat>(tmp_split_value, p));
		_pot_split.insert(pair<int, map<Tmy_dttype, Tlabel_stat>>(idx_col, col_pot_split));

	} else {
		auto itr1 = itr->second.find(tmp_split_value);
		if (itr1 == itr->second.end())
		{
			Tlabel_stat p(credal_s);
			p.add(label);
			itr->second.insert(pair<Tmy_dttype, Tlabel_stat>(tmp_split_value, p));
		} else {
			itr1->second.add(label);
		}
	}

}

void Tmap_col_split::clear()
{
	for (auto i = _pot_split.begin(); i != _pot_split.end(); ++i)
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
           j->second.clear(); 
		}
		i->second.clear();
	}
	_pot_split.clear();
}

map<Tmy_dttype, Tlabel_stat> Tmap_col_split::get_pot_split(int idx)
{
   return _pot_split[idx];	
}