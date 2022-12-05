#include "tb_dataset.h"


tb_dataset::tb_dataset()
{

}

tb_dataset::~tb_dataset()
{
	//global_query_builder.close_connection();
	_data.clear();
	_data.shrink_to_fit();
	_data_header.clear();
	_data_header.shrink_to_fit();
	_data_type.clear();
	_data_type.shrink_to_fit();
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

	//global_query_builder.close_connection();
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

void tb_dataset::set_parent(int depth, int branch)
{
	_parent_depth = depth;
	_parent_branch = branch;
}

void tb_dataset::set_child(int depth, int branch)
{
	_child_depth = depth;
	_child_branch = branch;
}

void tb_dataset::switch_parent_child()
{
	_parent_depth = _child_depth;
	_parent_branch = _child_branch;
}

bool tb_dataset::is_child_parent_exist()
{
	bool hsl = false;

	//global_query_builder.open_connection();
	string query = "select count(*) as jml from tmp_dataset where child_depth=" + to_string(_child_depth) + " and child_branch=" + to_string(_child_branch) + " and  parent_depth=" + to_string(_parent_depth) + " and parent_branch=" + to_string(_parent_branch);

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			hsl = stoi(tmp[0]) > 0;
		}
	}

	//global_query_builder.close_connection();

	return hsl;
}

void tb_dataset::filter(string sql)
{
	if (sql != "")
	{
		if (!is_child_parent_exist())
		{
			clear_tb_index();
			clear_tb_index1();

			//global_query_builder.open_connection();
			string query = "insert into tb_index(idx_row) (select id_row from tmp_dataset where child_depth=" + to_string(_parent_depth) + " and child_branch=" + to_string(_parent_branch) + " order by id_row)";
			global_query_builder.query(query);

			query = "insert into tb_index1(idx_row) (select dataset.id from dataset partition(" + _partition + ") inner join tb_index on dataset.id = idx_row  where (" + sql + "))";
			global_query_builder.query(query);

			query = "update (tmp_dataset inner join tb_index1 on id_row=idx_row) set tmp_dataset.child_depth=" + to_string(_child_depth) + ",tmp_dataset.child_branch=" + to_string(_child_branch) + ",tmp_dataset.parent_depth=" + to_string(_parent_depth) + ",tmp_dataset.parent_branch=" + to_string(_parent_branch) + " where (tmp_dataset.child_depth=" + to_string(_parent_depth) + " and tmp_dataset.child_branch=" + to_string(_parent_branch) + ")";
			global_query_builder.query(query);
			//global_query_builder.close_connection();

			clear_tb_index1();
		}

		clear_tb_index();

		string query = "insert into tb_index(idx_row) (select id_row from tmp_dataset where child_depth=" + to_string(_child_depth) + " and child_branch=" + to_string(_child_branch) + " and parent_depth=" + to_string(_parent_depth) + " and parent_branch=" + to_string(_parent_branch) + " order by id_row)";

		//global_query_builder.open_connection();
		global_query_builder.query(query);
		//global_query_builder.close_connection();

	}

	//global_query_builder.open_connection();
	string query = "select count(*) as jml from tb_index";

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			_jml_row = stoi(tmp[0]);
		}
	}

	//global_query_builder.close_connection();
}


void tb_dataset::read_hsl_filter()
{
	//global_query_builder.open_connection();

	string query = "select * from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row order by dataset.id";

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

	//global_query_builder.open_connection();

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

	//global_query_builder.close_connection();

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
		//global_query_builder.open_connection();

		string tmp = "truncate attr" + to_string(i);
		global_query_builder.query(tmp);

		if (_data_type[i] == "continuous.")
		{
			tmp = "select round(" + group_kolom + ",7) as hsl_round,label from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row";

			tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select hsl_round,label,count(label) as jml from (" + tmp + ") tb group by hsl_round,label order by hsl_round,label";

			global_query_builder.query(tmp);

		} else {

			tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select " + group_kolom + ",label,count(label) as jml from dataset partition(" + _partition + ") inner join tb_index on dataset.id=idx_row";

			tmp = tmp + " group by " + group_kolom + ",label order by " + group_kolom + ",label";

			global_query_builder.query(tmp);

		}

		//global_query_builder.close_connection();
	}


	return col_split;
}


