#include "tb_dataset.h"


tb_dataset::tb_dataset()
{
	_tmp_dataset_tb = "tmp_dataset_train_svm";
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
	_tmp_dataset_tb = "tmp_dataset_test_svm";
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
			for (int i = 3; i < _jml_col; i++)
			{
				_data_header.push_back(tmp_header[i]);
				_data_type.push_back(tmp_type[i]);

				_idx_label = i - 3;
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

string tb_dataset::get_id_row()
{
	return _id_row;
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
		_id_row = data[0];

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
	_parent_depth = depth;
	_parent_branch = branch;
	_parent_branch_number = branch_number;
}

void tb_dataset::set_child(int depth, int branch, int branch_number)
{
	_child_depth = depth;
	_child_branch = branch;
	_child_branch_number = branch_number;
}

void tb_dataset::switch_parent_child()
{
	_parent_depth = _child_depth;
	_parent_branch = _child_branch;
	_parent_branch_number = _child_branch_number;
}

void tb_dataset::set_label_idx_svm(int idx_svm, string label)
{
	string where_str  = "";
	where_str  = "child_depth=" + to_string(_child_depth) + " and ";
	where_str += "child_branch=" + to_string(_child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(_parent_branch_number);

	string query = "update " + _tmp_dataset_tb + " set no_svm =" + to_string(idx_svm) + " , label='" + label + "' where " + where_str;
	global_query_builder.query(query);

}

void tb_dataset::set_label_svm(string id_row, string label)
{
	string query = "update " + _tmp_dataset_tb + " set label='" + label + "' where  id_row='" + id_row + "'";
	global_query_builder.query(query);
}

bool tb_dataset::is_child_parent_exist()
{
	bool hsl = false;

	string where_str  = "";
	where_str  = "child_depth=" + to_string(_child_depth) + " and ";
	where_str += "child_branch=" + to_string(_child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(_parent_branch_number);

	string query = "select count(*) as jml from " + _tmp_dataset_tb + " where " + where_str;

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

bool tb_dataset::is_parent_exist(int idx)
{
	bool hsl = false;

	string where_str  = "";

	where_str  = "child_depth=" + to_string(_parent_depth) + " and ";
	where_str += "child_branch=" + to_string(_parent_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_parent_branch_number);
	if (idx == 1) {
		where_str  = "parent_depth=" + to_string(_child_depth) + " and ";
		where_str += "parent_branch=" + to_string(_child_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(_child_branch_number);
	}
	string query = "select count(*) as jml from " + _tmp_dataset_tb + " where " + where_str;

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

bool tb_dataset::exist_in_detil(int idx)
{
	bool hsl = false;

	string where_str  = "";
	where_str  = "child_depth=" + to_string(_child_depth) + " and ";
	where_str += "child_branch=" + to_string(_child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(_parent_branch_number);

	if (idx == 1)
	{
		where_str  = "";
		where_str  = "child_depth=" + to_string(_parent_depth) + " and ";
		where_str += "child_branch=" + to_string(_parent_branch) + " and ";
		where_str += "child_branch_number=" + to_string(_parent_branch_number);
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

posisi_cabang tb_dataset::get_posisi_parent()
{
	posisi_cabang tmp_posisi;

	string where_str  = "";
	where_str  = "child_depth=" + to_string(_parent_depth) + " and ";
	where_str += "child_branch=" + to_string(_parent_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_parent_branch_number);

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

		bool is_pass1 = true;

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
				set_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_child_depth) + ",";
				set_str += _tmp_dataset_tb + ".child_branch=" + to_string(_child_branch) + ",";
				set_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_child_branch_number) + ",";
				set_str += _tmp_dataset_tb + ".parent_depth=" + to_string(_parent_depth) + ",";
				set_str += _tmp_dataset_tb + ".parent_branch=" + to_string(_parent_branch) + ",";
				set_str += _tmp_dataset_tb + ".parent_branch_number=" + to_string(_parent_branch_number);

				// string where_str  = "";
				// where_str  = _tmp_dataset_tb + ".child_depth=" + to_string(_parent_depth) + " and ";
				// where_str += _tmp_dataset_tb + ".child_branch=" + to_string(_parent_branch) + " and ";
				// where_str += _tmp_dataset_tb + ".child_branch_number=" + to_string(_parent_branch_number);
				// query = "update (" + _tmp_dataset_tb + " inner join tb_index1 on id_row=idx_row) set " + set_str + " where (" + where_str + ")";
				// global_query_builder.query(query);

				query = "call sp_filter(" + to_string(_child_depth) + "," + to_string(_child_branch) + "," + to_string(_child_branch_number) + "," + to_string(_parent_depth) + "," + to_string(_parent_branch) + "," + to_string(_parent_branch_number) + "," + '"' + sql + '"' + ",'" + _tmp_dataset_tb + "','" + _partition + "')";
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

						query = "call sp_filter1(" + to_string(_child_depth) + "," + to_string(_child_branch) + "," + to_string(_child_branch_number) + "," + to_string(_parent_depth) + "," + to_string(_parent_branch) + "," + to_string(_parent_branch_number) + ",'" + _tmp_dataset_tb + "')";
						global_query_builder.query(query);

					} else {
						// if ((_tmp_dataset_tb == "tmp_dataset_train"))
						// {
						// 	cetak(" kacau 1 !!! ");
						// }

						// query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") where (" + sql + "))";
						// global_query_builder.query(query);
						query = "call sp_filter2(" + to_string(_child_depth) + "," + to_string(_child_branch) + "," + to_string(_child_branch_number) + "," + to_string(_parent_depth) + "," + to_string(_parent_branch) + "," + to_string(_parent_branch_number) + "," + '"' + sql + '"' + ",'" + _tmp_dataset_tb + "','" + _partition + "')";
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
					// {
					// 	cetak(" kacau 2 !!! ");
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

		string query = "call sp_filter3(" + to_string(_child_depth) + "," + to_string(_child_branch) + "," + to_string(_child_branch_number) + "," + to_string(_parent_depth) + "," + to_string(_parent_branch) + "," + to_string(_parent_branch_number) + ",'" + _tmp_dataset_tb + "')";
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

	string query = "select dataset.* from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row order by dataset.id";

	_jml_row = 0;

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			_jml_row = global_query_builder.get_jml_row();
		}
	}

}

void tb_dataset::filter_by_idx_svm(int idx_svm)
{
	clear_tb_index();

	string where_str  = "";
	where_str  = "no_svm =" + to_string(idx_svm) + " ";

	string query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where " + where_str + " order by id_row)";
	global_query_builder.query(query);

	query = "select count(*) as jml from tb_index";

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



Tlabel_stat tb_dataset::hit_label_stat()
{
	Tlabel_stat label_stat;

	string query = "select label,count(label) as jml from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row group by label order by label";

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

			tmp = "call sp_hit_stat('attr" + to_string(i) + "','" + _partition + "','" + group_kolom + "')";

			// tmp = "select round(" + group_kolom + ",7) as hsl_round,label from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row";

			// tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select hsl_round,label,count(label) as jml from (" + tmp + ") tb group by hsl_round,label order by hsl_round,label";

			global_query_builder.query(tmp);

		} else {

			tmp = "call sp_hit_stat1('attr" + to_string(i) + "','" + _partition + "','" + group_kolom + "')";

			// tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select " + group_kolom + ",label,count(label) as jml from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row";

			// tmp = tmp + " group by " + group_kolom + ",label order by " + group_kolom + ",label";

			global_query_builder.query(tmp);

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

	// string query = "call sp_child_to_tmp_dataset('" + _tmp_dataset_tb + "','" + _partition + "')";
	// global_query_builder.query(query);

	clear_tmp_dataset();

	string query = "insert into " + _tmp_dataset_tb + " (select id,0,0,0,0,0,0,0,'-1' from dataset partition(" + _partition + ") order by label,id)";
	global_query_builder.query(query);

	clear_tb_index();

	query = "insert into tb_index(idx_row) (select id_row from " + _tmp_dataset_tb + " where child_depth=0 and child_branch=0 and child_branch_number=0 order by id_row)";
	global_query_builder.query(query);
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
	string where_str  = "";
	where_str  = "child_depth=" + to_string(_child_depth) + " and ";
	where_str += "child_branch=" + to_string(_child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(_parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(_parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(_parent_branch_number);

	string set_str  = "";
	set_str  = "child_depth = " + to_string(_parent_depth) + ",";
	set_str += "child_branch = " + to_string(_parent_branch) + ",";
	set_str += "child_branch_number = " + to_string(_parent_branch_number) + ",";
	set_str += "parent_depth = " + to_string(_parent_depth) + ",";
	set_str += "parent_branch = " + to_string(_parent_branch) + ",";
	set_str += "parent_branch_number = " + to_string(_parent_branch_number);

	string query = "update " + _tmp_dataset_tb + " set " + set_str + " where " + where_str;
	global_query_builder.query(query);
	clear_tb_index();
}

void tb_dataset::set_child_parent()
{
	string where_str  = "";
	where_str  = "child_depth=" + to_string(_child_depth) + " and ";
	where_str += "child_branch=" + to_string(_child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(_child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(_child_depth) + " and ";
	where_str += "parent_branch=" + to_string(_child_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(_child_branch_number);

	string set_str  = "";
	set_str = "parent_depth = " + to_string(_parent_depth) + ",";
	set_str += "parent_branch = " + to_string(_parent_branch) + ",";
	set_str += "parent_branch_number = " + to_string(_parent_branch_number);

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
	where_str  = "child_depth=" + to_string(child_depth) + " and ";
	where_str += "child_branch=" + to_string(child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(child_branch_number);

	string query = "update " + _tmp_dataset_tb + " set " + set_str + " where " + where_str;
	global_query_builder.query(query);
}

void tb_dataset::train_to_test()
{
	string tmp_sql = "truncate tmp_dataset_test";
	global_query_builder.query(tmp_sql);

	tmp_sql = "insert into tmp_dataset_test (select * from tmp_dataset_train)";
	global_query_builder.query(tmp_sql);

}


vector<int> tb_dataset::get_idx_svm()
{
	vector<int> tmp_idx;

	string tmp_sql = "select no_svm from " + _tmp_dataset_tb + " where no_svm > -1 group by no_svm order by no_svm";
	if (global_query_builder.query(tmp_sql))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();
					tmp_idx.push_back(stoi(data[0]));
					jml_row--;
				}
			}
		}
	}

	return tmp_idx;
}


void tb_dataset::dtsvm_stat(time_t id_experiment, time_t id_detail_experiment, time_t id_experiment_dt, time_t id_detail_experiment_dt)
{
	string tmp_sql = "insert into dtsvm_stat(id_experiment,id_detail_experiment,id_experiment_dt,id_detail_experiment_dt,label_org,label_pred,jml) (select " + to_string(id_experiment) + "," + to_string(id_detail_experiment) + "," + to_string(id_experiment_dt) + "," + to_string(id_detail_experiment_dt) + ",dataset.label as label_org," + _tmp_dataset_tb + ".label as label_pred,count(*) as jml from dataset partition(" + _partition + ") inner join " + _tmp_dataset_tb + " on id_row=id group by dataset.label," + _tmp_dataset_tb + ".label)";
	global_query_builder.query(tmp_sql);
}

void tb_dataset::dtsvm_conf_metrix(time_t id_experiment, time_t id_detail_experiment, time_t id_experiment_dt, time_t id_detail_experiment_dt,Tconf_metrix &tmp_conf_metrix)
{
	string where_str  = "";
	where_str  = "id_experiment=" + to_string(id_experiment) + " and ";
	where_str += "id_detail_experiment=" + to_string(id_detail_experiment) + " and ";
	where_str += "id_experiment_dt=" + to_string(id_experiment_dt)+ " and ";
	where_str += "id_detail_experiment_dt=" + to_string(id_detail_experiment_dt);

	string query = "select label_org,label_pred,jml from dtsvm_stat where " + where_str;

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			int jml_row = global_query_builder.get_jml_row();

			if (jml_row > 0) {

				while (jml_row > 0)
				{
					vector<string> data = global_query_builder.fetch_row();
				    tmp_conf_metrix.add_jml(data[0], data[1], stoi(data[2]));               
					jml_row--;
				}
			}
		}
	}

	tmp_conf_metrix.kalkulasi();

	
}

