#include "tb_dataset.h"


tb_dataset::tb_dataset()
{
	_tmp_dataset_tb = "tmp_dataset_train";
	_tmp_attr_dataset_tb = "attr_dataset_train";


}

tb_dataset::~tb_dataset()
{
	_data.clear();
	_data.shrink_to_fit();
	_data_header.clear();
	_data_header.shrink_to_fit();
	_data_type.clear();
	_data_type.shrink_to_fit();
}

void tb_dataset::switch_to_test()
{
	_tmp_dataset_tb = "tmp_dataset_test";
	_tmp_attr_dataset_tb = "attr_dataset_test";
}


void tb_dataset::read_header_type()
{
	string query = "select * from dataset limit 1";

	global_query_builder.open_connection();

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			_jml_col = global_query_builder.get_jml_col();

			_data_header.clear();
			_data_type.clear();

			vector<string> tmp_header = global_query_builder.get_column_name();
			vector<string> tmp_type = global_query_builder.get_column_type();
			for (int i = 2; i < _jml_col; i++)
			{
				_data_header.push_back(tmp_header[i]);
				_data_type.push_back(tmp_type[i]);

				_idx_label = i - 2;
			}

		}
	}


}


vector<string> tb_dataset::get_data_header()
{
	return _data_header;
}

vector<string> tb_dataset::get_data_type()
{
	return _data_type;
}

int tb_dataset::get_idx_label()
{
	return _idx_label;
}

int tb_dataset::get_jml_row()
{
	return _jml_row;
}

int tb_dataset::get_jml_col()
{
	return _jml_col;
}

void tb_dataset::close_file()
{
	global_query_builder.close_connection();
}

void tb_dataset::reset_file()
{
	global_query_builder.go_to_no_record(0);
	read_file();
}


bool tb_dataset::is_eof()
{
	return _data.size() == 0;
}

vector<string> tb_dataset::get_record()
{
	return _data;
}

void tb_dataset::next_col()
{
	_idx_col = _idx_col + 1;
}

bool tb_dataset::is_end_col()
{
	return  !(_idx_col < _data.size());
}

string tb_dataset::get_col_val()
{

	return _data[_idx_col];
}

int tb_dataset::get_idx_col()
{

	return _idx_col;
}

string tb_dataset::get_col_val(int idx_col)
{
	return _data[idx_col];
}

void tb_dataset::next_record()
{
	read_file();
}

void tb_dataset::read_file()
{
	_data.clear();
	_data.shrink_to_fit();
	_idx_col = 0;

	vector<string> data = global_query_builder.fetch_row();

	if (data.size() > 0) {

		for (int i = 3; i < _jml_col; i++)
		{
			_data.push_back(data[i]);
		}

	}

}


void tb_dataset::set_dataset(int id_dt, int jns_dt, string partition)
{
	_id_dt = id_dt;
	_jns_dt = jns_dt;
	_partition = partition;
}

void tb_dataset::set_parent(int depth, int branch, int branch_number)
{
	_posisi_cabang.set_parent(depth, branch, branch_number);
}

void tb_dataset::set_child(int depth, int branch, int branch_number)
{
	_posisi_cabang.set_child(depth, branch, branch_number);
}

void tb_dataset::switch_parent_child()
{
	_posisi_cabang.switch_parent_branch();
}

void tb_dataset::set_label(string label)
{
	string where_str  = _posisi_cabang.to_query(0);

	string query = "update " + _tmp_dataset_tb + " set label='" + label + "' where " + where_str;
	global_query_builder.query(query);
}

bool tb_dataset::is_child_parent_exist()
{
	bool hsl = false;

	string where_str  = _posisi_cabang.to_query(0);

	string query = "select * from " + _tmp_dataset_tb + " where " + where_str + " limit 1 ";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			int tmp = global_query_builder.get_jml_row();
			hsl = tmp > 0;
		}
	}

	return hsl;
}

bool tb_dataset::is_parent_exist(int idx)
{
	bool hsl = false;

	string where_str  = _posisi_cabang.to_query(3);

	if (idx == 1) {
		where_str  = _posisi_cabang.to_query(4);
	}

	string query = "select * from " + _tmp_dataset_tb + " where " + where_str + " limit 1 ";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			int tmp = global_query_builder.get_jml_row();
			hsl = tmp > 0;
		}
	}

	return hsl;
}

