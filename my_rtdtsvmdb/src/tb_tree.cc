#include "tb_tree.h"



tb_tree::tb_tree()
{

}

tb_tree::~tb_tree()
{
	global_query_builder.close_connection();
}

void tb_tree::baca_tree()
{
	string tmp = "select * from tree order by id";
    global_query_builder.open_connection(); 
	
	if (global_query_builder.query(tmp))
	{
		if (global_query_builder.get_result())
		{
			_jml_row = global_query_builder.get_jml_row();
			_jml_col = global_query_builder.get_jml_col();			
		}
	}

}


void tb_tree::clear_data()
{
	_data.clear();
	_data.shrink_to_fit();
}

void tb_tree::reset_file()
{
	global_query_builder.go_to_no_record(0);
	read_file();
}

void tb_tree::read_file()
{
	clear_data();

	vector<string> data = global_query_builder.fetch_row();

	if (data.size() > 0) {
		int awal = 1;

		for (int i = awal; i < (_jml_col - 2); i++)
		{
			_data.push_back(data[i]);
		}

	}
}

void tb_tree::close_file()
{
	global_query_builder.close_connection();
}


bool tb_tree::is_eof()
{
	return _data.size() == 0;
}


void tb_tree::next_record()
{
	read_file();
}


vector<string> tb_tree::get_record()
{
	return _data;
}


