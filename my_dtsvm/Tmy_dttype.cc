#include "Tmy_dttype.h"


Tmy_dttype::Tmy_dttype()
{
	_is_continue = false;
}

Tmy_dttype::~Tmy_dttype()
{

}

Tmy_dttype::Tmy_dttype(string value, bool is_continue)
{
	_value = value;
	_lower_value = value;
	_is_continue = is_continue;
	if(~_is_continue)
	{
      _lower_value = to_lower(_value);
	}
}

string Tmy_dttype::to_lower(const string str)
{
	string tmp_str;
	for (int i = 0; i < str.length(); ++i)
	{
		tmp_str[i] = toupper(str[i]);
	}
	return tmp_str;
}

string Tmy_dttype::get_string()
{
	return _value;
}