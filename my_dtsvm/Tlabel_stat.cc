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

float Tlabel_stat::get_entropy()
{
	float entropy = 0;
	auto it = _map.begin();
	while (it != _map.end())
	{
		entropy += ((float) it->second / _jml_row)  * (-1 * log2((float) it->second / _jml_row));
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