bool tb_dataset::exist_in_detil(int idx)
{
	bool hsl = false;

	string where_str  = _posisi_cabang.to_query(0);

	if (idx == 1)
	{
		where_str  = _posisi_cabang.to_query(3);
	}

	string query = "select count(*) as jml from detail_missing_branch where " + where_str;

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			hsl = stoi(tmp[0]) > 0;
		}
	}

	return hsl;
}

Tposisi_cabang tb_dataset::get_posisi_parent()
{
	Tposisi_cabang tmp_posisi;

	string where_str  = _posisi_cabang.to_query(3);

	string query = "select child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number from " + _tmp_dataset_tb + " where " + where_str + " group by child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number ";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{

			vector<string> data = global_query_builder.fetch_row();
			tmp_posisi.child_depth = stoi(data[0]);
			tmp_posisi.child_branch = stoi(data[1]);
			tmp_posisi.child_branch_number = stoi(data[2]);
			tmp_posisi.parent_depth = stoi(data[3]);
			tmp_posisi.parent_branch = stoi(data[4]);
			tmp_posisi.parent_branch_number = stoi(data[5]);

		}
	}


	return tmp_posisi;
}


void tb_dataset::filter(string sql, bool is_all)
{
	if (sql != "")
	{

		//bool is_pass1 = true;

		if (!is_child_parent_exist())
		{
			clear_tb_index();
			clear_tb_index1();
			string query = "";

			if (is_parent_exist(0)) {
				// string where_str  = "";
				// where_str  = "child_depth=" + to_string(_parent_depth) + " and ";
				// where_str += "child_branch=" + to_string(_parent_branch) + " and ";
				// where_str += "child_branch_number=" + to_string(_parent_branch_number);
				// query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where " + where_str + " order by id_row)";
				//global_query_builder.query(query);

				// query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") inner join tb_index on dataset.id = idx_row  where (" + sql + "))";
				// global_query_builder.query(query);

				string set_str = "";
				set_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_posisi_cabang.child_depth) + ",";
				set_str += _tmp_dataset_tb + ".child_branch=" + to_string(_posisi_cabang.child_branch) + ",";
				set_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_posisi_cabang.child_branch_number) + ",";
				set_str += _tmp_dataset_tb + ".parent_depth=" + to_string(_posisi_cabang.parent_depth) + ",";
				set_str += _tmp_dataset_tb + ".parent_branch=" + to_string(_posisi_cabang.parent_branch) + ",";
				set_str += _tmp_dataset_tb + ".parent_branch_number=" + to_string(_posisi_cabang.parent_branch_number);

				// string where_str  = "";
				// where_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_parent_depth) + " and ";
				// where_str += _tmp_dataset_tb + ".child_branch=" + to_string(_parent_branch) + " and ";
				// where_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_parent_branch_number);
				// query = "update (" + _tmp_dataset_tb + " inner join tb_index1 on id_row=idx_row) set " + set_str + " where (" + where_str + ")";
				// global_query_builder.query(query);

				query = "call sp_filter(" + to_string(_posisi_cabang.child_depth) + "," + to_string(_posisi_cabang.child_branch) + "," + to_string(_posisi_cabang.child_branch_number) + "," + to_string(_posisi_cabang.parent_depth) + "," + to_string(_posisi_cabang.parent_branch) + "," + to_string(_posisi_cabang.parent_branch_number) + "," + '"' + sql + '"' + ",'" + _tmp_dataset_tb + "','" + _tmp_attr_dataset_tb + "')";
				global_query_builder.query(query);

			} else {

				if (is_all) {

					if ( (_tmp_dataset_tb == "tmp_dataset_train") and exist_in_detil(0)) {
						// string where_str  = "";
						// where_str  = "child_depth=" + to_string(_child_depth) + " and ";
						// where_str += "child_branch=" + to_string(_child_branch) + " and ";
						// where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
						// where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
						// where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
						// where_str += "parent_branch_number=" + to_string(_parent_branch_number);

						// query = "insert into tb_index1(idx_row) (select id_row from detail_missing_branch where " + where_str + ")";
						// global_query_builder.query(query);

						query = "call sp_filter1(" + to_string(_posisi_cabang.child_depth) + "," + to_string(_posisi_cabang.child_branch) + "," + to_string(_posisi_cabang.child_branch_number) + "," + to_string(_posisi_cabang.parent_depth) + "," + to_string(_posisi_cabang.parent_branch) + "," + to_string(_posisi_cabang.parent_branch_number) + ",'" + _tmp_dataset_tb + "')";
						global_query_builder.query(query);

					} else {
						// if ((_tmp_dataset_tb == "tmp_dataset_train"))
						// {
						// 	cetak(" kacau 1 !!! ");
						// }

						// query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") where (" + sql + "))";
						// global_query_builder.query(query);

						query = "call sp_filter2(" + to_string(_posisi_cabang.child_depth) + "," + to_string(_posisi_cabang.child_branch) + "," + to_string(_posisi_cabang.child_branch_number) + "," + to_string(_posisi_cabang.parent_depth) + "," + to_string(_posisi_cabang.parent_branch) + "," + to_string(_posisi_cabang.parent_branch_number) + "," + '"' + sql + '"' + ",'" + _tmp_dataset_tb + "','" + _tmp_attr_dataset_tb + "')";
						global_query_builder.query(query);
					}

					// string set_str = "";
					// set_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_child_depth) + ",";
					// set_str += _tmp_dataset_tb + ".child_branch=" + to_string(_child_branch) + ",";
					// set_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_child_branch_number) + ",";
					// set_str += _tmp_dataset_tb + ".parent_depth=" + to_string(_parent_depth) + ",";
					// set_str += _tmp_dataset_tb + ".parent_branch=" + to_string(_parent_branch) + ",";
					// set_str += _tmp_dataset_tb + ".parent_branch_number=" + to_string(_parent_branch_number);

					// query = "update (" + _tmp_dataset_tb + " inner join tb_index1 on id_row=idx_row) set " + set_str;
					// global_query_builder.query(query);


				} else {
					// if ((_tmp_dataset_tb == "tmp_dataset_train"))
					// {ak(" k
					// 	cetacau 2 !!! ");
					// }
				}

			}

			//if (is_parent_exist(0)) {
			// query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") inner join tb_index on dataset.id = idx_row  where (" + sql + "))";
			// global_query_builder.query(query);
			//} else {
			//if (is_all) {
			//query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") where (" + sql + "))";
			//global_query_builder.query(query);
			//} else {
			//cetak(" kacau !!! ");
			//}
			//}

			// string set_str = "";
			// set_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_child_depth) + ",";
			// set_str += _tmp_dataset_tb + ".child_branch=" + to_string(_child_branch) + ",";
			// set_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_child_branch_number) + ",";
			// set_str += _tmp_dataset_tb + ".parent_depth=" + to_string(_parent_depth) + ",";
			// set_str += _tmp_dataset_tb + ".parent_branch=" + to_string(_parent_branch) + ",";
			// set_str += _tmp_dataset_tb + ".parent_branch_number=" + to_string(_parent_branch_number);

			// if (is_parent_exist(0)) {
			// string where_str  = "";
			// where_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_parent_depth) + " and ";
			// where_str += _tmp_dataset_tb + ".child_branch=" + to_string(_parent_branch) + " and ";
			// where_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_parent_branch_number);
			// query = "update (" + _tmp_dataset_tb + " inner join tb_index1 on id_row=idx_row) set " + set_str + " where (" + where_str + ")";
			// global_query_builder.query(query);
			// } else {
			// 	if (is_all) {
			// 		query = "update (" + _tmp_dataset_tb + " inner join tb_index1 on id_row=idx_row) set " + set_str;
			// 		global_query_builder.query(query);
			// 	} else {
			// 		cetak(" kacau !!! ");
			// 	}
			// }

			clear_tb_index1();
		}

		clear_tb_index();

		// string where_str  = "";
		// where_str  = "child_depth=" + to_string(_child_depth) + " and ";
		// where_str += "child_branch=" + to_string(_child_branch) + " and ";
		// where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
		// where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
		// where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
		// where_str += "parent_branch_number=" + to_string(_parent_branch_number);

		// string query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where " + where_str + " order by id_row)";
		// global_query_builder.query(query);

		string query = "call sp_filter3(" + to_string(_posisi_cabang.child_depth) + "," + to_string(_posisi_cabang.child_branch) + "," + to_string(_posisi_cabang.child_branch_number) + "," + to_string(_posisi_cabang.parent_depth) + "," + to_string(_posisi_cabang.parent_branch) + "," + to_string(_posisi_cabang.parent_branch_number) + ",'" + _tmp_dataset_tb + "')";
		global_query_builder.query(query);

	}

	string query = "select count(*) as jml from tb_index";

	_jml_row = 0;

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			_jml_row = stoi(tmp[0]);
		}
	}

}


