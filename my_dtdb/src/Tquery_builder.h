#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#ifndef Included_query_builder_H

#define Included_query_builder_H


class Tquery_builder
{

private:
    MYSQL *_con;
    MYSQL_RES *_result;

    string _url;
    string _user;
    string _pass;
    string _database;

    void finish_with_error();

public:
	Tquery_builder();
	~Tquery_builder();

	void close_connection();
	void open_connection();

	bool query(string sql);
	bool get_result();
	int get_jml_row();
	int get_jml_col();
	vector<string> get_column_name();
	vector<string> get_column_type();
	vector<string> fetch_row();
	void go_to_no_record(int no_rec);

	Tquery_builder& operator = (const Tquery_builder &t)
	{
         this->_con =t._con;
         return *this;
	}
};


#endif