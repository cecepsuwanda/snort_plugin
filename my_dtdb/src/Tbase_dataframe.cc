#include "Tbase_dataframe.h"


Tbase_dataframe::Tbase_dataframe()
{

}

Tbase_dataframe::~Tbase_dataframe()
{
	_data_header.clear();
	_data_header.shrink_to_fit();
	_data_type.clear();
	_data_type.shrink_to_fit();
	_filter.clear();
	_filter.shrink_to_fit();
}


void Tbase_dataframe::switch_to_test()
{
	_data.switch_to_test();
}

void Tbase_dataframe::read_header_type()
{
	_data.read_header_type();
	_data_header = _data.get_data_header();
	_data_type = _data.get_data_type();
	_jml_col = _data.get_idx_label() + 1;
}


void Tbase_dataframe::set_dataset(int id_dt, int jns_dt, string partition)
{
	_id_dt = id_dt;
	_jns_dt = jns_dt;
	_partition = partition;
	_data.set_dataset(id_dt, jns_dt, _partition);
}

void Tbase_dataframe::set_branch(int depth, int branch, int branch_number)
{
	_child_depth = depth;
	_child_branch = branch;
	_child_branch_number = branch_number;
	_data.delete_child(depth, branch, branch_number);
	_data.set_child(_child_depth, _child_branch, _child_branch_number);

}

void Tbase_dataframe::set_parent(int depth, int branch, int branch_number)
{
	_parent_depth = depth;
	_parent_branch = branch;
	_parent_branch = branch_number;
	_data.set_child(_parent_depth, _parent_branch, _parent_branch_number);
}

void Tbase_dataframe::switch_parent_branch()
{
	_parent_depth = _child_depth;
	_parent_branch = _child_branch;
	_parent_branch_number = _child_branch_number;
	_data.switch_parent_child();
}


string Tbase_dataframe::filter_to_query(bool is_last)
{
	string tmp = "";

	if (_filter.size() > 0)
	{

		if (is_last)
		{
			string tmp1 = "(";

			switch (_filter[_filter.size() - 1].idx_opt)
			{
			case 0 :
				tmp1 = tmp1 + " round(dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + ",7)<=" + _filter[_filter.size() - 1].value + ")" ;
				break;
			case 1 :
				tmp1 = tmp1 + " round(dataset." + _data_header[_filter[_filter.size() - 1].idx_col] +  ",7)>" + _filter[_filter.size() - 1].value + ")";
				break;
			case 2 :
				tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + "='" + _filter[_filter.size() - 1].value + "')";
				break;
			case 3 :
				tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + "!='" + _filter[_filter.size() - 1].value + "')";;
				break;
			}

			tmp = tmp1;
		} else {
			size_t i = 0;
			while ((i < _filter.size()))
			{
				string tmp1 = "(";
				if (tmp != "")
				{
					tmp1 = " and (";
				}

				switch (_filter[i].idx_opt)
				{
				case 0 :
					tmp1 = tmp1 + " round(dataset." + _data_header[_filter[i].idx_col] + ",7)<=" + _filter[i].value + ")" ;
					break;
				case 1 :
					tmp1 = tmp1 + " round(dataset." + _data_header[_filter[i].idx_col]  + ",7)>" + _filter[i].value + ")";
					break;
				case 2 :
					tmp1 = tmp1 + " dataset." + _data_header[_filter[i].idx_col] + "='" + _filter[i].value  + "')";
					break;
				case 3 :
					tmp1 = tmp1 + " dataset." + _data_header[_filter[i].idx_col] + "!='" + _filter[i].value  + "')";
					break;
				}

				tmp = tmp + tmp1;
				i++;
			}

		}
	}
	return tmp;
}


// bool Tbase_dataframe::is_pass(vector<string> &data)
// {

// 	bool pass = true;
// 	if (_filter.size() > 0)
// 	{
// 		size_t i = 0;
// 		while ((i < _filter.size()) and pass)
// 		{
// 			Tmy_dttype tmp1(_filter[i].value, _data_type[_filter[i].idx_col] == "continuous.");
// 			Tmy_dttype tmp2(data[_filter[i].idx_col], _data_type[_filter[i].idx_col] == "continuous.");

// 			switch (_filter[i].idx_opt)
// 			{
// 			case 0 :
// 				pass = tmp2 <= tmp1 ;
// 				break;
// 			case 1 :
// 				pass = tmp1  < tmp2;
// 				break;
// 			case 2 :
// 				pass = tmp1 == tmp2;
// 				break;
// 			case 3 :
// 				pass = tmp1 != tmp2;
// 				break;
// 			}
// 			i++;
// 		}
// 	}

// 	return pass;
// }

// bool Tbase_dataframe::is_pass()
// {

// 	bool pass = true;
// 	if (_filter.size() > 0)
// 	{
// 		size_t i = 0;
// 		while ((i < _filter.size()) and pass)
// 		{
// 			Tmy_dttype tmp1(_filter[i].value, _data_type[_filter[i].idx_col] == "continuous.");
// 			Tmy_dttype tmp2(_data.get_col_val(_filter[i].idx_col), _data_type[_filter[i].idx_col] == "continuous.");

