#include <mutex>
#include "Tread_file.h"
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"
#include "Tbelow_above.h"
#include "Tbase_dataframe.h"
#include <thread>
#include <unordered_map>
#include <map>
#include <cmath>
#include <string>

using namespace std;

#ifndef Included_Tdataframe_H

#define Included_Tdataframe_H

struct Tmetric_split_value
{
  int idx = -1;
  float max_gain_ratio = -1;
  float max_gain = -1;
  int jml_below = 0;
  int jml_above = 0;
  Tmy_dttype split_value;
};


class Tsplit_stat
{
private:
  Tmy_dttype _split_value, _entropy_before_split;
  Tlabel_stat _stat_below, _stat_above;
public:
  Tsplit_stat();
  ~Tsplit_stat();

  void set_value(Tmy_dttype split_value, Tlabel_stat stat_below, Tlabel_stat stat_above);
  void set_entropy_before_split(Tmy_dttype entropy_before_split);
  Tmy_dttype get_split_value();
  int get_jml_below();
  int get_jml_above();
  bool is_normal();
  bool is_known();
  bool cek_valid();
  Tgain_ratio kalkulasi_gain_ratio();

  Tsplit_stat(const Tsplit_stat &t)
  {
    _split_value = t._split_value;
    _stat_below = t._stat_below;
    _stat_above = t._stat_above;
    _entropy_before_split = t._entropy_before_split;
  }


  Tsplit_stat& operator = (const Tsplit_stat &t)
  {
    this->_split_value = t._split_value;
    this->_stat_below = t._stat_below;
    this->_stat_above = t._stat_above;
    this->_entropy_before_split = t._entropy_before_split;

    return *this;
  }


  const Tsplit_stat operator + (const Tsplit_stat &rhs) const
  {
    Tsplit_stat tmp;

    Tmy_dttype separator(";", false);       

    tmp._split_value = _split_value + separator + rhs._split_value;     

    Tlabel_stat tmp_stat = _stat_below + _stat_above;

    tmp._stat_below = _stat_below + rhs._stat_below;
    tmp._stat_above = tmp_stat - tmp._stat_below;

    return tmp;
  }

};

class Tproses_split_stat
{
private:
  vector<Tsplit_stat> _vec_split_stat;
  vector<Tsplit_stat> _tmp_vec_split_stat;
  
  vector<int> _idx_max_gain_ratio;
  vector<int> _idx_rata2;
  vector<int> _idx_normal;
  vector<int> _idx_known;

  Tmy_dttype _entropy_before_split;
  double _sum_gain_po, _sum_gain_neg, _rata2, _sd;

  bool _first_iteration;
  Tmy_dttype _max_gain_ratio;
  Tmy_dttype _max_gain;
  Tmy_dttype _tmp_split_value;
  int _jml_below;
  int _jml_above;

  size_t _jml_attr;

  Tglobal_config global_config;  

  bool _is_continue;

  int _idx_attr;

public:
  Tproses_split_stat();
  ~Tproses_split_stat();

  void set_continue(bool is_continue);
  void set_entropy_before_split(Tmy_dttype entropy_before_split);
  void set_idx_attr(int idx);

  void insert_tmp_split_stat(Tmy_dttype split_value, Tlabel_stat stat_below, Tlabel_stat stat_above);
  void clear_tmp();
  void del_last_tmp();
  void insert_split_stat();

  void kalkulasi_sd();
  
  void split_by_label();

  Tmetric_split_value get_max_gain_ratio();
};


class Tdataframe : public Tbase_dataframe
{
private:

  map<int, int> _unique_attr;
  Tlabel_stat _stat_label;
  Tmap_col_split _map_col_split;
  int _idx_label;
  bool is_non_continuous = false;
  bool is_42 = false;
  //bool _search_uniqe_val_on = true;
  Tglobal_config global_config;

  void calculate_metric(int idx,map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, string & split_value, Tlabel_stat & stat_label);

  Tmetric_split_value handle_continuous(int idx);
  Tmetric_split_value handle_non_continuous(int idx);

public:
  Tdataframe();  
  ~Tdataframe();

  Tdataframe(const Tdataframe &t)
  {
    //cout << "Copy Constructor" << endl;
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


  Tdataframe& operator = (const Tdataframe &t)
  {
    //cout << "operator =" << endl;
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

  void set_shared_memory_on();
  void set_shared_memory_off();

  //void set_search_uniqe_val_off();

  void read_data_type(string nm_f);

  void stat_tabel();
  map<string, int> get_stat_label();
  float get_estimate_error();
  string get_max_label();
  bool is_single_label();
  map<int, int> get_unique_attr();

  int getjmlcol_svm();
  int getjmlrow_svm();
  vector<string> get_record_svm();  
  vector<vector<string>> get_all_record_svm();

  map<Tmy_dttype, Tlabel_stat>* get_col_split(int idx);
  void clear_col_split();
  int get_jml_stat(string label);


  // void set_config(Tconfig v_config);

  int get_jml_valid_attr();
  int get_valid_attr(int idx);

  void info();

  void add_filter(int idx_col, int idx_opt, Tmy_dttype value);
  void add_filter(field_filter filter);
  void ReFilter();

  void clear_map_col_split();

  void split_data(int split_column, Tmy_dttype split_value, Tdataframe &data_below, Tdataframe &data_above);
  Tmy_dttype get_entropy();

  string get_nm_header(int idx_col);
  int get_opt(int idx_col, int is_below);
  void get_col_pot_split(int idx);
  Tmetric_split_value calculate_overall_metric(int idx);

};

#endif