void tb_dataset::read_hsl_filter()
{

	string query = "select * from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row order by a.id";

	_jml_row = 0;

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			_jml_row = global_query_builder.get_jml_row();
		}
	}

}

Tlabel_stat tb_dataset::hit_label_stat()
{
	Tlabel_stat label_stat;

	string query = "select label,count(label) as jml from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row group by label order by label";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();
					label_stat.add(data[0], stoi(data[1]));
					jml_row--;
				}
			}
		}
	}

	return label_stat;

}

map<string, map<string, int>> tb_dataset::hit_conf_metrik()
{
	map<string, map<string, int>> tmp_data;

	string where_str  = _posisi_cabang.to_query(0);

	string query = "select a.label as label_org,b.label as label_pred,count(*) as jml from " + _tmp_attr_dataset_tb + " a inner join tmp_dataset_test b on a.id=b.id_row where " + where_str + " group by a.label,b.label order by a.label,b.label";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();

					//label_stat.add(data[0], stoi(data[1]));

					auto it = tmp_data.find(data[0]);
					if (it == tmp_data.end())
					{
						map<string, int> tmp_map;
						tmp_map.insert(pair<string, int>(data[1], stoi(data[2])));
						tmp_data.insert(pair<string, map<string, int>>(data[0], tmp_map));
					} else {
						auto it1 = it->second.find(data[1]);
						if (it1 == it->second.end())
						{
							it->second.insert(pair<string, int>(data[1], stoi(data[2])));
						} else {
							it1->second += stoi(data[2]);
						}
					}

					jml_row--;
				}
			}
		}
	}

	return tmp_data;

}

