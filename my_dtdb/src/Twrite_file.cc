#include "Twrite_file.h"

Twrite_file::Twrite_file()
{

}

Twrite_file::~Twrite_file()
{
	close_file();
}

void Twrite_file::setnm_f(string nm_tb)
{
	_nm_tb = nm_tb;
}

void Twrite_file::close_file()
{
	global_query_builder.close_connection();
}

void Twrite_file::write_file(string nm_kolom, string data)
{

}

void Twrite_file::write_file(string nm_kolom, vector<string> &data)
{
	global_query_builder.open_connection();

	for (size_t i = 0; i < data.size(); ++i)
	{
		string sql = "insert into " + _nm_tb + "(" + nm_kolom + ") values(" + data[i] + ")";
		global_query_builder.query(sql);
	}

	global_query_builder.close_connection();
}