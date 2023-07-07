#include "Tlabel_stat.h"
#include "global.h"
#include <string>

using namespace std;


#ifndef Included_Tbelow_above_kategori_H

#define Included_Tbelow_above_kategori_H

class Tbelow_above_kategori
{
 private: 
  Tmy_dttype _value;  
  
  vector<Tlabel_stat> _stat;
  int _jml_row;   
 
  Tglobal_config global_config;

public:
	Tbelow_above_kategori();	
	~Tbelow_above_kategori();    

	void set_value(Tmy_dttype value);
	Tmy_dttype get_value();
	
	void add_stat(Tlabel_stat stat);
	vector<Tlabel_stat> get_stat();

	Tmy_dttype get_overall_metric();
	float get_split_info();

	void clear();
	
	bool cek_valid();	
	
};

#endif