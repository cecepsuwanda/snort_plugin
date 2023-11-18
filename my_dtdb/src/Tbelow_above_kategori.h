#include "Tlabel_stat.h"
#include "global.h"
#include <string>

using namespace std;


#ifndef Included_Tbelow_above_kategori_H

#define Included_Tbelow_above_kategori_H

struct Tgain_ratio_kategori
{
	Tmy_dttype gain;
	Tmy_dttype gain_ratio;
	float split_info = 0.0;

	Tgain_ratio_kategori()
	{
        gain.set_value("0.0",true);
        gain_ratio.set_value("0.0",true);
		split_info = 0.0;
	}
};

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

	Tgain_ratio_kategori kalkulasi_gain_ratio(Tmy_dttype entropy_before_split);
};

#endif