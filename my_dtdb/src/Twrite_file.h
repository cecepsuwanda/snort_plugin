#include <iostream>
#include <vector>
#include <string>

#include "Tquery_builder.h"

using namespace std;

#ifndef Included_Twrite_file_H

#define Included_Twrite_file_H

class Twrite_file
{
private:
	Tquery_builder global_query_builder;
	string _nm_tb;	

public:
	Twrite_file();
	~Twrite_file();

	void setnm_f(string nm_tb);
	void close_file();

	void write_file(string nm_kolom,vector<string> &data);
	void write_file(string nm_kolom,string data);

};

#endif