#include "Tlabel_stat.h"
#include "global.h"
#include <string>

using namespace std;


#ifndef Included_Tbelow_above_H

#define Included_Tbelow_above_H

class Tbelow_above
{
 private: 
  Tmy_dttype _value;
  Tlabel_stat _below;
  Tlabel_stat _above;
 
  Tglobal_config global_config;

public:
	Tbelow_above();	
	~Tbelow_above();    

	void set_value(Tmy_dttype value);
	Tmy_dttype get_value();

	void add_below(Tlabel_stat stat);
	Tlabel_stat get_below();
	void add_above(Tlabel_stat stat);
	Tlabel_stat get_above();

	Tmy_dttype get_overall_metric();
	float get_split_info();

	void clear();

	bool cek_valid_cont();
	bool cek_valid_non_cont();
	
	
};

#endif