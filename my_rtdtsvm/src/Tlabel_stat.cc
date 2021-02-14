#include "Tlabel_stat.h"


Tlabel_stat::Tlabel_stat()
{
	_jml_row = 0;
}

Tlabel_stat::~Tlabel_stat()
{
	_jml_row = 0;
	_map.clear();
}

void Tlabel_stat::clear()
{
	_jml_row = 0;
	_map.clear();
}

int Tlabel_stat::get_jml_row()
{
	return _jml_row;
}

int Tlabel_stat::get_jml_row_in_map()
{
	return _map.size();
}

string Tlabel_stat::get_first_value_in_map()
{
	auto itr = _map.begin();
    string tmp_string = itr->first;
    return tmp_string;
}


float Tlabel_stat::get_entropy()
{
	float entropy = 0;
	auto it = _map.begin();
	while (it != _map.end())
	{
		if( (it->second>0) and (_jml_row>0) ){
		  entropy += ((float) it->second / _jml_row)  * (-1 * log2((float) it->second / _jml_row));
		}

		it++;
	}
	return entropy;
}

void Tlabel_stat::add(string value)
{
	_jml_row += 1;
	map<string, int>::iterator it;

	it = _map.find(value);
	if (it == _map.end())
	{
		_map.insert(pair<string, int>(value, 1));
	} else {
		it->second += 1;
	}

}

map<string, int> Tlabel_stat::get_map()
{
	return _map;
}

ostream & operator << (ostream &out, const Tlabel_stat &tc)
{
	if (tc._map.size() > 0)
	{
		out << " Statistik   : " << endl;
		out << std::setw(30) << "label" << std::setw(10) << "Jumlah" << endl;
		for (auto it = tc._map.begin(); it != tc._map.end(); ++it) {
			out << std::setw(30) << (*it).first << std::setw(10) << (*it).second << endl;
		}
	}	
	return out;
}