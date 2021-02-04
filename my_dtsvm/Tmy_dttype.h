#include <string>

using namespace std;


#ifndef Included_Tmy_dttype_H

#define Included_Tmy_dttype_H

class Tmy_dttype
{
private:
	string _value;
	string _lower_value;
	bool _is_continue;

	string to_lower(const string str);

public:
	Tmy_dttype();
	~Tmy_dttype();
	Tmy_dttype(string value, bool is_continue);

	string get_string();


	Tmy_dttype& operator = (const Tmy_dttype &t)
	{
		this->_value = t._value;
		this->_lower_value = t._lower_value;
		this->_is_continue = t._is_continue;

		return *this;
	}

	bool operator <(const Tmy_dttype& rhs) const
	{
		if (_is_continue)
		{
			return ((abs((stof(_value) - stof(rhs._value))) > 0.1) and (stof(_value) < stof(rhs._value)));
		} else {
			
			return (_lower_value < rhs._lower_value);
		}
	}

	bool operator <=(const Tmy_dttype& rhs) const
	{
		return (stof(_value) <= stof(rhs._value));
	}

	bool operator >(const Tmy_dttype& rhs) const
	{
		return (stof(_value) > stof(rhs._value));
	}

	bool operator ==(const Tmy_dttype& rhs) const
	{
		
		return (_lower_value == rhs._lower_value);
	}

	bool operator !=(const Tmy_dttype& rhs) const
	{
		
		return (_lower_value != rhs._lower_value);
	}

};

#endif