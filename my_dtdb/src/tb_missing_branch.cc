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

	_is_split = false;
	_is_cut_off = false;

	_label = "-1";
	_jml_known = 0;
	_jml_normal = 0;

	_attrindex = -1;
	_attrvalue.set_value("-1", false);
	_opt = -1;
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

	tmp_sql = "truncate branch_histori";
	tb_missing_branch::global_query_builder.query(tmp_sql);

	tmp_sql = "truncate split_histori";
	tb_missing_branch::global_query_builder.query(tmp_sql);

	tmp_sql = "truncate opt_label_histori";
	tb_missing_branch::global_query_builder.query(tmp_sql);

	tmp_sql = "truncate cut_off_histori";
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

void tb_missing_branch::add_branch(Tposisi_cabang posisi)
{
	if (!cabang_exist(posisi))
	{
		string query = "insert into branch_histori values(" + posisi.to_query(8) + ")";
		tb_missing_branch::global_query_builder.query(query);
	}
}

void tb_missing_branch::add_split(Tposisi_cabang posisi, int attrindex, Tmy_dttype attrvalue)
{
	string query = "insert into split_histori values('" + posisi.get_id_branch() + "'," + to_string(attrindex) + ",'" + attrvalue.get_string() + "')";
	tb_missing_branch::global_query_builder.query(query);
}



void tb_missing_branch::add_opt_label(Tposisi_cabang posisi, int opt, string label, int jml_known, int jml_normal)
{
	string query = "insert into opt_label_histori values('" + posisi.get_id_branch() + "'," + to_string(opt) + ",'" + label + "'," + to_string(jml_known) + "," + to_string(jml_normal) + ")";
	tb_missing_branch::global_query_builder.query(query);
}


void tb_missing_branch::get_branch_stat(Tposisi_cabang posisi)
{
	_is_pure = false;
	_is_min_sample = false;
	_is_depth_limit = false;
	_is_same_label = false;
	_is_pruning = false;
	_is_not_split = false;
	_is_lanjut = true;

	_is_split = false;
	_is_cut_off = false;

	_label = "-1";
	_jml_known = 0;
	_jml_normal = 0;

	_attrindex = -1;
	_attrvalue.set_value("-1", false);
	_opt = -1;


	string query = "select b.* from branch_histori a inner join split_histori b on a.id_branch=b.id_branch where b.id_branch='" + posisi.get_id_branch() + "'";
	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();

			if (data.size() > 0) {
				_is_split = true;
				_attrindex = stoi(data[1]);
				_attrvalue.set_value(data[2], false);
			}
		}
	}


	query = "select b.* from branch_histori a inner join opt_label_histori b on a.id_branch=b.id_branch where b.id_branch='" + posisi.get_id_branch() + "'";
	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();

			if (data.size() > 0) {
				_opt = stoi(data[1]);
				_label = data[2];
				_jml_known = stoi(data[3]);
				_jml_normal = stoi(data[4]);
				_attrvalue.set_value(_attrvalue.get_string(), (_opt == 0) or (_opt == 1));
			}
		}
	}

	query = "select b.id_branch,if(b.kd_cut_off=11,1,0) as is_pure,if(b.kd_cut_off=12,1,0) as is_min_sample,if(b.kd_cut_off=13,1,0) as is_depth_limit,if(b.kd_cut_off=4,1,0) as is_same_label,if(b.kd_cut_off=5,1,0) as is_pruning,if(b.kd_cut_off=3 or b.kd_cut_off=2,1,0) as is_not_split from branch_histori a inner join cut_off_histori b on a.id_branch=b.id_branch where b.id_branch='" + posisi.get_id_branch() + "' group by b.id_branch,b.kd_cut_off";
	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{

			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();

			if (data.size() > 0) {
				_is_cut_off = true;
				_is_pure = data[1] == "1";
				_is_min_sample = data[2] == "1";
				_is_depth_limit = data[3] == "1";
				_is_same_label = data[4] == "1";
				_is_pruning = data[5] == "1";
				_is_not_split = data[6] == "1";
				_is_lanjut = !((data[1] == "1") or (data[2] == "1"));

			}
		}
	}



}

void tb_missing_branch::delete_histori(Tposisi_cabang posisi)
{
	string query = "delete from cut_off_histori where id_branch='" + posisi.get_id_branch() + "'";
	tb_missing_branch::global_query_builder.query(query);


}

