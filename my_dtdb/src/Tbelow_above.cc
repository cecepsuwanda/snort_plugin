#include "Tbelow_above.h"


Tbelow_above::Tbelow_above()
{

}

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


bool Tbelow_above::cek_valid()
{
	int jml = _below.get_jml_row() + _above.get_jml_row();
	bool pass = true;

	if (global_config.limited)
	{
		if (global_config.threshold >= 1) {
			pass = ((_below.get_jml_row() >= global_config.threshold) and (_above.get_jml_row() >= global_config.threshold));
		} else {
			//pass = ((_below.get_jml_row() >= ceil(global_config.threshold * jml) ) and (_above.get_jml_row() <= ceil((1-global_config.threshold) * jml) ));
			pass = ((_below.get_jml_row() >= ceil(global_config.threshold * jml) ) and (_below.get_jml_row() <= ceil((1 - global_config.threshold) * jml) ));
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

Tmy_dttype Tbelow_above::get_overall_metric()
{
	float overall_metric = 0.0;


	if (!global_config.use_credal) {
		int jml = _below.get_jml_row() + _above.get_jml_row();
		float p_dt_below = (float) _below.get_jml_row() / jml;
		float p_dt_above = (float) _above.get_jml_row() / jml;

		double entropy_below = stof(_below.get_entropy().get_value());
		double entropy_above = stof(_above.get_entropy().get_value());

		overall_metric = (p_dt_below * entropy_below) + (p_dt_above * entropy_above);

		// if (global_config.cetak_credal)
		// {
		// 	cout << "jml = " << jml << endl;
		// 	cout << "p_dt_below = " << p_dt_below << endl;
		// 	cout << "p_dt_above = " << p_dt_above << endl;
		// 	cout << "entropy_below = " << entropy_below << endl;
		// 	cout << "entropy_above = " << entropy_above << endl;
		// 	cout << "overall_metric = " << overall_metric << endl;
		// }

	} else {

		credal crd(global_config.credal_s);

		vector<int> freq;
		vector<double> ent, max_ent;

		freq.push_back(_below.get_jml_row());
		freq.push_back(_above.get_jml_row());

		crd.input_frec(freq);

		ent.push_back(stof(_below.get_entropy().get_value()));
		ent.push_back(stof(_above.get_entropy().get_value()));

		max_ent.push_back(stof(_below.get_credal_entropy().get_value()));
		max_ent.push_back(stof(_above.get_credal_entropy().get_value()));

		overall_metric = crd.get_overall_metric(ent, max_ent);
	}

	Tmy_dttype tmp;
	tmp.set_value(to_string(overall_metric), true);

	return tmp;
}

float Tbelow_above::get_split_info()
{
	int jml = _below.get_jml_row() + _above.get_jml_row();
	float p_dt_below = (float) _below.get_jml_row() / jml;
	float p_dt_above = (float) _above.get_jml_row() / jml;

	float split_info = 0.0;
	if (p_dt_below > 0) {
		split_info += (p_dt_below  * (-1 * log2(p_dt_below)));
	}
	if (p_dt_above > 0) {
		split_info += (p_dt_above  * (-1 * log2(p_dt_above)));
	}
	return split_info;
}

Tgain_ratio Tbelow_above::kalkulasi_gain_ratio(Tmy_dttype entropy_before_split)
{
	Tgain_ratio hsl;

	if (cek_valid())
	{
		Tmy_dttype entropy_after_split = get_overall_metric();
		hsl.split_info = get_split_info();
		hsl.gain = entropy_before_split - entropy_after_split;

		if (abs(hsl.split_info) > 0) {
			hsl.gain_ratio = hsl.gain / hsl.split_info;
		}
	}

	// if ((hsl.gain < 0.0) and (abs(stof(hsl.gain.get_string()))>0.01))
	// {
	// 	 cout << endl << " negatif gain !!! " << entropy_before_split.get_string() << " " << entropy_after_split.get_string() << endl;
	// }



	return hsl;
}
