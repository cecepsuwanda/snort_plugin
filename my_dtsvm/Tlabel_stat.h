
#include <map>
#include <cmath>
#include <iterator>
#include <string>

using namespace std;

#ifndef Included_Tlabel_stat_H

#define Included_Tlabel_stat_H

class Tlabel_stat
{
private:
	int _jml_row;
	map<string, int> _map;
public:
	Tlabel_stat();
	~Tlabel_stat();
	void add(string value);
	void clear();

	Tlabel_stat& operator = (const Tlabel_stat &t)
	{
		this->_jml_row = t._jml_row;

		this->_map.clear();

		for (auto it = t._map.begin(); it != t._map.end(); it++)
		{
			this->_map.insert(pair<string, int>(it->first, it->second));
		}

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

		return tmp;
	}




};

#endif