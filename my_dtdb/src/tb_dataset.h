#include <string>
#include <vector>
#include <thread>

#include "global.h"
#include "Tquery_builder.h"
#include "Tlabel_stat.h"
#include "Tmy_dttype.h"

using namespace std;

#ifndef Included_tb_dataset_H

#define Included_tb_dataset_H

class tb_dataset
{
private:
	Tquery_builder global_query_builder;
	Tglobal_config global_config;

	int _id_dt;
	int _jns_dt;
	string _partition;

	Tposisi_cabang _posisi_cabang;

	string _tmp_dataset_tb = "";
	string _tmp_attr_dataset_tb = "";

	int _jml_row = 0;
	int _jml_col = 0;

	int _idx_label = 0;
	size_t _idx_col = 0;

	vector<string> _data;
	vector<string> _data_header;
	vector<string> _data_type;

	void clear_tb_index();
	void clear_tb_index1();
	bool is_child_parent_exist();
	bool is_parent_exist(int idx);

	bool exist_in_detil(int idx);

	void gen_permutation(vector<string> lst_label, int depth,string tb, string group_kolom);
	void gen_permutation_rec(vector<string> lst_label, int count, int depth, int geser,string tb, string group_kolom, string str_select, string query);

public:
	tb_dataset();
	~tb_dataset();

	vector<string> get_data_header();
	vector<string> get_data_type();
	int get_idx_label();

	int get_jml_row();
	int get_jml_col();

	void reset_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

	void next_col();
	bool is_end_col();
	string get_col_val();
	string get_col_val(int idx_col);
	int get_idx_col();

	void read_header_type();

	void set_dataset(int id_dt, int jns_dt, string partition);
	void set_parent(int depth, int branch, int branch_number);
	void set_child(int depth, int branch, int branch_number);
	void switch_parent_child();
	void set_label(string label);

	Tposisi_cabang get_posisi_parent();


	void filter(string sql, bool is_all);
	void read_hsl_filter();

	Tlabel_stat hit_label_stat();
	map<string, map<string, int>> hit_conf_metrik();
	map<Tmy_dttype, Tlabel_stat> hit_col_split(string group_kolom);
	void update_attr_stat(int idx);
	void clear_tmp_dataset();
	void child_to_tmp_dataset();
	void reset_depth_branch();
	void clear_child_parent();
	void set_child_parent();
	void delete_child(int child_depth, int child_branch, int child_branch_number);

	void switch_to_test();

	void train_to_test();

	tb_dataset& operator = (const tb_dataset &t)
	{
		this->global_query_builder = t.global_query_builder;

		this->_id_dt = t._id_dt;
		this->_jns_dt = t._jns_dt;
		this->_partition = t._partition;

		this->_posisi_cabang = t._posisi_cabang;


		this->_tmp_dataset_tb = t._tmp_dataset_tb;
		this->_tmp_attr_dataset_tb = t._tmp_attr_dataset_tb;

		this->_data_header = t._data_header;
		this->_data_type = t._data_type;

		this->_jml_col = t._jml_col;

		return *this;
	}

};

#endif