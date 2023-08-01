#include <mutex>
#include "tb_dataset.h"
#include "Tlabel_stat.h"
#include "Tmap_col_split.h"
#include "tb_missing_branch.h"
#include "global.h"
#include <string>

using namespace std;

#ifndef Included_Tbase_dataframe_H

#define Included_Tbase_dataframe_H

struct field_filter
{
	int idx_col;
	int idx_opt;
	Tmy_dttype value;

	string to_query(vector<string> data_header)
	{
		vector<string> opt_arr{ "<=", ">", "=", "!="};

		string tmp_str = "(a." + data_header[idx_col] + opt_arr[idx_opt] + value.get_string() + ")";

		if ((idx_opt == 2) or (idx_opt == 3))
		{
			if (value.delimiter_exist())
			{
				vector<string> v_tmp = value.str_split(";");
				string tmp = "";
				for (size_t i = 0; i < v_tmp.size(); ++i)
				{
					tmp = tmp + "'" + v_tmp[i] + "'";
					if (i < (v_tmp.size()-1))
					{
						tmp = tmp + ",";
					}
				}
				if (idx_opt == 2)
				{
					tmp_str = "(a." + data_header[idx_col] + " in (" + tmp + "))";
				} else {
					if (idx_opt == 3)
					{
						tmp_str = "(a." + data_header[idx_col] + " not in (" + tmp + "))";
					}
				}
				//cout << tmp_str << endl;

			} else {
				tmp_str = "(a." + data_header[idx_col] + opt_arr[idx_opt] + "'" + value.get_string() + "')";
			}


		}

		return tmp_str;
	}

};

class Tbase_dataframe
{
protected:

	tb_dataset _data;
	vector<string> _data_header;
	vector<string> _data_type;
	vector<field_filter> _filter;
	map<int, map<int, vector<field_filter>>> _map_filter;
	tb_missing_branch missing_branch;

	int _jml_col = 0;
	int _jml_row = 0;
	int _jml_total_row = 0;

	int _id_dt;
	int _jns_dt;
	string _partition;

	Tposisi_cabang _posisi_cabang;

	bool _is_train;

	mutable std::mutex v_mutex;

	string filter_to_query(bool is_last);

private:

public:
	Tbase_dataframe();
	~Tbase_dataframe();

	Tbase_dataframe(const Tbase_dataframe &t)
	{
		_data = t._data;
		_data_header = t._data_header;
		_data_type = t._data_type;
		_filter = t._filter;
		_map_filter = t._map_filter;

		_jml_col = t._jml_col;
		_jml_row = t._jml_row;

		_id_dt = t._id_dt;
		_jns_dt = t._jns_dt;
		_partition = t._partition;

		_posisi_cabang = t._posisi_cabang;
		_is_train = t._is_train;

		_jml_total_row = t._jml_total_row;
	}


	Tbase_dataframe& operator = (const Tbase_dataframe &t)
	{
		this->_data = t._data;
		this->_data_header = t._data_header;
		this->_data_type = t._data_type;
		this->_filter = t._filter;
		this->_map_filter = t._map_filter;

		this->_jml_col = t._jml_col;
		this->_jml_row = t._jml_row;

		this->_id_dt = t._id_dt;
		this->_jns_dt = t._jns_dt;
		this->_partition = t._partition;

		this->_posisi_cabang = t._posisi_cabang;
		this->_is_train = t._is_train;

		this->_jml_total_row = t._jml_total_row;
		return *this;
	}

	void switch_to_test();

	void read_header_type();

	void set_dataset(int id_dt, int jns_dt, string partition);
	void set_branch(int depth, int branch, int branch_number);
	void set_parent(int depth, int branch, int branch_number);
	void switch_parent_branch();
	void set_label(string label);

	void save_to(string nm_file);
	string get_data_type(int idx);

	//bool is_pass(vector<string> &data);
	//bool is_pass();

	int getjmlcol();
	int getjmlrow();
	void setjmltotalrow();
	int getjmltotalrow();

	void reset_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();
	vector<vector<string>> get_all_record();

	void next_col();
	bool is_end_col();
	string get_col_val();
	string get_col_val(int idx_col);
	int get_idx_col();

	void add_filter(int idx_col, int idx_opt, Tmy_dttype value, bool is_filter, bool is_last);
	void add_filter(field_filter filter, bool is_filter, bool is_last);
	void ReFilter(bool is_last);
	vector<field_filter> get_filter();

	void clear_memory(int idx);

	void info();
	void head();

	void train_to_test();

	Tposisi_cabang get_posisi();

};

#endif