void tb_dataset::update_attr_stat(int idx)
{
	//global_query_builder.open_connection();
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
	//global_query_builder.close_connection();
}

void tb_dataset::clear_tmp_dataset()
{
	//global_query_builder.open_connection();

	string tmp_sql = "truncate tmp_dataset";
	global_query_builder.query(tmp_sql);
	//global_query_builder.close_connection();
}

void tb_dataset::clear_tb_index()
{
	//global_query_builder.open_connection();

	string tmp_sql = "truncate tb_index";
	global_query_builder.query(tmp_sql);
	//global_query_builder.close_connection();
}

void tb_dataset::clear_tb_index1()
{
	//global_query_builder.open_connection();
	string tmp_sql = "truncate tb_index1";
	global_query_builder.query(tmp_sql);
	//global_query_builder.close_connection();
}

void tb_dataset::child_to_tmp_dataset()
{
	clear_tmp_dataset();
	//global_query_builder.open_connection();
	string query = "insert into tmp_dataset (select id,0,0,0,0 from dataset partition(" + _partition + ") order by label,id)";
	global_query_builder.query(query);
	//global_query_builder.close_connection();

	clear_tb_index();
	//global_query_builder.open_connection();
	query = "insert into tb_index(idx_row) (select id_row from tmp_dataset where child_depth=0 and child_branch=0 order by id_row)";
	global_query_builder.query(query);
	//global_query_builder.close_connection();
}

void tb_dataset::reset_depth_branch()
{
	//global_query_builder.open_connection();
	string query = "update tmp_dataset set child_depth=0,child_branch=0,parent_depth=0,parent_branch=0";
	global_query_builder.query(query);
	//global_query_builder.close_connection();

	clear_tb_index();
	//global_query_builder.open_connection();
	query = "insert into tb_index(idx_row) (select id_row from tmp_dataset where child_depth=0 and child_branch=0 order by id_row)";
	global_query_builder.query(query);
	//global_query_builder.close_connection();
}

void tb_dataset::clear_child_parent()
{
	int new_child_depth = 0, new_child_branch = 0, new_parent_depth = 0, new_parent_branch = 0;

	//if ((_child_depth > 0) and (_parent_depth > 0))
	//{
	// if (_parent_depth == 1)
	// {
	// 	new_parent_depth = 0;
	// 	new_parent_branch = 0;
	// } else {
	// 	new_parent_depth = _parent_depth - 1;
	// 	new_parent_branch = _parent_branch;
	// }

	// if (_child_depth == 1)
	// {
	// 	new_child_depth = 0;
	// 	new_child_branch = 0;
	// } else {
	// 	new_child_depth = _child_depth - 1;
	// 	new_child_branch = _child_branch;
	// }

	//global_query_builder.open_connection();
	string query = "update tmp_dataset set child_depth=" + to_string(new_child_depth) + ",child_branch=" + to_string(new_child_branch) + ",parent_depth=" + to_string(new_parent_depth) + ",parent_branch=" + to_string(new_parent_branch) + " where child_depth=" + to_string(_child_depth) + " and child_branch=" + to_string(_child_branch) + " and parent_depth=" + to_string(_parent_depth) + " and parent_branch=" + to_string(_parent_branch);
	global_query_builder.query(query);
	//global_query_builder.close_connection();

	clear_tb_index();
	//}
}

void tb_dataset::delete_child(int child_depth, int child_branch)
{
	int new_child_depth = 0, new_child_branch = 0, new_parent_depth = 0, new_parent_branch = 0;
	//global_query_builder.open_connection();
	string query = "update tmp_dataset set child_depth=" + to_string(new_child_depth) + ",child_branch=" + to_string(new_child_branch) + ",parent_depth=" + to_string(new_parent_depth) + ",parent_branch=" + to_string(new_parent_branch) + " where child_depth=" + to_string(child_depth) + " and child_branch=" + to_string(child_branch);
	global_query_builder.query(query);
	//global_query_builder.close_connection();
}

