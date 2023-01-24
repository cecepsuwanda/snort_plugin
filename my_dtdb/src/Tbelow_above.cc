#include "Tbelow_above.h"


Tbelow_above::Tbelow_above()
{
	config = NULL;
}

Tbelow_above::Tbelow_above(Tconfig* v_config)
{
	config = v_config;
	_below.set_config(config);
	_above.set_config(config);
}


/*void Tbelow_above::set_config(Tconfig* v_config)
{
	config = v_config;
	_below.set_config(config);
	_above.set_config(config);
}*/

Tbelow_above::~Tbelow_above()
{
	_below.clear();
	_above.clear();
}

void Tbelow_above::clear()
{
	_below.clear();
	_above.clear();
}


bool Tbelow_above::cek_valid_cont()
{
	int jml = _below.get_jml_row() + _above.get_jml_row();
	bool pass = true;

	if (config->limited)
	{
		if (config->threshold >= 1) {
			pass = (_below.get_jml_row() >= config->threshold) and  (_above.get_jml_row() >= config->threshold);
		} else {
			pass = ((_below.get_jml_row() >= ceil(config->threshold * jml) ) and (_above.get_jml_row() >= ceil(config->threshold * jml) ));
		}


	}

	return pass;
}

bool Tbelow_above::cek_valid_non_cont()
{
	int jml = _below.get_jml_row() + _above.get_jml_row();
	bool pass = true;

	if (config->limited)
	{
		if (config->threshold >= 1) {
			pass = (_below.get_jml_row() >= config->threshold) and  (_above.get_jml_row() >= config->threshold);
		} else {
			pass = ((_below.get_jml_row() >= ceil(config->threshold * jml) ) and (_above.get_jml_row() >= ceil(config->threshold * jml) ));
		}
	}

	return pass;
}

void Tbelow_above::set_value(Tmy_dttype value)
{
	_value = value;
}

Tmy_dttype Tbelow_above::get_value()
{
	return _value;
}

void Tbelow_above::add_below(Tlabel_stat stat)
{
	_below = _below + stat;
}

Tlabel_stat Tbelow_above::get_below()
{
	return _below;
}


void Tbelow_above::add_above(Tlabel_stat stat)
{
	_above = _above + stat;
}

Tlabel_stat Tbelow_above::get_above()
{
	return _above;
}

float Tbelow_above::get_overall_metric()
{
	float overall_metric = 0.0;

	_below.set_config(config);
	_above.set_config(config);

	if (!config->use_credal) {
		int jml = _below.get_jml_row() + _above.get_jml_row();
		float p_dt_below = (float) _below.get_jml_row() / jml;
		float p_dt_above = (float) _above.get_jml_row() / jml;

		double entropy_below = _below.get_entropy();
		double entropy_above = _above.get_entropy();

		overall_metric = (p_dt_below * entropy_below) + (p_dt_above * entropy_above);
	} else {

		credal crd(config->credal_s);

		vector<int> freq;
		vector<double> ent, max_ent;

		freq.push_back(_below.get_jml_row());
		freq.push_back(_above.get_jml_row());

		crd.input_frec(freq);

		ent.push_back(_below.get_entropy());
		ent.push_back(_above.get_entropy());

		max_ent.push_back(_below.get_credal_entropy());
		max_ent.push_back(_above.get_credal_entropy());

		overall_metric = crd.get_overall_metric(ent, max_ent);
	}
	return overall_metric;
}

float Tbelow_above::get_split_info()
{
	int jml = _below.get_jml_row() + _above.get_jml_row();
	float p_dt_below = (float) _below.get_jml_row() / jml;
	float p_dt_above = (float) _above.get_jml_row() / jml;

	float split_info = 0;
	if (p_dt_below > 0) {
		split_info += (p_dt_below  * (-1 * log2(p_dt_below)));
	}
	if (p_dt_above > 0) {
		split_info += (p_dt_above  * (-1 * log2(p_dt_above)));
	}
	return split_info;
}


