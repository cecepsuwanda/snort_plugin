#include <mutex>
#include "Tread_file.h"
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"
#include "Tbelow_above.h"
#include "Tbase_dataframe.h"
#include <future>
#include <thread>
#include <unordered_map>
#include <map>
#include <cmath>
#include <string>

using namespace std;

#ifndef Included_Tdataframe_H

#define Included_Tdataframe_H

struct Tpot_split
{
  int idx;
  map<Tmy_dttype, Tlabel_stat> data;
};


class Tdataframe : public Tbase_dataframe
{
private:
  
  Tlabel_stat _stat_label;
  Tmap_col_split _map_col_split;
  int _idx_label;
  bool is_non_continuous = false;
  bool is_42 = false;
  Tconfig* config;
  

  void calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, string & split_value, Tlabel_stat & stat_label);

  void handle_continuous(int idx, float & current_overall_metric, string & split_value);
  void handle_non_continuous(int idx, float & current_overall_metric, string & split_value);  

  static Tpot_split get_pot_split(int id_dt, int jns_dt, string partition, int parent_depth, int parent_branch, int parent_branch_number, int child_depth, int child_branch, int child_branch_number, int idx);



public:
  Tdataframe();
  Tdataframe(Tconfig* v_config);
  ~Tdataframe();

  Tdataframe(const Tdataframe &t)
  {
    //cout << "Copy Constructor" << endl;
    _data = t._data;
    _data_header = t._data_header;
    _data_type = t._data_type;
    _filter = t._filter;
    

    _jml_col = t._jml_col;
    _jml_row = t._jml_row;

    _stat_label = t._stat_label;
    _idx_label = t._idx_label;


    _id_dt = t._id_dt;
    _jns_dt = t._jns_dt;
    _partition = t._partition;

    _parent_depth = t._parent_depth;
    _parent_branch = t._parent_branch;
    _parent_branch_number = t._parent_branch_number;

    _child_depth = t._child_depth;
    _child_branch = t._child_branch;
    _child_branch_number = t._child_branch_number;

    is_non_continuous = t.is_non_continuous;
    is_42 = t.is_42;
    _jml_total_row = t._jml_total_row;
    config = t.config;

    _stat_label.set_config(config);
    _map_col_split.set_config(config);

  }


  Tdataframe& operator = (const Tdataframe &t)
  {
    //cout << "operator =" << endl;
    this->_data = t._data;
    this->_data_header = t._data_header;
    this->_data_type = t._data_type;
    this->_filter = t._filter;
    
    this->_jml_col = t._jml_col;
    this->_jml_row = t._jml_row;
    this->_stat_label = t._stat_label;
    this->_idx_label = t._idx_label;

    this->_id_dt = t._id_dt;
    this->_jns_dt = t._jns_dt;
    this->_partition = t._partition;

    this->_parent_depth = t._parent_depth;
    this->_parent_branch = t._parent_branch;
    this->_parent_branch_number = t._parent_branch_number;

    this->_child_depth = t._child_depth;
    this->_child_branch = t._child_branch;
    this->_child_branch_number = t._child_branch_number;

    this->is_non_continuous = t.is_non_continuous;
    this->is_42 = t.is_42;
    this->_jml_total_row = t._jml_total_row;
    this->config = t.config;

    this->_stat_label.set_config(this->config);
    this->_map_col_split.set_config(this->config);

    return *this;
  }

  void read_header_type();

  void clone_dataset();
  void reset_depth_branch();  

  void stat_tabel(bool is_filter, bool is_last, bool is_stat_label);
  void search_col_split();
  
  map<string, int> get_stat_label();
  float get_estimate_error();
  string get_max_label();
  bool is_single_label();
  
  int getjmlcol_svm();
  int getjmlrow_svm();
  vector<string> get_record_svm();
  vector<vector<string>> get_all_record_svm();

  map<Tmy_dttype, Tlabel_stat> get_col_split(int idx);
  void clear_col_split();

  // void set_config(Tconfig v_config);

  int get_jml_valid_attr();
  int get_valid_attr(int idx);

  void info();

  void add_filter(int idx_col, int idx_opt, string value,bool is_filter,bool is_last);
  void add_filter(field_filter filter,bool is_filter,bool is_last);
  void ReFilter(bool is_last);

  void clear_map_col_split();

  void split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above);
  float get_entropy();

  string get_nm_header(int idx_col);
  int get_opt(int idx_col, int is_below);
  
  void calculate_overall_metric(int idx, float &current_overall_metric, string &split_value);

  void set_config(Tconfig* v_config);
  

};

#endif