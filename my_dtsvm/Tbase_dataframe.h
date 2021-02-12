#include "Tread_file.h"
#include "Tlabel_stat.h"
#include "Tmy_dttype.h"
#include <string>

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
	vector<string> _data_header;
	vector<string> _data_type;
	vector<field_filter> _filter;
	Tlabel_stat _stat_label;

	int _jml_col = 0;
	int _jml_row = 0;
	int _idx_label;
	string _nm_file;

	bool is_index = false;

	void cetak ( const char * format, ... );

	int _id = 0;

public:
	Tbase_dataframe();
	~Tbase_dataframe();
	Tbase_dataframe& operator = (const Tbase_dataframe &t)
	{
		this->_data = t._data;
		this->_data_header = t._data_header;
		this->_data_type = t._data_type;
		this->_filter = t._filter;
		this->_jml_col = t._jml_col;
		this->_idx_label = t._idx_label;
		return *this;
	}

	void set_id(int id);
	int get_id();

	void read_data(string nm_f);
	void read_data_type(string nm_f);
	void write_data(vector<string> &data);

	bool is_pass(vector<string> &data);

	void stat_tabel();
	map<string, int> get_stat_label();


	int getjmlcol();
	int getjmlrow();

	bool open_file();
	void read_file();
	void close_file();
	bool is_eof();
	void next_record();
	vector<string> get_record();

	void add_filter(int idx_col, int idx_opt, string value);
	void add_filter(field_filter filter);
	void ReFilter();
	vector<field_filter> get_filter();

	void info();
	void clear_memory();

};

#endif