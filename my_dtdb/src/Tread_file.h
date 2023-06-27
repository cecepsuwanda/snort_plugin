#include <iostream>
#include <vector>
#include <string>
#include <mutex>

#include "Tquery_builder.h"
#include "Tlabel_stat.h"
#include "Tmy_dttype.h"


using namespace std;

#ifndef Included_Tread_file_H

#define Included_Tread_file_H

class Tread_file {
private:
	
    Tquery_builder global_query_builder;

    bool _is_con;
    
    int _id_dt;
    int _jns_dt;
    string _nm_tb;
    string _partition;

	int _jml_row = 0;
	int _jml_col = 0;
	
	vector<string> _data;
	vector<string> _data_header;
	vector<string> _data_type;	
	
	size_t _idx_col = 0;
	int _idx_label = 0;

	string _head_where;
	string _default_query;

	mutable std::mutex v_mutex;
	
	void clear_data();	

public:
	Tread_file();
	~Tread_file();
	
	Tread_file& operator = (const Tread_file &t)
	{		
		this->_id_dt = t._id_dt;
		this->_jns_dt = t._jns_dt;
		this->_nm_tb = t._nm_tb;
		this->_partition = t._partition;

		this->_head_where = t._head_where;
	    this->_default_query = t._default_query;

	    this->_data_header = t._data_header;
	    this->_data_type = t._data_type;

		return *this;
	}	

	void setnm_f(string nm_tb,int id_dt, int jns_dt,string partition);
	void filter(string sql,bool is_con);
	void delete_data(string sql);    
	Tlabel_stat hit_label_stat(string nm_kolom,string sql);	
	// map<Tmy_dttype, Tlabel_stat> hit_col_split(string group_kolom ,string count_kolom,string sql);
	void update_attr_stat(int idx);
	
	int get_jml_row();
	int get_jml_col();
	
	void reset_file();
	void read_file();
	void close_file();

	vector<string> get_data_header();
	vector<string> get_data_type();
	int get_idx_label();

	bool is_eof();
	void next_record();
	vector<string> get_record();

	void next_col();
	bool is_end_col();
	string get_col_val();
	string get_col_val(int idx_col);
	int get_idx_col();



};

#endif