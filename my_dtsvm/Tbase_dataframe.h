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
	map<int, int> _unique_attr;
	Tlabel_stat _stat_label;

	int _jml_col = 0;
	int _jml_row = 0;
	int _jml_total_row = 0;
	int _idx_label;
	string _nm_file;

    bool is_non_continuous = false;
    bool is_42 = false;

	void cetak ( const char * format, ... );

	int _id = 0;

private:
	
public:
	Tbase_dataframe();
	Tbase_dataframe(const Tbase_dataframe &t)
	{
		_data = t._data;
		_data_header = t._data_header;
		_data_type = t._data_type;
		_filter = t._filter;
		_unique_attr = t._unique_attr;
		_jml_col = t._jml_col;
		_jml_row = t._jml_row;
		_stat_label = t._stat_label;
		_idx_label = t._idx_label;
		_nm_file = t._nm_file;
		is_non_continuous = t.is_non_continuous;
		is_42 = t.is_42;
		_jml_total_row = t._jml_total_row;
	}

	~Tbase_dataframe();
	Tbase_dataframe& operator = (const Tbase_dataframe &t)
	{
		this->_data = t._data;
		this->_data_header = t._data_header;
		this->_data_type = t._data_type;
		this->_filter = t._filter;
		this->_unique_attr = t._unique_attr;
		this->_jml_col = t._jml_col;
		this->_jml_row = t._jml_row;
		this->_stat_label = t._stat_label;
		this->_idx_label = t._idx_label;
		this->_nm_file = t._nm_file;
		this->is_non_continuous = t.is_non_continuous;
		this->is_42 = t.is_42;
		this->_jml_total_row = t._jml_total_row;
		return *this;
	}

	void set_id(int id);
	int get_id();

	void read_data(string nm_f);
	void read_data_type(string nm_f);	
	void save_to(string nm_file);
	string get_data_type(int idx);

	bool is_pass(vector<string> &data);

	void stat_tabel();
	map<string, int> get_stat_label();
	float get_estimate_error();
	string get_max_label();
	bool is_single_label();
	map<int, int> get_unique_attr();

	int getjmlcol();
	int getjmlrow();
    int getjmlcol_svm();
    void setjmltotalrow();
    int getjmltotalrow();


	void reset_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();
	vector<string> get_record_svm();


	void add_filter(int idx_col, int idx_opt, string value);
	void add_filter(field_filter filter);
	void ReFilter();
	vector<field_filter> get_filter();

	void clear_memory();

	void info();
	void head();

};

#endif