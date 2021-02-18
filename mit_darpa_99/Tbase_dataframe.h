#include "Tread_file.h"
#include <vector>
#include <string>
#include <map>

using namespace std;


#ifndef Included_Tbase_dataframe_H

#define Included_Tbase_dataframe_H

struct field_filter
{
	int idx_col;
	int idx_opt;
	string value;
};


class Tbase_dataframe
{

protected:
	Tread_file _data;
	vector<field_filter> _filter;
	map<string, int> _stat_label;

	bool is_index = false;

	string _nm_file;	

	int _jml_col = 0;
	int _jml_row = 0;	

public:
	Tbase_dataframe();
	~Tbase_dataframe();

	Tbase_dataframe& operator = (const Tbase_dataframe &t)
	{
		this->_data = t._data;
		this->_filter = t._filter;
		this->_jml_col = t._jml_col;
		
		return *this;
	}

	void read_data(string nm_f,string separator);
	void read_data(string nm_f);
	void write_data(vector<string> &data);
	void add_filter(field_filter filter);
	void clear_filter();
	void ReFilter();
	vector<field_filter> get_filter();
	int getjmlcol();
	int getjmlrow();
	virtual bool is_pass(vector<string> &data);
	virtual void stat_tabel();
	map<string, int> get_stat_label();

	bool open_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

	void info();
	virtual void head();

	void clear_memory();

};

#endif