void tb_dataset::gen_permutation_rec(vector<string> lst_label, int count, int depth, int geser, string tb, string group_kolom, string str_select , string query)
{
	if (count == depth)
	{
		for (size_t i = geser; i < lst_label.size(); ++i)
		{
			string tmp_query = query + ",'" + lst_label[i] + "'";
			string tmp_select = str_select + ";" + lst_label[i];
			string tmp = "insert into " + tb + "(" + group_kolom + ",label,jml) (select '" + tmp_select + "',label,count(*) as jml from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " in (" + tmp_query + ") group by label)";
			global_query_builder.query(tmp);
		}
	} else {
		count++;
		for (size_t i = geser; i < lst_label.size(); ++i)
		{
			string tmp_query = query + ",'" + lst_label[i] + "'";
			string tmp_select = str_select + ";" + lst_label[i];
			gen_permutation_rec(lst_label, count, depth, i + 1, tb, group_kolom, tmp_select, tmp_query);
		}
	}
}

void tb_dataset::gen_permutation(vector<string> lst_label, int depth, string tb, string group_kolom)
{
	for (size_t i = 0; i < lst_label.size(); ++i)
	{
		string tmp_query = "'" + lst_label[i] + "'";
		string str_select = lst_label[i];
		gen_permutation_rec(lst_label, 1, depth, i + 1, tb, group_kolom, str_select, tmp_query);
	}
}

