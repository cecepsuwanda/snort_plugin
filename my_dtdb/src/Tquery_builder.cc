#include "Tquery_builder.h"



Tquery_builder::Tquery_builder()
{
	_url = "localhost";
	//_user = "root";
	_user = "cecepsuwanda";
	_pass = "198202";
	_database = "nsl";

	_con = NULL;
	_result = NULL;
}

Tquery_builder::~Tquery_builder()
{
	//close_connection();
}

void Tquery_builder::finish_with_error()
{
	string tmp = string(mysql_error(_con)) + " \n";
	perror(tmp.c_str());
	mysql_close(_con);
}

void Tquery_builder::open_connection()
{
	_con = mysql_init(NULL);

	if (_con == NULL)
	{
		perror(" mysql_init() failed \n");
	} else {
		if (mysql_real_connect(_con, _url.c_str(), _user.c_str(), _pass.c_str(), _database.c_str(), 0, NULL, 0) == NULL)
		{
			finish_with_error();
		}
	}
}

bool Tquery_builder::query(string sql)
{
	bool hsl = true;
	if (mysql_query(_con, sql.c_str()))
	{
		hsl = false;
		cout << sql << endl;
		finish_with_error();
	}
	return hsl;
}

bool Tquery_builder::get_result()
{
	bool hsl = true;
	_result = mysql_store_result(_con);
	if (_result == NULL)
	{
		hsl = false;
		finish_with_error();
	}
	return hsl;
}

int Tquery_builder::get_jml_row()
{
	int hsl = 0;
	if (_result != NULL)
	{
		hsl = mysql_num_rows(_result);
	}
	return hsl;
}

int Tquery_builder::get_jml_col()
{
	int hsl = 0;
	if (_result != NULL)
	{
		hsl = mysql_num_fields(_result);
	}
	return hsl;
}

vector<string> Tquery_builder::get_column_name()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		for (int i = 0; i < jml_col; ++i)
		{
			MYSQL_FIELD *field = mysql_fetch_field_direct(_result, i);
			tmp.push_back(field->name);
		}
	}
	return tmp;
}

vector<string> Tquery_builder::get_column_type()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		for (int i = 0; i < jml_col; ++i)
		{
			MYSQL_FIELD *field = mysql_fetch_field_direct(_result, i);
			if (field->type == FIELD_TYPE_VAR_STRING)
			{
				tmp.push_back("symbolic.");
			} else {
				tmp.push_back("continuous.");
			}
		}
	}
	return tmp;
}

vector<string> Tquery_builder::fetch_row()
{
	int jml_col = get_jml_col();
	vector<string> tmp;
	if (_result != NULL)
	{
		MYSQL_ROW row = mysql_fetch_row(_result);
		if (row != NULL) {
			for (int i = 0; i < jml_col; i++)
			{
				tmp.push_back(row[i]);
			}

		}
	}
	return tmp;
}

void Tquery_builder::go_to_no_record(int no_rec)
{
	if (_result != NULL)
	{
		mysql_data_seek(_result, no_rec);
	}
}

void Tquery_builder::close_connection()
{
	if (_result != NULL) {
		mysql_free_result(_result);
		_result = NULL;
	}
	if (_con != NULL) {
		mysql_close(_con);
		_con = NULL;
	}
}