// 			switch (_filter[i].idx_opt)
// 			{
// 			case 0 :
// 				pass = tmp2 <= tmp1 ;
// 				break;
// 			case 1 :
// 				pass = tmp1  < tmp2;
// 				break;
// 			case 2 :
// 				pass = tmp1 == tmp2;
// 				break;
// 			case 3 :
// 				pass = tmp1 != tmp2;
// 				break;
// 			}
// 			i++;
// 		}
// 	}

// 	return pass;
// }



int Tbase_dataframe::getjmlcol()
{
	return _jml_col;
}

int Tbase_dataframe::getjmlrow()
{
	return _jml_row;
}

void Tbase_dataframe::setjmltotalrow()
{
	_jml_total_row = _jml_row;
}

int Tbase_dataframe::getjmltotalrow()
{
	return _jml_total_row;
}

void Tbase_dataframe::reset_file()
{
	_data.reset_file();
}



void Tbase_dataframe::close_file()
{
	_data.close_file();
}

bool Tbase_dataframe::is_eof()
{
	return _data.is_eof();
}

void Tbase_dataframe::next_record()
{
	_data.next_record();
}

vector<string> Tbase_dataframe::get_record()
{
	return _data.get_record();
}

void Tbase_dataframe::next_col()
{
	_data.next_col();
}

bool Tbase_dataframe::is_end_col()
{
	return _data.is_end_col();
}

string Tbase_dataframe::get_col_val()
{
	return _data.get_col_val();
}

string Tbase_dataframe::get_col_val(int idx_col)
{
	return _data.get_col_val(idx_col);
}

int Tbase_dataframe::get_idx_col()
{
	return _data.get_idx_col();
}


vector<vector<string>> Tbase_dataframe::get_all_record()
{
	//std::lock_guard<std::mutex> lock(v_mutex);
	//ReFilter();

	// string tmp_sql = filter_to_query(false);
	// _data.filter(tmp_sql,false);

	_data.read_hsl_filter();

	vector<vector<string>> Table;
	_data.reset_file();
	while (!_data.is_eof())
	{
		Table.push_back(_data.get_record());
		_data.next_record();
	}

	//_data.close_file();

	//clear_memory();

	return Table;
}


void Tbase_dataframe::add_filter(int idx_col, int idx_opt, string value)
{
	field_filter f;
	f.idx_col = idx_col;
	f.idx_opt = idx_opt;
	f.value = value;
	_filter.push_back(f);

	string sql = filter_to_query(false);
	_data.filter(sql, false);

}

void Tbase_dataframe::add_filter(field_filter filter)
{

	_filter.push_back(filter);

	string sql = filter_to_query(false);
	_data.filter(sql, false);
}

void Tbase_dataframe::ReFilter()
{
	string sql = filter_to_query(false);
	if (sql != "") {
		_data.filter(sql, false);
	}
}

vector<field_filter> Tbase_dataframe::get_filter()
{
	return _filter;
}

void Tbase_dataframe::clear_memory(int idx)
{
	if (idx == 0) {
		_data.clear_child_parent();
	} else {
		_data.set_child_parent();
	}

}

void Tbase_dataframe::info()
{
	cout << " Info" << endl;
	//cout << " Nama File   : " << _nm_file << endl;
	cout << " Jumlah Data : " << _jml_row << endl;
}

void Tbase_dataframe::save_to(string nm_file)
{
	// Tread_file tmp;
	// tmp.setnm_f(nm_file, ",");
	// tmp.open_file("w+");

	// vector<string> tmp_data;
	// _data.reset_file();
	// while (!_data.is_eof())
	// {
	// 	tmp_data = _data.get_record();

	// 	string tmp_str = "";
	// 	for (size_t i = 0; i < (tmp_data.size() - 1); ++i)
	// 	{
	// 		tmp_str = tmp_str + tmp_data[i] + ",";
	// 	}
	// 	tmp_str = tmp_str + tmp_data[tmp_data.size() - 1];

	// 	tmp.write_file(tmp_str);


	// 	tmp_data.clear();
	// 	tmp_data.shrink_to_fit();
	// 	_data.next_record();
	// }
}

string Tbase_dataframe::get_data_type(int idx)
{
	return _data_type[idx];
}

void Tbase_dataframe::head()
{
	vector<string> tmp_data;
	map<string, int>::iterator it;

	cout << " Head" << endl;
	//cout << " Nama File   : " << _nm_file << endl;

	if (_jml_row > 0) {
		int i = 0;

		_data.reset_file();
		while ((!_data.is_eof()) and (i < 5))
		{
			tmp_data = _data.get_record();
			for (size_t idx = 0; idx < tmp_data.size(); ++idx)
			{
				cout << setw(tmp_data[idx].length() + 2) << tmp_data[idx];
			}
			cout << endl;
			i++;

			tmp_data.clear();
			tmp_data.shrink_to_fit();
			_data.next_record();
		}

	}
}

void Tbase_dataframe::train_to_test()
{
	_data.train_to_test();
}

posisi_cabang Tbase_dataframe::get_posisi()
{
	posisi_cabang tmp;

	tmp.parent_depth  = _parent_depth;
	tmp.parent_branch = _parent_branch;
	tmp.parent_branch_number = _parent_branch_number;

	tmp.child_depth  = _child_depth;
	tmp.child_branch = _child_branch;
	tmp.child_branch_number = _child_branch_number;


	return tmp;
}



