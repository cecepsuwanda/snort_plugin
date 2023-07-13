#include <mutex>
#include "Tread_file.h"
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"
#include "Tbelow_above.h"
#include "Tbelow_above_kategori.h"
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
  Tglobal_config global_config;

  void calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, Tmy_dttype & split_value, Tlabel_stat & stat_label);

  void handle_continuous(int idx, float & current_overall_metric, Tmy_dttype & split_value);
  void handle_non_continuous(int idx, float & current_overall_metric, Tmy_dttype & split_value);

  static Tpot_split get_pot_split(int id_dt, int jns_dt, string partition, Tposisi_cabang posisi_cabang, int idx);

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
    _map_filter = t._map_filter;


    _jml_col = t._jml_col;
    _jml_row = t._jml_row;

    _stat_label = t._stat_label;
    _idx_label = t._idx_label;


    _id_dt = t._id_dt;
    _jns_dt = t._jns_dt;
    _partition = t._partition;

    _posisi_cabang = t._posisi_cabang;
    _is_train = t._is_train;

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
    this->_map_filter = t._map_filter;

    this->_jml_col = t._jml_col;
    this->_jml_row = t._jml_row;
    this->_stat_label = t._stat_label;
    this->_idx_label = t._idx_label;

    this->_id_dt = t._id_dt;
    this->_jns_dt = t._jns_dt;
    this->_partition = t._partition;

    this->_posisi_cabang = t._posisi_cabang;
    this->_is_train = t._is_train;

    this->is_non_continuous = t.is_non_continuous;
    this->is_42 = t.is_42;
    this->_jml_total_row = t._jml_total_row;

    return *this;
  }

  void read_header_type();

  void clone_dataset();
  void reset_depth_branch();

  void stat_tabel(bool is_filter, bool is_last, bool is_stat_label);
  void search_col_split();
  int get_jml_stat(string label);

  map<string, int> get_stat_label();
  map<string, map<string, int>> get_conf_metrix();

  float get_estimate_error();
  string get_max_label();
  bool is_single_label();
 

  map<Tmy_dttype, Tlabel_stat> get_col_split(int idx);
  void clear_col_split();

  int get_jml_valid_attr();
  int get_valid_attr(int idx);

  void info();

  void add_filter(int idx_col, int idx_opt, Tmy_dttype value, bool is_filter, bool is_last);
  void add_filter(field_filter filter, bool is_filter, bool is_last);
  void ReFilter(bool is_last);

  void clear_map_col_split();

  void split_data(int split_column, Tmy_dttype split_value, Tdataframe &data_below, Tdataframe &data_above);
  Tmy_dttype get_entropy();

  string get_nm_header(int idx_col);
  int get_opt(int idx_col, int is_below);

  void calculate_overall_metric(int idx, float &current_overall_metric, Tmy_dttype &split_value);

};

#endif