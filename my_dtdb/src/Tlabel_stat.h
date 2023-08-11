#include "Tmy_dttype.h"
#include <map>
#include <cmath>
#include <iterator>
#include <string>
#include <iomanip>
#include <algorithm>
#include "credal.h"
#include "global.h"

using namespace std;

#ifndef Included_Tlabel_stat_H

#define Included_Tlabel_stat_H

class Tlabel_stat
{
private:
	int _jml_row;
	map<string, int> _map;
	string _max_label;
	int _max_value;
	int _min_value;
	Tglobal_config global_config;

	void cari_max_label();

public:
	Tlabel_stat();
	~Tlabel_stat();

	void add(string value);
	void add(string value, int count);
	void clear();
	int get_jml_row();

	Tmy_dttype get_entropy();
	Tmy_dttype get_credal_entropy();

	float get_estimate_error();
	map<string, int> get_map();
	int get_value(string key);

	int get_jml_row_in_map();
	string get_first_value_in_map();

	string get_max_label();
	bool is_single_label();
	bool is_more_half();
	int get_jml_stat(string label);   

	Tlabel_stat& operator = (const Tlabel_stat &t)
	{
		this->_jml_row = t._jml_row;

		this->_map.clear();

		for (auto it = t._map.begin(); it != t._map.end(); it++)
		{
			this->_map.insert(pair<string, int>(it->first, it->second));
		}

		this->_max_value = t._max_value;
		this->_max_label = t._max_label;
		this->_min_value = t._min_value;

		return *this;
	}

	const Tlabel_stat operator + (const Tlabel_stat &t) const
	{
		Tlabel_stat tmp;

		tmp._jml_row =  this->_jml_row + t._jml_row;

		tmp._map.clear();

		for (auto it = t._map.begin(); it != t._map.end(); it++)
		{
			auto it1 = this->_map.find(it->first);
			if (it1 == this->_map.end())
			{
				tmp._map.insert(pair<string, int>(it->first, it->second));
			} else {
				tmp._map.insert(pair<string, int>(it->first, it->second + it1->second));
			}
		}

		for (auto it = this->_map.begin(); it != this->_map.end(); it++)
		{
			auto it1 = tmp._map.find(it->first);
			if (it1 == tmp._map.end())
			{
				tmp._map.insert(pair<string, int>(it->first, it->second));
			}
		}

		tmp.cari_max_label();

		return tmp;
	}

	const Tlabel_stat operator - (const Tlabel_stat &t) const
	{
		Tlabel_stat tmp;

		tmp._jml_row =  this->_jml_row - t._jml_row;

		tmp._map.clear();


		for (auto it = this->_map.begin(); it != this->_map.end(); it++)
		{
			auto it1 = t._map.find(it->first);
			if (it1 == t._map.end())
			{
				tmp._map.insert(pair<string, int>(it->first, it->second));
			} else {
				if(it->second > it1->second){
				  tmp._map.insert(pair<string, int>(it->first, it->second - it1->second));
				}
			}
		}

		for (auto it = t._map.begin(); it != t._map.end(); it++)
		{
			auto it1 = tmp._map.find(it->first);
			if (it1 == tmp._map.end())
			{
				//cout<<"kacau !!! "<< it->first << " " << it->second <<endl;
				//tmp._map.insert(pair<string, int>(it->first, -1*it->second));
			}
		}

        tmp.cari_max_label();

		return tmp;
	}


	friend ostream & operator << (ostream & out, const Tlabel_stat & tc);

};

#endif