map<Tmy_dttype, Tlabel_stat> tb_dataset::hit_col_split(string group_kolom)
{
	map<Tmy_dttype, Tlabel_stat> col_split;

	size_t i = 0;
	bool ketemu = false;
	while ((i < _data_header.size()) and (not ketemu))
	{
		if (_data_header[i] == group_kolom)
		{
			ketemu = true;
		} else {
			i++;
		}
	}

	if (ketemu)
	{
		string tmp = "truncate attr" + to_string(i);
		global_query_builder.query(tmp);

		if (_data_type[i] == "continuous.")
		{

			tmp = "call sp_hit_stat('attr" + to_string(i) + "','" + _tmp_attr_dataset_tb + "','" + group_kolom + "')";
			global_query_builder.query(tmp);

		} else {

			//bool pass = true;
            
            // if(i==6)
            // {
            //    pass = true;
            // }
			
			// if (i == 1)
			// {
			// 	pass = false;
			// 	// string query = "select "+group_kolom+" from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " not in ('icmp','tcp','udp') group by "+group_kolom;
			// 	// global_query_builder.query(query);
			// 	// global_query_builder.get_result();
			// 	// pass = global_query_builder.get_jml_row() <= 2;

			// 	// if (pass1)
			// 	// {
			// 	// 	string tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) (select 'icmp;tcp;udp',label,count(*) as jml from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " in ('icmp','tcp','udp') group by label)";
			// 	// 	global_query_builder.query(tmp);
			// 	// }

			// }

			// if (i == 2) {
			// 	string query = "select * from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " in ('private','ecri','ecr_i','http')";
			// 	global_query_builder.query(query);
			// 	global_query_builder.get_result();
			// 	bool pass1 = global_query_builder.get_jml_row() > 0;
			// 	query = "select * from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " not in ('private','ecri','ecr_i','http')";
			// 	global_query_builder.query(query);
			// 	global_query_builder.get_result();
			// 	bool pass2 = global_query_builder.get_jml_row() > 0;

			// 	if (pass1 and pass2)
			// 	{
			// 		pass = false;
			// 		string tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) (select 'private;ecri;ecr_i;http',label,count(*) as jml from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " in ('private','ecri','ecr_i','http') group by label)";
			// 		global_query_builder.query(tmp);
			// 	}
			// }

			// if (i == 3)
			// {
			// 	string query = "select * from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + "='SF'";
			// 	global_query_builder.query(query);
			// 	global_query_builder.get_result();
			// 	bool pass1 = global_query_builder.get_jml_row() > 0;
			// 	query = "select * from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + "!='SF'";
			// 	global_query_builder.query(query);
			// 	global_query_builder.get_result();
			// 	bool pass2 = global_query_builder.get_jml_row() > 0;

			// 	if (pass1 and pass2)
			// 	{
			// 		pass = false;
			// 		string tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) (select 'SF',label,count(*) as jml from " + _tmp_attr_dataset_tb + " a inner join tb_index b on a.id=b.idx_row where " + group_kolom + " ='SF' group by label)";
			// 		global_query_builder.query(tmp);
			// 	}
			// }


			// if (pass) {
				tmp = "call sp_hit_stat1('attr" + to_string(i) + "','" + _tmp_attr_dataset_tb + "','" + group_kolom + "')";
				global_query_builder.query(tmp);

				vector<string> lst_label;

				bool is_kombinasi = false;

				string query = "select distinct " + group_kolom + ",count(*) as jml_data from attr" + to_string(i) + " group by " + group_kolom + " having jml_data>1 ";
				if (global_query_builder.query(query))
				{
					if (global_query_builder.get_result())
					{
						int jml_row = global_query_builder.get_jml_row();

						is_kombinasi = true;

						if (jml_row > 0) {

							while (jml_row > 0)
							{
								vector<string> data = global_query_builder.fetch_row();
								lst_label.push_back(data[0]);
								jml_row--;
							}

						}
					}
				}

				if (is_kombinasi) {
					size_t jml_kombinasi = 2;
					while ((lst_label.size() >= (2 * jml_kombinasi)) and (jml_kombinasi < 4) ) {
						gen_permutation(lst_label, jml_kombinasi - 1, "attr" + to_string(i), group_kolom);
						jml_kombinasi++;
					}
				}

				lst_label.clear();
				lst_label.shrink_to_fit();
			//}
		}

	}


	return col_split;
}


