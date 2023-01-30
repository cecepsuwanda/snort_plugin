#include <string>
#include <vector>
#include <thread>

#include "Tquery_builder.h"
#include "Tlabel_stat.h"
#include "Tmy_dttype.h"
#include "Tconf_metrix.h"

using namespace std;

#ifndef Included_tb_dataset_H

#define Included_tb_dataset_H

class tb_dataset
{
private:
	Tquery_builder global_query_builder;

	int _id_dt;
	int _jns_dt;
	string _partition;

	int _parent_depth = 0;
	int _parent_branch = 0;
	int _parent_branch_number = 0;

	int _child_depth = 0;
	int _child_branch = 0;
	int _child_branch_number = 0;

	string _tmp_dataset_tb="";

	int _jml_row = 0;
	int _jml_col = 0;

	string _id_row = "-1";

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

public:
	tb_dataset();
	~tb_dataset();

	vector<string> get_data_header();
	vector<string> get_data_type();
	int get_idx_label();

	string get_id_row();
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
	vector<int> get_idx_svm();

	void read_header_type();

	void set_dataset(int id_dt, int jns_dt, string partition);
	void set_parent(int depth, int branch, int branch_number);
	void set_child(int depth, int branch,int branch_number);	
	void switch_parent_child();
	void set_label_idx_svm(int idx_svm,string label);
	void set_label_svm(string id_row,string label);

	posisi_cabang get_posisi_parent();

	void filter(string sql, bool is_all);
	void read_hsl_filter();
	void filter_by_idx_svm(int idx_svm);

	Tlabel_stat hit_label_stat();
	map<Tmy_dttype, Tlabel_stat> hit_col_split(string group_kolom);
	void update_attr_stat(int idx);
	void clear_tmp_dataset();	
	void child_to_tmp_dataset();
	void reset_depth_branch();
	void clear_child_parent();
	void set_child_parent();
	void delete_child(int child_depth,int child_branch, int child_branch_number);

	void dtsvm_stat(time_t id_experiment,time_t id_detail_experiment,time_t id_experiment_dt,time_t id_detail_experiment_dt);
    void dtsvm_conf_metrix(time_t id_experiment, time_t id_detail_experiment, time_t id_experiment_dt, time_t id_detail_experiment_dt,Tconf_metrix &tmp_conf_metrix);
	
	void switch_to_test();
	void train_to_test();

	tb_dataset& operator = (const tb_dataset &t)
	{
		this->global_query_builder = t.global_query_builder;

		this->_id_dt = t._id_dt;
		this->_jns_dt = t._jns_dt;
		this->_partition = t._partition;

		this->_parent_depth = t._parent_depth;
		this->_parent_branch = t._parent_branch;
		this->_parent_branch_number = t._parent_branch_number;
		
		this->_child_depth = t._child_depth;
		this->_child_branch = t._child_branch;
		this->_child_branch_number = t._child_branch_number;

		this->_tmp_dataset_tb = t._tmp_dataset_tb;

		this->_data_header = t._data_header;
		this->_data_type = t._data_type;

		this->_jml_col = t._jml_col;

		return *this;
	}

};

#endif