void tb_missing_branch::add_cut_off(Tposisi_cabang posisi, int kd_cut_off)
{
	string query = "insert into cut_off_histori(id_branch,kd_cut_off) values('" + posisi.get_id_branch() + "'," +  to_string(kd_cut_off) + ")";
	tb_missing_branch::global_query_builder.query(query);
}


bool tb_missing_branch::get_split(int depth, int attrindex, Tmy_dttype attrvalue, Tposisi_cabang &posisi)
{
	bool hsl = false;

	string query = "select * from (branch_histori a inner join split_histori b on a.id_branch=b.id_branch) inner join opt_label_histori c on a.id_branch=c.id_branch where a.child_depth =" + to_string(depth) + " and b.attrindex=" + to_string(attrindex) + " and b.attrvalue='" + attrvalue.get_string() + "'";

	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();
			if (data.size() > 0) {
				hsl = true;
				posisi.child_depth = stoi(data[1]);
				posisi.child_branch = stoi(data[2]);
				posisi.child_branch_number = stoi(data[3]);
				posisi.parent_depth = stoi(data[4]);
				posisi.parent_branch = stoi(data[5]);
				posisi.parent_branch_number = stoi(data[6]);
				_label = data[12];
				_jml_known = stoi(data[13]);
				_jml_normal = stoi(data[14]);
			}
		}
	}


	return hsl;
}

bool tb_missing_branch::get_opt_label(int opt, Tposisi_cabang posisi_root, Tposisi_cabang &posisi_child)
{
	bool hsl = false;

	string query = "select * from (branch_histori a inner join opt_label_histori b on a.id_branch=b.id_branch) where a.id_branch like '%" + to_string(posisi_root.child_depth)+ to_string(posisi_root.child_branch)+ to_string(posisi_root.child_branch_number) + "' and b.opt=" + to_string(opt);
	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();
			if (data.size() > 0) {
				hsl = true;
				posisi_child.child_depth = stoi(data[1]);
				posisi_child.child_branch = stoi(data[2]);
				posisi_child.child_branch_number = stoi(data[3]);
				posisi_child.parent_depth = stoi(data[4]);
				posisi_child.parent_branch = stoi(data[5]);
				posisi_child.parent_branch_number = stoi(data[6]);
				_label = data[9];
				_jml_known = stoi(data[10]);
				_jml_normal = stoi(data[11]);
			}
		}
	}


	return hsl;
}

bool tb_missing_branch::get_opt_label_child(int child_branch, Tposisi_cabang posisi_root, Tposisi_cabang &posisi_child)
{
	bool hsl = false;

	string query = "select * from (branch_histori a inner join opt_label_histori b on a.id_branch=b.id_branch) where a.id_branch like '%" + to_string(posisi_root.child_depth)+ to_string(posisi_root.child_branch)+ to_string(posisi_root.child_branch_number) + "' and a.child_branch=" + to_string(child_branch);
	if (tb_missing_branch::global_query_builder.query(query))
	{
		if (tb_missing_branch::global_query_builder.get_result())
		{
			vector<string> data = tb_missing_branch::global_query_builder.fetch_row();
			if (data.size() > 0) {
				hsl = true;
				posisi_child.child_depth = stoi(data[1]);
				posisi_child.child_branch = stoi(data[2]);
				posisi_child.child_branch_number = stoi(data[3]);
				posisi_child.parent_depth = stoi(data[4]);
				posisi_child.parent_branch = stoi(data[5]);
				posisi_child.parent_branch_number = stoi(data[6]);
				_label = data[9];
				_jml_known = stoi(data[10]);
				_jml_normal = stoi(data[11]);
			}
		}
	}


	return hsl;
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

bool tb_missing_branch::cabang_exist(Tposisi_cabang posisi)
{
	bool hsl = false;

	string where_str  = posisi.to_query(0);

	string query = "select count(*) as jml from branch_histori where " + where_str;

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
					attrvalue.set_value(data[7], (opt == 0) or (opt == 1));
				}
			}
		}

	} else {
		attrindex = -1;
		attrvalue.set_value("-1", true);
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

int tb_missing_branch::get_jml_known()
{
	return _jml_known;
}

int tb_missing_branch::get_jml_normal()
{
	return _jml_normal;
}

int tb_missing_branch::get_attrindex()
{
	return _attrindex;
}

int tb_missing_branch::get_opt()
{
	return _opt;
}

Tmy_dttype tb_missing_branch::get_attrvalue()
{
	return _attrvalue;
}

bool tb_missing_branch::get_is_cut_off()
{
	return _is_cut_off;
}

bool tb_missing_branch::get_is_split()
{
	return _is_split;
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

