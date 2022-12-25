#include "tb_missing_branch.h"


tb_missing_branch::tb_missing_branch()
{
	global_query_builder.open_connection();
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


void tb_missing_branch::insert_not_split(posisi_cabang posisi, int attrindex, string attrvalue)
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
		values += "'" + attrvalue + "'";

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,attrindex,attrvalue,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0,0,0,1)";
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
		set_str  = "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue + "',";
		set_str += "is_pure=0,";
		set_str += "is_min_sample=0,";
		set_str += "is_depth_limit=0,";
		set_str += "is_same_label=0,";
		set_str += "is_pruning=0,";
		set_str += "is_not_split=1";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_same_label(posisi_cabang posisi, int attrindex, string attrvalue)
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
		values += "'" + attrvalue + "'";

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,attrindex,attrvalue,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0,1,0,0)";
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
		set_str  = "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue + "',";
		set_str += "is_pure=0,";
		set_str += "is_min_sample=0,";
		set_str += "is_depth_limit=0,";
		set_str += "is_same_label=1,";
		set_str += "is_pruning=0,";
		set_str += "is_not_split=0";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_cut_off(posisi_cabang posisi, int is_pure, int is_min_sample, int is_depth_limit)
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
		values += to_string(is_pure) + ",";
		values += to_string(is_min_sample) + ",";
		values += to_string(is_depth_limit);

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0)";
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
		set_str = "is_pure=" + to_string(is_pure) + ",";
		set_str += "is_min_sample=" + to_string(is_min_sample) + ",";
		set_str += "is_depth_limit=" + to_string(is_depth_limit) + ",";
		set_str += "is_same_label=0,";
		set_str += "is_pruning=0,";
		set_str += "is_not_split=0";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

void tb_missing_branch::update_split(posisi_cabang posisi, int attrindex, string attrvalue)
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
		set_str = "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue + "'";


		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);
	}
}

void tb_missing_branch::insert_pruning(posisi_cabang posisi, int attrindex, string attrvalue)
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
		values += "'" + attrvalue + "'";

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,attrindex,attrvalue,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0,0,1,0)";
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
		set_str  = "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue + "',";
		set_str += "is_pure=0,";
		set_str += "is_min_sample=0,";
		set_str += "is_depth_limit=0,";
		set_str += "is_same_label=0,";
		set_str += "is_pruning=1,";
		set_str += "is_not_split=0";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		global_query_builder.query(query);

	}
}

