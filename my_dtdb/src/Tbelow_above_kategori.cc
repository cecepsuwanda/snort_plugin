#include "Tbelow_above_kategori.h"


Tbelow_above_kategori::Tbelow_above_kategori()
{
	_jml_row = 0;
}

Tbelow_above_kategori::~Tbelow_above_kategori()
{
	clear();
}

void Tbelow_above_kategori::clear()
{
	_stat.clear();
}

bool Tbelow_above_kategori::cek_valid()
{
	bool pass = true;

	if (global_config.limited)
	{
		// if (global_config.threshold >= 1) {

		// 	pass = (_stat[0].get_jml_row() >= global_config.threshold) and (_stat[0].get_jml_row()<= (_jml_row-global_config.threshold));

		// } else {

		// 	pass = ((_stat[0].get_jml_row() >= ceil(global_config.threshold * _jml_row) ) and (_stat[0].get_jml_row() <= ceil((1 - global_config.threshold) * _jml_row) ));

		// }
	}

	return pass;
}

void Tbelow_above_kategori::set_value(Tmy_dttype value)
{
	_value = value;
}

Tmy_dttype Tbelow_above_kategori::get_value()
{
	return _value;
}

void Tbelow_above_kategori::add_stat(Tlabel_stat stat)
{
	_stat.push_back(stat);
	_jml_row = _jml_row + stat.get_jml_row();
}

vector<Tlabel_stat> Tbelow_above_kategori::get_stat()
{
	return _stat;
}

Tmy_dttype Tbelow_above_kategori::get_overall_metric()
{
	float overall_metric = 0.0;

	if (!global_config.use_credal) {

		for (auto itr = _stat.begin(); itr != _stat.end(); ++itr)
		{
			double entropy = stof((*itr).get_entropy().get_value());
			overall_metric = overall_metric + (((float)  (*itr).get_jml_row() / _jml_row) * entropy);
		}


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


		for (auto itr = _stat.begin(); itr != _stat.end(); ++itr)
		{
			freq.push_back((*itr).get_jml_row());
		}

		crd.input_frec(freq);

		for (auto itr = _stat.begin(); itr != _stat.end(); ++itr)
		{
			ent.push_back(stof((*itr).get_entropy().get_value()));
		}

		for (auto itr = _stat.begin(); itr != _stat.end(); ++itr)
		{
			max_ent.push_back(stof((*itr).get_credal_entropy().get_value()));
		}

		overall_metric = crd.get_overall_metric(ent, max_ent);
	}

	Tmy_dttype tmp;
	tmp.set_value(to_string(overall_metric), true);

	return tmp;
}

float Tbelow_above_kategori::get_split_info()
{

	float split_info = 0.0;
	for (auto itr = _stat.begin(); itr != _stat.end(); ++itr)
	{
		float p = (float)  (*itr).get_jml_row() / _jml_row;
		split_info += (p  * (-1 * log2(p)));
	}

	return split_info;
}

Tgain_ratio_kategori Tbelow_above_kategori::kalkulasi_gain_ratio(Tmy_dttype entropy_before_split)
{
	Tgain_ratio_kategori hsl;

	Tmy_dttype entropy_after_split = get_overall_metric();
	float split_info = get_split_info();
	hsl.gain = entropy_before_split - entropy_after_split;
	if (abs(split_info) > 0) {
		hsl.gain_ratio = hsl.gain / split_info;
	}

	return hsl;
}
