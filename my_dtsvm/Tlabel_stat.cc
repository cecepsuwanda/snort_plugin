#include "Tlabel_stat.h"


Tlabel_stat::Tlabel_stat()
{
	_jml_row = 0;
}

Tlabel_stat::~Tlabel_stat()
{
	_map.clear();
}

void Tlabel_stat::clear()
{
	_map.clear();
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