#include "tb_missing_branch.h"

Tquery_builder tb_missing_branch::global_query_builder;


tb_missing_branch::tb_missing_branch()
{
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
	
}

void tb_missing_branch::open_connection()
{
    tb_missing_branch::global_query_builder.open_connection();
}

void tb_missing_branch::close_connection()
{
	tb_missing_branch::global_query_builder.close_connection();
}

void tb_missing_branch::clear_table()
{
	string tmp_sql = "truncate missing_branch";
	tb_missing_branch::global_query_builder.query(tmp_sql);

	tmp_sql = "truncate detail_missing_branch";
	tb_missing_branch::global_query_builder.query(tmp_sql);

}

void tb_missing_branch::add_branch(Tposisi_cabang posisi, int attrindex, int opt, Tmy_dttype attrvalue)
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
		values += "'" + attrvalue.get_string() + "',";
		values += to_string(opt);

		string query = "insert into missing_branch(child_depth,child_branch,child_branch_number,parent_depth,parent_branch,parent_branch_number,attrindex,attrvalue,opt,is_pure,is_min_sample,is_depth_limit,is_same_label,is_pruning,is_not_split) values(" + values + ",0,0,0,0,0,0)";
		tb_missing_branch::global_query_builder.query(query);
	} else {
		string where_str  = posisi.to_query(0);
		
		string set_str = "";
		set_str = "is_pure=0,";
		set_str += "is_min_sample=0,";
		set_str += "is_depth_limit=0,";
		set_str += "is_not_split=0,";
		set_str += "is_pruning=0,";
		set_str += "is_same_label=0,";
		set_str += "label='-1',";
		set_str += "attrindex=" + to_string(attrindex) + ",";
		set_str += "attrvalue='" + attrvalue.get_string() + "'";

		if (opt != -1)
		{
			set_str += ",opt=" + to_string(opt);
		}


		string query = "update missing_branch set " + set_str + " where " + where_str;
		tb_missing_branch::global_query_builder.query(query);
	}

}

bool tb_missing_branch::cabang_exixst(Tposisi_cabang posisi)
{
	bool hsl = false;

	string where_str  = posisi.to_query(0);

	string query = "select count(*) as jml from missing_branch where " + where_str;

	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> tmp = tb_missing_branch::global_query_builder.fetch_row();
			hsl = stoi(tmp[0]) > 0;
		}
	}

	return hsl;
}



void tb_missing_branch::insert_not_split(Tposisi_cabang posisi, string label, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string set_str = "";
		set_str = "is_not_split=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		tb_missing_branch::global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_same_label(Tposisi_cabang posisi, string label, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string set_str = "";
		set_str = "is_same_label=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		tb_missing_branch::global_query_builder.query(query);

	}
}

void tb_missing_branch::insert_cut_off(Tposisi_cabang posisi, string label, int is_pure, int is_min_sample, int is_depth_limit, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string set_str = "";
		set_str = "is_pure=" + to_string(is_pure) + ",";
		set_str += "is_min_sample=" + to_string(is_min_sample) + ",";
		set_str += "is_depth_limit=" + to_string(is_depth_limit) + ",";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		tb_missing_branch::global_query_builder.query(query);

	}
}


void tb_missing_branch::insert_pruning(Tposisi_cabang posisi, string label, int is_lanjut)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string set_str = "";
		set_str = "is_pruning=1,";
		set_str += "is_lanjut=" + to_string(is_lanjut) + ",";
		set_str += "label='" + label + "'";

		string query = "update missing_branch set " + set_str + " where " + where_str;
		tb_missing_branch::global_query_builder.query(query);
	}
}

void tb_missing_branch::get_split(Tposisi_cabang posisi, int &attrindex, int &opt, Tmy_dttype &attrvalue)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string query = "select * from missing_branch where " + where_str;

		if (tb_missing_branch::global_query_builder.query(query))
		{
			if (tb_missing_branch::global_query_builder.get_result())
			{

				vector<string> data = tb_missing_branch::global_query_builder.fetch_row();

				if (data.size() > 0) {
					attrindex = stoi(data[6]);
					opt = stoi(data[9]);
					attrvalue.set_value(data[7],(opt==0) or (opt==1));
				}
			}
		}

	} else {
		attrindex = -1;
		attrvalue.set_value("-1",true);
	}
}

bool tb_missing_branch::parent_exixst(Tposisi_cabang posisi)
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

	where_str += posisi.to_query(2);
	
	string query = "select count(*) as jml from missing_branch where " + where_str;

	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> tmp = tb_missing_branch::global_query_builder.fetch_row();
			hsl = stoi(tmp[0]) > 0;
		}
	}

	return hsl;
}


void tb_missing_branch::get_stat(Tposisi_cabang posisi)
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
		string where_str  = posisi.to_query(0);

		string query = "select * from missing_branch where " + where_str;

		if (tb_missing_branch::global_query_builder.query(query))
		{
			if (tb_missing_branch::global_query_builder.get_result())
			{

				vector<string> data = tb_missing_branch::global_query_builder.fetch_row();

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

void tb_missing_branch::delete_cabang(Tposisi_cabang posisi)
{
	if (cabang_exixst(posisi))
	{
		string where_str  = posisi.to_query(0);

		string query = "delete from missing_branch where " + where_str;

		tb_missing_branch::global_query_builder.query(query);

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

	// string query = "select * from missing_branch where " + where_str;
	// if (global_query_builder.query(query))
	// {
	// 	if (global_query_builder.get_result())
	// 	{
	// 		bool baca = true;
	// 		while (baca) {
	// 			vector<string> data = global_query_builder.fetch_row();

	// 			if (data.size() > 0) {
	// 				string where_str1  = "";
	// 				where_str1  = "child_depth=" + data[0] + " and ";
	// 				where_str1 += "child_branch=" + data[1] + " and ";
	// 				where_str1 += "child_branch_number=" + data[2] + " and ";
	// 				where_str1 += "parent_depth=" + data[3] + " and ";
	// 				where_str1 += "parent_branch=" + data[4] + " and ";
	// 				where_str1 += "parent_branch_number=" + data[5];

	// 				query = "delete from detail_missing_branch where " + where_str1;
	// 				global_query_builder.query(query);
	// 			} else {
	// 				baca = false;
	// 			}


	// 		}
	// 	}
	// }


	string query = "delete from missing_branch where " + where_str;
	tb_missing_branch::global_query_builder.query(query);
}

