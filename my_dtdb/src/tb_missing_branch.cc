#include "tb_missing_branch.h"


tb_missing_branch::tb_missing_branch()
{
	global_query_builder.open_connection();

	_is_pure = false;
	_is_min_sample = false;
	_is_depth_limit = false;
	_is_same_label = false;
	_is_pruning = false;
	_is_not_split = false;
	_is_lanjut = true;
	_label = "-1";
}

tb_missing_branch::~tb_missing_branch()
{
	global_query_builder.close_connection();
}


void tb_missing_branch::clear_table()
{
	string tmp_sql = "truncate missing_branch";
	global_query_builder.query(tmp_sql);
}

void tb_missing_branch::add_branch(posisi_cabang posisi, int attrindex, int opt, string attrvalue)
{
	if (!cabang_exixst(posisi))
	{
		string values = "";
		values  = to_string(posisi.child_depth) + ",";
		values += to_string(posisi.child_branch) + ",";
		values += to_string(posisi.child_branch_number) + ",";
		values += to_string(posisi.parent_depth) + ",";
		values += to_string(posisi.parent_branch) + ",";
		values += to_string(posisi.parent_branch_number) + ",";
		values += to_string(attrindex) + ",";
		values += "'" + attrvalue + "',";
		values += to_string(opt);

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,attrindex,attrvalue,opt,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0,0,0,0)";
		global_query_builder.query(query);
	} else {
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string set_str = "";
		set_str = "is_pure=0,";
		set_str += "is_min_sample=0,";
		set_str += "is_depth_limit=0,";
		set_str += "is_not_split=0,";
		set_str += "is_pruning=0,";
		set_str += "is_same_label=0,";
		set_str += "label='-1',";
		set_str += "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue + "'";

		if (opt != -1)
		{
			set_str += ",opt=" + to_string(opt);
		}


		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);
	}

}

bool tb_missing_branch::cabang_exixst(posisi_cabang posisi)
{
	bool hsl = false;

	string where_str  = "";
	where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
	where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
	where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
	where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);


	string query = "select count(*) as jml from missing_branch where " + where_str;

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


void tb_missing_branch::insert_not_split(posisi_cabang posisi, string label, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string set_str = "";
		set_str = "is_not_split=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_same_label(posisi_cabang posisi, string label,int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string set_str = "";
		set_str = "is_same_label=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_cut_off(posisi_cabang posisi, string label, int is_pure, int is_min_sample, int is_depth_limit, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string set_str = "";
		set_str = "is_pure=" + to_string(is_pure) + ",";
		set_str += "is_min_sample=" + to_string(is_min_sample) + ",";
		set_str += "is_depth_limit=" + to_string(is_depth_limit) + ",";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}


void tb_missing_branch::insert_pruning(posisi_cabang posisi, string label,int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string set_str = "";
		set_str = "is_pruning=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);
	}
}

void tb_missing_branch::get_split(posisi_cabang posisi, int &attrindex, int &opt, string &attrvalue)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string query = "select * from missing_branch where " + where_str;

		if (global_query_builder.query(query))
		{
			if (global_query_builder.get_result())
			{

				vector<string> data = global_query_builder.fetch_row();

				if (data.size() > 0) {
					attrindex = stoi(data[6]);
					attrvalue = data[7];
					opt = stoi(data[9]);
				}
			}
		}

	} else {
		attrindex = -1;
		attrvalue = "-1";
	}
}

bool tb_missing_branch::parent_exixst(posisi_cabang posisi)
{
	bool hsl = false;

	string where_str  = "";
	where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";

	if (posisi.child_branch != -1) {
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
	}

	if (posisi.child_branch_number != -1) {
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
	}

	where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
	where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
	where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

	string query = "select count(*) as jml from missing_branch where " + where_str;

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


void tb_missing_branch::get_stat(posisi_cabang posisi)
{
	_is_pure = false;
	_is_min_sample = false;
	_is_depth_limit = false;
	_is_same_label = false;
	_is_pruning = false;
	_is_not_split = false;
	_is_lanjut = true;
	_label = "-1";

	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string query = "select * from missing_branch where " + where_str;

		if (global_query_builder.query(query))
		{
			if (global_query_builder.get_result())
			{

				vector<string> data = global_query_builder.fetch_row();

				if (data.size() > 0) {
					_is_pure = data[10] == "1";
					_is_min_sample = data[11] == "1";
					_is_depth_limit = data[12] == "1";
					_is_same_label = data[13] == "1";
					_is_pruning = data[14] == "1";
					_is_not_split = data[15] == "1";
					_is_lanjut = data[16] == "1";
					_label = data[8];
				}
			}
		}

	}
}

bool tb_missing_branch::get_is_pure()
{
	return _is_pure;
}

bool tb_missing_branch::get_is_min_sample()
{
	return _is_min_sample;
}

bool tb_missing_branch::get_is_depth_limit()
{
	return _is_depth_limit;
}

bool tb_missing_branch::get_is_same_label()
{
	return _is_same_label;
}

bool tb_missing_branch::get_is_pruning()
{
	return _is_pruning;
}

bool tb_missing_branch::get_is_not_split()
{
	return _is_not_split;
}

bool tb_missing_branch::get_is_lanjut()
{
    return _is_lanjut;	
}

string tb_missing_branch::get_label()
{
	return _label;
}

void tb_missing_branch::delete_cabang(posisi_cabang posisi)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = "";
		where_str  = "child_depth=" + to_string(posisi.child_depth) + " and ";
		where_str += "child_branch=" + to_string(posisi.child_branch) + " and ";
		where_str += "child_branch_number=" + to_string(posisi.child_branch_number) + " and ";
		where_str += "parent_depth=" + to_string(posisi.parent_depth) + " and ";
		where_str += "parent_branch=" + to_string(posisi.parent_branch) + " and ";
		where_str += "parent_branch_number=" + to_string(posisi.parent_branch_number);

		string query = "delete from missing_branch where " + where_str;

		global_query_builder.query(query);

	}
}

void tb_missing_branch::delete_non_missing()
{
	string where_str  = "";
	where_str  = "is_pure=0 and ";
	where_str += "is_min_sample=0 and ";
	where_str += "is_depth_limit=0 and ";
	where_str += "is_same_label=0 and ";
	where_str += "is_pruning=0 and ";
	where_str += "is_not_split=0";

	string query = "delete from missing_branch where " + where_str;

	global_query_builder.query(query);
}

