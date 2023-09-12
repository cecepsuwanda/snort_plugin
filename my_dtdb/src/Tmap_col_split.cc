#include "Tmap_col_split.h"


Tmap_col_split::Tmap_col_split()
{

}

Tmap_col_split::~Tmap_col_split()
{

}


void Tmap_col_split::add_data(int idx_col, map<Tmy_dttype, Tlabel_stat> data)
{
	_pot_split.insert(pair<int, map<Tmy_dttype, Tlabel_stat>>(idx_col, data));
}


void Tmap_col_split::add_data(int idx_col, string split_value, string tipe_data, string label)
{

	Tmy_dttype tmp_split_value(split_value, tipe_data == "continuous.");
	auto itr = _pot_split.find(idx_col);
	if (itr == _pot_split.end())
	{
		Tlabel_stat p;
		p.add(label);
		map<Tmy_dttype, Tlabel_stat> col_pot_split;
		col_pot_split.insert(pair<Tmy_dttype, Tlabel_stat>(tmp_split_value, p));
		_pot_split.insert(pair<int, map<Tmy_dttype, Tlabel_stat>>(idx_col, col_pot_split));

	} else {
		auto itr1 = itr->second.find(tmp_split_value);
		if (itr1 == itr->second.end())
		{
			Tlabel_stat p;
			p.add(label);
			itr->second.insert(pair<Tmy_dttype, Tlabel_stat>(tmp_split_value, p));
		} else {
			itr1->second.add(label);
		}
	}

}

void Tmap_col_split::cek_valid_attr(int jml_data_cabang,int jml_data_root)
{
	_valid_attr.clear();

	Tquery_builder global_query_builder;

	global_query_builder.open_connection();

	//(is_continue=0) or

	string tmp = "select * from attr_stat where is_continue=1 order by id"; // where (is_continue=0) or ((is_continue=1) and  (jml <= round("+to_string(global_config.ratio_valid_attr)+"*" + to_string(jml_row) + ",2)) and  (jml>=2))

	if (!global_config.continue_attr_only)
	{   
		tmp = "select * from attr_stat where ((is_continue=1) and (jml>=2)) or ((is_continue=0) and (id not in (1)) and (power(2,jml-1) <= round(" + to_string(global_config.ratio_valid_attr) + "*" + to_string(jml_data_cabang) + ",2)) and (jml>=2)) order by id";		
	}

	if (global_query_builder.query(tmp))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();					
					_valid_attr.push_back(stoi(data[0]));
					jml_row--;
				}

			}
		}
	}

	


	global_query_builder.close_connection();



}

int Tmap_col_split::get_jml_valid_attr()
{
	return _valid_attr.size();
}

int Tmap_col_split::get_valid_attr(int idx)
{
	return _valid_attr[idx];
}

bool Tmap_col_split::is_valid_attr(int idx)
{
	bool is_valid = false;
	if (_valid_attr.size() > 0)
	{
		auto itr = _valid_attr.begin();
		while ((!is_valid) and (itr != _valid_attr.end()))
		{
			is_valid = (*itr == idx);
			itr++;
		}
	}
	return is_valid;
}

void Tmap_col_split::clear()
{
	for (auto i = _pot_split.begin(); i != _pot_split.end(); ++i)
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			j->second.clear();
		}
		i->second.clear();
	}
	_pot_split.clear();
}

map<Tmy_dttype, Tlabel_stat> Tmap_col_split::get_pot_split(int idx)
{
	map<Tmy_dttype, Tlabel_stat> col_split;
	Tquery_builder global_query_builder;

	global_query_builder.open_connection();

	string tmp = "select * from attr" + to_string(idx) + " order by id";

	if (global_query_builder.query(tmp))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();
			vector<string> tmp_type = global_query_builder.get_column_type();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();
					Tmy_dttype tmp_split_value(data[1], tmp_type[1] == "continuous.");

					auto itr = col_split.find(tmp_split_value);
					if (itr == col_split.end())
					{
						Tlabel_stat label_stat;
						label_stat.add(data[2], stoi(data[3]));

						col_split.insert(pair<Tmy_dttype, Tlabel_stat>(tmp_split_value, label_stat));
					} else {

						itr->second.add(data[2], stoi(data[3]));
					}
					jml_row--;
				}

			}
		}
	}


	global_query_builder.close_connection();

	//return &_pot_split[idx];
	return col_split;
}