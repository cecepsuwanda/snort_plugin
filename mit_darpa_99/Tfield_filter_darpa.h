
#include "Tdatetime_holder.h"
#include <vector>
#include <string>

using namespace std;


#ifndef Included_Tfield_filter_darpa_H

#define Included_Tfield_filter_darpa_H

class Tfield_filter_darpa
{
private:
	string _ID;
	Tdatetime_holder _Date;
	string _Name;
	string _Category;
	Tdatetime_holder _Start_Time;
	Tdatetime_holder _Duration;
	vector<string> _Attacker;
	vector<string> _Victim;
	string _Username;
	vector<string> _At_Attacker;
	vector<string> _At_Victim;

public:
	Tfield_filter_darpa();
	~Tfield_filter_darpa();

};


#endif