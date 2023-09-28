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

struct Tmetric_split_value
{
  int idx = -1;
  float max_gain_ratio = -1;
  float max_gain = -1;
  int jml_below = 0;
  int jml_above = 0;
  Tmy_dttype split_value;
};

// struct Tsplit_stat
// {
//   Tmy_dttype split_value;
//   Tlabel_stat label_stat;
// };

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

  void gen_split_attr_rec(int counter, int depth, int geser);

public:
  Tproses_split_stat();
  ~Tproses_split_stat();

  void set_entropy_before_split(Tmy_dttype entropy_before_split);

  void insert_tmp_split_stat(Tmy_dttype split_value, Tlabel_stat stat_below, Tlabel_stat stat_above);
  void clear_tmp();
  void del_last_tmp();
  void insert_split_stat();

  void kalkulasi_sd();

  void gen_split_attr();

  Tmetric_split_value get_max_gain_ratio();
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
    int idx_attr;
    int jml_below;
    int jml_above;

    bool is_multiway;
    Tmy_dttype multiway_gain_ratio;

    Tglobal_config global_config;

  public:
    Tcari_gain_max();
    void set_idx_attr(int idx);
    void set_is_multiway(bool stat);
    void set_multiway_gain_ratio(Tmy_dttype gain_ratio);
    void cari_gain_max(Tlabel_stat stat_below, Tlabel_stat stat_above, Tmy_dttype mid_point, Tmy_dttype entropy_before_split);
    Tmetric_split_value get_gain_max();
  };


  // class Thanlde_split_map
  // {
  // private:
  //   vector<Tsplit_stat> _vec_split_stat;
  //   Tmy_dttype _entropy_before_split;
  //   Tlabel_stat _stat_label;
  //   float _rata2;
  //   Tcari_gain_max _cari_gain_max;

  //   Tglobal_config global_config;

  //   static bool cmp(Tsplit_stat a, Tsplit_stat b);
  // public:
  //   Thanlde_split_map();
  //   ~Thanlde_split_map();

  //   void set_value(Tmy_dttype entropy_before_split, Tlabel_stat stat_label);
  //   void konversi_map_vec(map<Tmy_dttype, Tlabel_stat> &map_split_stat);
  //   void gen_kombinasi_normal(int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below);
  //   Tmetric_split_value cari_gain(int idx);
  // };


  class Tstat_gain_split_holder
  {
  private:
    Tmy_dttype mid_point;
    Tlabel_stat stat_below, stat_above;
    int jml;
  public:
    Tstat_gain_split_holder();
    ~Tstat_gain_split_holder();
    void set_value(Tmy_dttype v_mid_point, Tlabel_stat v_stat_below, Tlabel_stat stat);
    Tmy_dttype get_mid_point();
    Tlabel_stat get_stat_below();
    Tlabel_stat get_stat_above();
    bool is_empty();
    int get_jml();

    Tgain_ratio kalkulasi_gain_ratio(Tmy_dttype entropy_before_split);
  };


  




  Tlabel_stat _stat_label;
  Tmap_col_split _map_col_split;
  int _idx_label;

  Tglobal_config global_config;

  void calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, Tmy_dttype & split_value, Tlabel_stat & stat_label);

  Tmetric_split_value handle_continuous(int idx);
  Tmetric_split_value handle_non_continuous(int idx);


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