void tb_dataset::update_attr_stat(int idx)
{
	string tmp = "delete from attr_stat where id=" + to_string(idx);
	global_query_builder.query(tmp);

	tmp = "select count(*) as jml from ( select distinct " + _data_header[idx] + " from attr" + to_string(idx) + ") tb";
	if (global_query_builder.query(tmp))
	{
		if (global_query_builder.get_result())
		{
			vector<string> data = global_query_builder.fetch_row();
			tmp = "insert into attr_stat values(" + to_string(idx) + ",'" + _data_header[idx] + "'," + (_data_type[idx] == "continuous." ? "1" : "0") + "," + data[0] + ")";
			global_query_builder.query(tmp);
		}
	}
}

void tb_dataset::clear_tmp_dataset()
{
	string tmp_sql = "truncate " + _tmp_dataset_tb + "";
	global_query_builder.query(tmp_sql);
}

void tb_dataset::clear_tb_index()
{
	string tmp_sql = "truncate tb_index";
	global_query_builder.query(tmp_sql);
}

void tb_dataset::clear_tb_index1()
{
	string tmp_sql = "truncate tb_index1";
	global_query_builder.query(tmp_sql);
}

void tb_dataset::child_to_tmp_dataset()
{

	string query = "call sp_child_to_tmp_dataset('" + _tmp_dataset_tb + "','" + _tmp_attr_dataset_tb + "','" + _partition + "','" + to_string(_id_dt) + "','" + to_string(_jns_dt) + "')";
	global_query_builder.query(query);

	// clear_tmp_dataset();

	// string query = "insert into " + _tmp_dataset_tb + " (select id,0,0,0,0,0,0 from dataset partition(" + _partition + ") order by label,id)";
	// global_query_builder.query(query);

	// clear_tb_index();

	// query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where child_depth=0 and child_branch=0 and child_branch_number=0 order by id_row)";
	// global_query_builder.query(query);
}

void tb_dataset::reset_depth_branch()
{
	string query = "update " + _tmp_dataset_tb + " set child_depth=0,child_branch=0,child_branch_number=0,parent_depth=0,parent_branch=0,parent_branch_number=0";
	global_query_builder.query(query);

	clear_tb_index();

	query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where child_depth=0 and child_branch=0 and child_branch_number=0 order by id_row)";
	global_query_builder.query(query);
}

void tb_dataset::clear_child_parent()
{
	string where_str  = _posisi_cabang.to_query(0);

	string set_str   = _posisi_cabang.to_query(5);

	string query = "update " + _tmp_dataset_tb + " set " + set_str + " where " + where_str;
	global_query_builder.query(query);
	clear_tb_index();
}

void tb_dataset::set_child_parent()
{
	string where_str  = _posisi_cabang.to_query(0);

	string set_str   = _posisi_cabang.to_query(7);

	string query = "update " + _tmp_dataset_tb + " set " + set_str + " where " + where_str;
	global_query_builder.query(query);
	clear_tb_index();
}

void tb_dataset::delete_child(int child_depth, int child_branch, int child_branch_number)
{

	string set_str  = "";
	set_str  = "child_depth = 0,";
	set_str += "child_branch = 0,";
	set_str += "child_branch_number = 0,";
	set_str += "parent_depth = 0,";
	set_str += "parent_branch = 0,";
	set_str += "parent_branch_number = 0";

	string where_str  = "";
	where_str  = "((child_depth=" + to_string(child_depth) + ") and ";
	where_str += "(child_branch=" + to_string(child_branch) + ") and ";
	where_str += "(child_branch_number=" + to_string(child_branch_number) + "))";

	string query = "update " + _tmp_dataset_tb + " set " + set_str + " where " + where_str;
	global_query_builder.query(query);
}

void tb_dataset::train_to_test()
{
	string tmp_sql = "truncate tmp_dataset_test";
	global_query_builder.query(tmp_sql);

	tmp_sql = "insert into tmp_dataset_test(id_row,child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number) (select id_row,child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number from tmp_dataset_train)";
	global_query_builder.query(tmp_sql);

}

