#include "tb_dataset.h"


tb_dataset::tb_dataset()
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

	global_query_builder.close_connection();
}

tb_dataset::~tb_dataset()
{
	global_query_builder.close_connection();
	_data.clear();
	_data.shrink_to_fit();
	_data_header.clear();
	_data_header.shrink_to_fit();
	_data_type.clear();
	_data_type.shrink_to_fit();
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

void tb_dataset::filter(string sql)
{
	if (sql != "")
	{

		string query = "delete from tmp_dataset where " + sql;

		global_query_builder.open_connection();
		global_query_builder.query(query);
		global_query_builder.close_connection();

		query = "insert into tmp_dataset(id,depth,branch,duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count,dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label) (select id," + to_string(_child_depth) + "," + to_string(_child_branch) + ",duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count,dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label from dataset partition(" + _partition + ") where " + sql + ")";

		global_query_builder.open_connection();
		global_query_builder.query(query);
		global_query_builder.close_connection();

	}



	global_query_builder.open_connection();
	string query = "select count(*) as jml from tmp_dataset where depth=" + to_string(_child_depth) + " and branch=" + to_string(_child_branch);

	if (global_query_builder.query(query))
	{
		if (global_query_builder.get_result())
		{
			vector<string> tmp = global_query_builder.fetch_row();
			_jml_row = stoi(tmp[0]);
		}
	}

	global_query_builder.close_connection();
}


void tb_dataset::read_hsl_filter()
{
	global_query_builder.open_connection();

	string query = "select * from tmp_dataset where depth =" + to_string(_child_depth) + " and branch = " + to_string(_child_branch) + " order by id";

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

	global_query_builder.open_connection();

	string query = "select label,count(label) as jml from tmp_dataset where depth =" + to_string(_child_depth) + " and branch = " + to_string(_child_branch) + " group by label order by label";

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

	global_query_builder.close_connection();

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
		global_query_builder.open_connection();

		string tmp = "truncate attr" + to_string(i);
		global_query_builder.query(tmp);

		if (_data_type[i] == "continuous.")
		{
			tmp = "select round(" + group_kolom + ",7) as hsl_round,label from tmp_dataset where depth =" + to_string(_child_depth) + " and branch = " + to_string(_child_branch);

			tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select hsl_round,label,count(label) as jml from (" + tmp + ") tb group by hsl_round,label order by hsl_round,label";

			global_query_builder.query(tmp);

		} else {

			tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select " + group_kolom + ",label,count(label) as jml from tmp_dataset where depth =" + to_string(_child_depth) + " and branch = " + to_string(_child_branch);

			tmp = tmp + " group by " + group_kolom + ",label order by " + group_kolom + ",label";

			global_query_builder.query(tmp);

		}

		global_query_builder.close_connection();
	}


	return col_split;
}


void tb_dataset::update_attr_stat(int idx)
{
	global_query_builder.open_connection();
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
	global_query_builder.close_connection();
}

void tb_dataset::clear_tmp_dataset()
{
	global_query_builder.open_connection();

	string tmp_sql = "truncate tmp_dataset";
	global_query_builder.query(tmp_sql);
	global_query_builder.close_connection();
}

void tb_dataset::child_to_tmp_dataset()
{
	clear_tmp_dataset();
	global_query_builder.open_connection();
	string query = "insert into tmp_dataset(id,depth,branch,duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count,dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label) (select id,0,0,duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count,dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label from dataset partition(" + _partition + "))";
	global_query_builder.query(query);
	global_query_builder.close_connection();
}

void tb_dataset::reset_depth_branch()
{
	global_query_builder.open_connection();
	string query = "update tmp_dataset set depth=0 and branch=0";
	global_query_builder.query(query);
	global_query_builder.close_connection();
}

