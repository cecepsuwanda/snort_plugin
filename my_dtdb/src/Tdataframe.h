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

struct Tsplit_stat
{
  Tmy_dttype split_value;
  Tlabel_stat label_stat;
};

struct Tmetric_split_value
{
  int idx = 0;
  float max_gain_ratio = -1;
  float max_gain = -1;
  int jml_below = 0;
  int jml_above = 0;
  Tmy_dttype split_value;
};


class Tdataframe : public Tbase_dataframe
{
private:

  class Tcari_gain_max
  {
  private:
    bool first_iteration;
    Tmy_dttype max_gain_ratio;
    Tmy_dttype max_gain;
    Tmy_dttype tmp_split_value;
    int jml_below;
    int jml_above;

    Tglobal_config global_config;

  public:


    Tcari_gain_max()
    {
      first_iteration = true;
      max_gain_ratio.set_value("0.0", true);
      max_gain.set_value("0.0", true);
      tmp_split_value.set_value("-1", false);
      jml_below = 0;
      jml_above = 0;
    }


    Tmetric_split_value cari_gain_max(int idx_attr,Tlabel_stat stat_below, Tlabel_stat stat_above, Tmy_dttype mid_point, Tmy_dttype entropy_before_split)
    {
      Tbelow_above tmp_ba;
      tmp_ba.add_below(stat_below);
      tmp_ba.add_above(stat_above);

      Tgain_ratio hsl = tmp_ba.kalkulasi_gain_ratio(entropy_before_split);

      bool is_pass = global_config.use_credal ? true : (hsl.gain_ratio > 0.0);

      if ((first_iteration and is_pass) or ((max_gain_ratio < hsl.gain_ratio) and is_pass))
      {
        first_iteration = false;
        tmp_split_value = mid_point;
        max_gain_ratio = hsl.gain_ratio;
        max_gain = hsl.gain;
        jml_below = stat_below.get_jml_row();
        jml_above = stat_above.get_jml_row();
      }
      
      Tmetric_split_value hsl_split;

      if (max_gain_ratio != "0.0") {
        hsl_split.idx = idx_attr;
        hsl_split.max_gain_ratio = stof(hsl.gain_ratio.get_string());
        hsl_split.max_gain = stof(hsl.gain.get_string());
        hsl_split.split_value = tmp_split_value;
        hsl_split.jml_below = jml_below;
        hsl_split.jml_above = jml_above;
      }

      return hsl_split;

    }


  };


  Tlabel_stat _stat_label;
  Tmap_col_split _map_col_split;
  int _idx_label;

  Tglobal_config global_config;

  void calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, Tmy_dttype & split_value, Tlabel_stat & stat_label);

  Tmetric_split_value handle_continuous(int idx);
  Tmetric_split_value handle_non_continuous(int idx);

  static bool cmp(Tsplit_stat a, Tsplit_stat b);
  Tmetric_split_value handle_non_continuous_1(int idx);

  static Tpot_split get_pot_split(int id_dt, int jns_dt, string partition, Tposisi_cabang posisi_cabang, int idx);

  void gen_kombinasi(map<Tmy_dttype, Tlabel_stat> v_col_pot_split, int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below, Tmetric_split_value& v_split);
  void gen_kombinasi_normal(vector<Tsplit_stat> &v_col_pot_split, int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below, Tmetric_split_value& v_split);
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

  Tmetric_split_value calculate_overall_metric(int idx);



};

#endif