
#include "Tdataframe.h"
#include <iterator>


Tdataframe::Tdataframe()
{
  _idx_label = -1;
}

Tdataframe::~Tdataframe()
{
  _stat_label.clear();
  _data.set_child(_posisi_cabang.child_depth, _posisi_cabang.child_branch, _posisi_cabang.child_branch_number);
}


void Tdataframe::read_header_type()
{
  _data.read_header_type();
  _data_header = _data.get_data_header();
  _data_type = _data.get_data_type();
  _idx_label = _data.get_idx_label();

  _jml_col = _idx_label + 1;

}


Tpot_split Tdataframe::get_pot_split(int id_dt, int jns_dt, string partition, Tposisi_cabang posisi_cabang, int idx)
{
  Tpot_split hsl;
  tb_dataset data;
  data.set_dataset(id_dt, jns_dt, partition);
  data.read_header_type();
  data.set_parent(posisi_cabang.parent_depth, posisi_cabang.parent_branch, posisi_cabang.parent_branch_number);
  data.set_child(posisi_cabang.child_depth, posisi_cabang.child_branch, posisi_cabang.child_branch_number);
  vector<string> data_header = data.get_data_header();
  hsl.data = data.hit_col_split(data_header[idx]);
  hsl.idx = idx;
  data.close_file();
  return hsl;
}



void Tdataframe::clone_dataset()
{
  _data.child_to_tmp_dataset();
}

void Tdataframe::reset_depth_branch()
{
  _data.reset_depth_branch();
}


void Tdataframe::stat_tabel(bool is_filter, bool is_last, bool is_stat_label)
{
  if (is_filter) {
    string tmp_sql = filter_to_query(is_last);
    _data.filter(tmp_sql, !is_last);
  }

  _jml_row = _data.get_jml_row();


  if (is_stat_label) {
    _stat_label.clear();
    _stat_label = _data.hit_label_stat();
  }
}

int Tdataframe::get_jml_stat(string label)
{
  return _stat_label.get_jml_stat(label);
}

void Tdataframe::search_col_split()
{
  vector<future<Tpot_split>> async_worker;


  for (size_t i = 0; i < (_data_header.size() - 1); ++i)
  {

    bool pass = true;

    if (global_config.unique_rule)
    {
      if (_data_type[i] == "continuous.")
      {
        auto itr = _map_filter.find(i);
        pass = (itr == _map_filter.end());
      }
    }


    if (pass)
    {
      async_worker.push_back(async(std::launch::async, &Tdataframe::get_pot_split, global_config.id_dt_train, global_config.jns_dt_train, global_config.partition_train, _posisi_cabang, i));
    } else {
      _data.clear_attr(i);
    }

  }

  if (async_worker.size() > 0)
  {

    Tpot_split hsl;
    for (future<Tpot_split> & th : async_worker)
    {
      hsl = th.get();
      _data.update_attr_stat(hsl.idx);
    }
    async_worker.clear();
    async_worker.shrink_to_fit();


  }

  _map_col_split.cek_valid_attr(_jml_row, _jml_total_row);
}



map<string, int> Tdataframe::get_stat_label()
{
  return _stat_label.get_map();
}

map<string, map<string, int>> Tdataframe::get_conf_metrix()
{
  return _data.hit_conf_metrik();
}

float Tdataframe::get_estimate_error()
{
  return _stat_label.get_estimate_error();
}

string Tdataframe::get_max_label()
{
  return _stat_label.get_max_label();
}

bool Tdataframe::is_single_label()
{
  return _stat_label.is_single_label();
}



map<Tmy_dttype, Tlabel_stat> Tdataframe::get_col_split(int idx)
{
  return _map_col_split.get_pot_split(idx);
}


void Tdataframe::clear_col_split()
{
  _map_col_split.clear();
}


int Tdataframe::get_jml_valid_attr()
{
  return _map_col_split.get_jml_valid_attr();
}

int Tdataframe::get_valid_attr(int idx)
{
  return _map_col_split.get_valid_attr(idx);
}


void Tdataframe::add_filter(int idx_col, int idx_opt, Tmy_dttype value, bool is_filter, bool is_last)
{

  Tbase_dataframe::add_filter(idx_col, idx_opt, value, is_filter, is_last);

  if (is_filter) {
    stat_tabel(false, is_last, true);
    if (_is_train) {
      missing_branch.add_opt_label(_posisi_cabang, idx_opt, _stat_label.get_max_label(), _stat_label.get_jml_stat("known"), _stat_label.get_jml_stat("normal"));
    }

  }

}

void Tdataframe::ReFilter(bool is_last)
{
  Tbase_dataframe::ReFilter(is_last);

  stat_tabel(false, is_last, true);
}

void Tdataframe::clear_map_col_split()
{
  _map_col_split.clear();
}

void Tdataframe::add_filter(field_filter filter, bool is_filter, bool is_last)
{
  Tbase_dataframe::add_filter(filter, is_filter, is_last);

  if (is_filter) {
    stat_tabel(false, is_last, true);
    if (_is_train) {
      missing_branch.add_opt_label(_posisi_cabang, filter.idx_opt, _stat_label.get_max_label(), _stat_label.get_jml_stat("known"), _stat_label.get_jml_stat("normal"));
    }

  }

}


void Tdataframe::info()
{
  cout << " Info" << endl;
  //cout << " Nama File   : " << _nm_file << endl;
  cout << " Jumlah Data : " << _jml_row << endl;

  cout << _stat_label << endl ;
}

void Tdataframe::split_data(int split_column, Tmy_dttype split_value, Tdataframe &data_below, Tdataframe &data_above)
{

  if (split_value != "-1") {
    missing_branch.add_split(_posisi_cabang, split_column, split_value);

    if (_posisi_cabang.get_id_branch() == "000000")
    {
      missing_branch.add_opt_label(_posisi_cabang, -1, _stat_label.get_max_label(), _stat_label.get_jml_stat("known"), _stat_label.get_jml_stat("normal"));
    }

    if (_data_type[split_column] == "continuous.")
    {
      data_below.add_filter(split_column, 0, split_value, true, true);
      data_above.add_filter(split_column, 1, split_value, true, true);

    } else {
      data_below.add_filter(split_column, 2, split_value, true, true);
      data_above.add_filter(split_column, 3, split_value, true, true);

    }
  }

}

Tmetric_split_value Tdataframe::handle_continuous(int idx)
{

  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);
  Tmetric_split_value hsl;

  if (_col_pot_split.size() > 0)
  {
    Tmy_dttype entropy_before_split;

    if (!global_config.use_credal) {
      entropy_before_split = _stat_label.get_entropy();
    } else {
      entropy_before_split = _stat_label.get_credal_entropy();
    }

    Tlabel_stat stat_label_below, stat_label_above;

    Tproses_split_stat proses_split_stat;
    proses_split_stat.set_entropy_before_split(entropy_before_split);
    proses_split_stat.set_continue(true);
    proses_split_stat.set_idx_attr(idx);

    auto itr_next = _col_pot_split.begin();
    itr_next++;

    auto itr = _col_pot_split.begin();
    while ((itr != _col_pot_split.end()))
    {

      stat_label_below = stat_label_below + (*itr).second;
      stat_label_above = _stat_label - stat_label_below;

      if (((itr != itr_next) and (itr_next != _col_pot_split.end()) ))
      {

        Tmy_dttype tmp1 = (*itr).first;
        Tmy_dttype tmp2 = (*itr_next).first;
        Tmy_dttype tmp = (tmp1 + tmp2) / 2.0;
        Tmy_dttype mid_point = tmp;


        //if (tmp1 < (tmp2 - 1e-5)) {
        proses_split_stat.insert_tmp_split_stat(mid_point, stat_label_below, stat_label_above);
        proses_split_stat.insert_split_stat();
        proses_split_stat.clear_tmp();
        //}


      }

      itr_next++;
      itr++;
    }

    _col_pot_split.clear();


    Tmetric_split_value tmp_hsl;

    tmp_hsl = proses_split_stat.get_max_gain_ratio();

    hsl.idx = idx;
    hsl.max_gain_ratio = tmp_hsl.max_gain_ratio;
    hsl.max_gain = tmp_hsl.max_gain;
    hsl.split_info = tmp_hsl.split_info;
    hsl.split_value = tmp_hsl.split_value;
    hsl.jml_below = tmp_hsl.jml_below;
    hsl.jml_above = tmp_hsl.jml_above;

  }

  _col_pot_split.clear();


  return hsl;

}


Tmetric_split_value Tdataframe::handle_non_continuous(int idx)
{
  Tmy_dttype entropy_before_split;
  Tmetric_split_value hsl_split;

  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  bool is_pass = true;

  if (_col_pot_split.size() == 2)
  {
    auto itr = _map_filter.find(idx);
    is_pass = (itr == _map_filter.end());
  }

  if (is_pass)
  {
    if (!global_config.use_credal) {
      entropy_before_split = _stat_label.get_entropy();
    } else {
      entropy_before_split = _stat_label.get_credal_entropy();
    }

    Tproses_split_stat proses_split_stat;
    proses_split_stat.set_entropy_before_split(entropy_before_split);
    proses_split_stat.set_continue(false);
    proses_split_stat.set_idx_attr(idx);

    auto itr = _col_pot_split.begin();
    while ((itr != _col_pot_split.end()))
    {
      Tmy_dttype mid_point = (*itr).first;

      Tlabel_stat stat_below = (*itr).second;
      Tlabel_stat stat_above = _stat_label - stat_below;

      proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
      proses_split_stat.insert_split_stat();
      proses_split_stat.clear_tmp();

      itr++;
    }

    _col_pot_split.clear();

    Tmetric_split_value tmp_hsl1;

    //tmp_hsl1 = proses_split_stat.get_gain_ratio_kategori();

    // cout << " attr idx sebelum " << idx << endl;
    // proses_split_stat.cetak_block();

    if (global_config.buat_kombinasi)
    {
      proses_split_stat.merge_single_label();

      bool is_lanjut = true;

      while ( (proses_split_stat.get_block_size() > 2) and is_lanjut)
      {
        is_lanjut = proses_split_stat.merge_block();
      }
    }

    // cout << " attr idx sesudah " << idx << endl;
    // proses_split_stat.cetak_block();

    Tmetric_split_value tmp_hsl;

    tmp_hsl = proses_split_stat.get_max_gain_ratio_1();

    if (!global_config.one_agains_many_only)
    {
      tmp_hsl1 = proses_split_stat.get_gain_ratio_kategori();
    }
    //cout << " [" << idx << "," << hsl.gain.get_string() << "," << tmp_hsl.max_gain << "," << tmp_hsl.split_value.get_string() << "] ";

    hsl_split.idx = idx;
    hsl_split.max_gain_ratio =  global_config.one_agains_many_only ? tmp_hsl.max_gain_ratio : tmp_hsl1.max_gain_ratio;
    hsl_split.max_gain = global_config.one_agains_many_only ? tmp_hsl.max_gain : tmp_hsl1.max_gain;
    hsl_split.split_info = global_config.one_agains_many_only ? tmp_hsl.split_info : tmp_hsl1.split_info;
    hsl_split.split_value = tmp_hsl.split_value;
    hsl_split.jml_below = tmp_hsl.jml_below;
    hsl_split.jml_above = tmp_hsl.jml_above;
  }

  return hsl_split;
}

Tmetric_split_value Tdataframe::calculate_overall_metric(int idx)
{
  std::lock_guard<std::mutex> lock(v_mutex);
  Tmetric_split_value hsl;
  if (_data_type[idx] == "continuous.") {
    hsl = handle_continuous(idx);
  } else {
    hsl = handle_non_continuous(idx);
  }

  return hsl;

}

string Tdataframe::get_nm_header(int idx_col)
{
  return _data_header[idx_col];
}

int Tdataframe::get_opt(int idx_col, int is_below)
{
  if (_data_type[idx_col] == "continuous.")
  {
    return (is_below == 1) ? 0 : 1;

  } else {
    return (is_below == 1) ? 2 : 3;
  }
}


Tmy_dttype Tdataframe::get_entropy()
{
  return _stat_label.get_entropy();
}



Tsplit_stat::Tsplit_stat()
{
  _entropy_before_split.set_value("0.0", true);

}


Tsplit_stat::~Tsplit_stat()
{

}

void Tsplit_stat::set_value(Tmy_dttype split_value, Tlabel_stat stat_below, Tlabel_stat stat_above)
{
  _split_value = split_value;
  _stat_below = stat_below;
  _stat_above = stat_above;
}

void Tsplit_stat::set_entropy_before_split(Tmy_dttype entropy_before_split)
{
  _entropy_before_split = entropy_before_split;
}

bool Tsplit_stat::cek_valid()
{
  Tbelow_above tmp_ba;
  tmp_ba.add_below(_stat_below);
  tmp_ba.add_above(_stat_above);

  return tmp_ba.cek_valid();
}

Tgain_ratio Tsplit_stat::kalkulasi_gain_ratio()
{
  Tbelow_above tmp_ba;
  tmp_ba.add_below(_stat_below);
  tmp_ba.add_above(_stat_above);

  Tgain_ratio hsl = tmp_ba.kalkulasi_gain_ratio(_entropy_before_split);

  return hsl;
}

int Tsplit_stat::get_jml_below()
{
  return _stat_below.get_jml_row();
}

int Tsplit_stat::get_jml_above()
{
  return _stat_above.get_jml_row();
}

Tmy_dttype Tsplit_stat::get_split_value()
{
  return _split_value;
}

string Tsplit_stat::get_max_label_below()
{
  return _stat_below.get_max_label();
}

bool Tsplit_stat::is_below_single_label()
{
  return _stat_below.is_single_label();
}

string Tsplit_stat::get_max_label_above()
{
  return _stat_above.get_max_label();
}

bool Tsplit_stat::is_above_single_label()
{
  return _stat_above.is_single_label();
}

Tlabel_stat Tsplit_stat::get_stat_below()
{
  return _stat_below;
}

Tproses_split_stat::Tproses_split_stat()
{
  _idx_attr = -1;

  _entropy_before_split.set_value("0.0", true);


  _first_iteration = true;
  _max_gain_ratio.set_value("0.0", true);
  _max_gain.set_value("0.0", true);
  _max_split_info = 0.0;

  _is_continue = false;
  _tmp_split_value.set_value("-1", _is_continue);

  _jml_below = 0.0;
  _jml_above = 0.0;

  _jml_attr = 0;

  _vec_split_stat.clear();
  _tmp_vec_split_stat.clear();
  _label.clear();

}

Tproses_split_stat::~Tproses_split_stat()
{
  _vec_split_stat.clear();
  _tmp_vec_split_stat.clear();
  _label.clear();

}

void Tproses_split_stat::insert_tmp_split_stat(Tmy_dttype split_value, Tlabel_stat stat_below, Tlabel_stat stat_above)
{
  Tsplit_stat split_stat;
  split_stat.set_entropy_before_split(_entropy_before_split);
  split_stat.set_value(split_value, stat_below, stat_above);

  _tmp_vec_split_stat.push_back(split_stat);
}

void Tproses_split_stat::insert_split_stat()
{
  if (_tmp_vec_split_stat.size() > 0)
  {
    Tsplit_stat tmp_split_stat;

    tmp_split_stat = _tmp_vec_split_stat[0];

    for (size_t i = 1; i < _tmp_vec_split_stat.size(); ++i)
    {
      tmp_split_stat = tmp_split_stat + _tmp_vec_split_stat[i];
    }

    _vec_split_stat.push_back(tmp_split_stat);

    if (!_is_continue)
    {
      auto itr = _label.find(tmp_split_stat.get_max_label_below());
      if (itr == _label.end())
      {
        vector<int> tmp_idx;
        tmp_idx.push_back(_vec_split_stat.size() - 1);
        _label.insert(pair<string, vector<int>>(tmp_split_stat.get_max_label_below(), tmp_idx));
      } else {
        _label[tmp_split_stat.get_max_label_below()].push_back(_vec_split_stat.size() - 1);
      }
    }

  }

}

void Tproses_split_stat::clear_tmp()
{
  _tmp_vec_split_stat.clear();
  _tmp_vec_split_stat.shrink_to_fit();
}

void Tproses_split_stat::del_last_tmp()
{
  auto last = _tmp_vec_split_stat.end() - 1;
  _tmp_vec_split_stat.erase(last);
  _tmp_vec_split_stat.shrink_to_fit();
}


Tmetric_split_value Tproses_split_stat::get_gain_ratio_kategori(size_t idx1, size_t idx2)
{
  Tbelow_above_kategori ba;

  Tsplit_stat tmp_split_stat = _vec_split_stat[idx1] + _vec_split_stat[idx2];
  ba.add_stat(tmp_split_stat.get_stat_below());

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    if ((i != idx1) and (i != idx2))
    {
      Tsplit_stat tmp_split_stat = _vec_split_stat[i];
      ba.add_stat(tmp_split_stat.get_stat_below());
    }

  }

  Tgain_ratio_kategori hsl = ba.kalkulasi_gain_ratio(_entropy_before_split);

  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(hsl.gain_ratio.get_string());
  hsl_split.max_gain = stof(hsl.gain.get_string());
  hsl_split.split_info = hsl.split_info;

  return hsl_split;
}

size_t Tproses_split_stat::get_block_size()
{
  return _vec_split_stat.size();
}

void Tproses_split_stat::cetak_block()
{
  for (size_t j = 0; j < _vec_split_stat.size(); ++j)
  {
    cout << "          " << j << " " << _vec_split_stat[j].get_split_value().get_string() << endl;
  }
}

bool Tproses_split_stat::merge_block()
{
  bool lanjut = false;

  Tmy_dttype max_best_gain_ratio("0.0", true);
  Tmy_dttype max_best_gain("0.0", true);
  float max_split_info = 0.0;
  int max_idx1 = -1, max_idx2 = -1;

  int ReasonableSubsets = 0;
  size_t Barred = 0;


  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    if (_vec_split_stat[i].get_jml_below() >= 2 ) //global_config.threshold
    {
      ReasonableSubsets++;
    }

    if (_vec_split_stat[i].get_jml_below() > _vec_split_stat[Barred].get_jml_below())
    {
      Barred = i;
    }

  }

  if (ReasonableSubsets >= 3)
  {
    Barred = -1;
  }


  for (size_t i = 0; i < _vec_split_stat.size() - 1; ++i)
  {
    for (size_t j = i + 1; j < _vec_split_stat.size(); ++j)
    {

      if ((i != Barred) or (j != Barred))
      {
        Tmetric_split_value tmp_hsl = get_gain_ratio_kategori(i, j);

        Tmy_dttype tmp_best_gain_ratio(to_string(tmp_hsl.max_gain_ratio), true);
        Tmy_dttype tmp_best_gain(to_string(tmp_hsl.max_gain), true);
        float tmp_split_info = tmp_hsl.split_info;

        bool is_pass = (tmp_best_gain >= _min_gain) and ((max_best_gain < _min_gain) or (tmp_best_gain_ratio >= max_best_gain_ratio));

        if (is_pass)
        {
          max_best_gain_ratio = tmp_best_gain_ratio;
          max_best_gain = tmp_best_gain;
          max_split_info = tmp_split_info;
          max_idx1 = i;
          max_idx2 = j;
        }
      }


    }
  }


  if ((max_idx1 == -1) or (((max_best_gain_ratio < (_best_gain_ratio + 1e-5)) or (max_best_gain_ratio == _best_gain_ratio)) and (max_best_gain < _best_gain)))
  {
    lanjut = false;
  } else {
    lanjut = true;
  }

  if ((lanjut) and (max_idx1 != max_idx2) and (max_idx1 != -1) and (max_idx2 != -1))
  {
    _best_gain_ratio = max_best_gain_ratio;
    _best_gain = max_best_gain;
    _best_split_info = max_split_info;


    _vec_split_stat.push_back(_vec_split_stat[max_idx1] + _vec_split_stat[max_idx2]);
    if (max_idx1 < max_idx2)
    {
      _vec_split_stat.erase(_vec_split_stat.begin() + max_idx2);
      _vec_split_stat.erase(_vec_split_stat.begin() + max_idx1);
    } else {
      _vec_split_stat.erase(_vec_split_stat.begin() + max_idx1);
      _vec_split_stat.erase(_vec_split_stat.begin() + max_idx2);
    }
  }

  return lanjut;
}

void Tproses_split_stat::merge_single_label()
{
  _jml_attr = _vec_split_stat.size();

  if ((_label.size() > 0))
  {

    for (auto itr = _label.begin(); itr != _label.end(); ++itr)
    {
      vector<int> tmp_v = itr->second;

      Tsplit_stat tmp_split_stat;
      int jml = 0;

      for (size_t i = 0; i < tmp_v.size(); ++i)
      {
        if (_vec_split_stat[tmp_v[i]].is_below_single_label())
        {
          if (jml == 0) {
            tmp_split_stat = _vec_split_stat[tmp_v[i]];
          } else {
            tmp_split_stat = tmp_split_stat + _vec_split_stat[tmp_v[i]];
          }
          jml++;
        }
      }

      if (jml > 0)
      {
        _vec_split_stat.push_back(tmp_split_stat);
      }
    }


    for (auto itr = _label.begin(); itr != _label.end(); ++itr)
    {
      vector<int> tmp_v = itr->second;

      for (size_t i = 0; i < tmp_v.size(); ++i)
      {
        if (!_vec_split_stat[tmp_v[i]].is_below_single_label())
        {
          _vec_split_stat.push_back(_vec_split_stat[tmp_v[i]]);
        }
      }

    }

    _vec_split_stat.erase(_vec_split_stat.begin(), _vec_split_stat.begin() + _jml_attr);


    Tmetric_split_value hsl = get_gain_ratio_kategori();


    _best_gain_ratio.set_value(to_string(hsl.max_gain_ratio), true);
    _best_gain.set_value(to_string(hsl.max_gain), true);;
    _best_split_info = hsl.split_info;
    _min_gain = _best_gain / 2.0;

    _label.clear();

  }

}

Tmetric_split_value Tproses_split_stat::get_max_gain_ratio()
{

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    bool is_pass = true;

    //is_pass = global_config.use_credal ? true : (hsl.gain > 0.0); //global_config.use_credal ? true :

    if (global_config.use_gain_ratio)
    {
      is_pass = _first_iteration  or ( _max_gain_ratio < hsl.gain_ratio);
    } else {
      is_pass = _first_iteration  or ( _max_gain < hsl.gain);
    }

    if (is_pass)
    {
      _first_iteration = false;
      _tmp_split_value = tmp_split_stat.get_split_value();
      _max_gain_ratio = hsl.gain_ratio;
      _max_gain = hsl.gain;
      _max_split_info = hsl.split_info;
      _jml_below = tmp_split_stat.get_jml_below();
      _jml_above = tmp_split_stat.get_jml_above();
    }

  }

  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(_max_gain_ratio.get_string());
  hsl_split.max_gain = stof(_max_gain.get_string());
  hsl_split.split_info = _max_split_info;
  hsl_split.split_value = _tmp_split_value;
  hsl_split.jml_below = _jml_below;
  hsl_split.jml_above = _jml_above;

  return hsl_split;

}

Tmetric_split_value Tproses_split_stat::get_max_gain_ratio_1()
{

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    bool is_pass = true;

    //is_pass = global_config.use_credal ? true : (hsl.gain_ratio > 0.0); //global_config.use_credal ? true :

    if (global_config.use_gain_ratio_1)
    {
      is_pass = _first_iteration  or ( _max_gain_ratio < hsl.gain_ratio);
    } else {
      is_pass = _first_iteration  or ( _max_gain < hsl.gain);
    }

    if (is_pass)
    {
      _first_iteration = false;
      _tmp_split_value = tmp_split_stat.get_split_value();
      _max_gain_ratio = hsl.gain_ratio;
      _max_gain = hsl.gain;
      _max_split_info = hsl.split_info;
      _jml_below = tmp_split_stat.get_jml_below();
      _jml_above = tmp_split_stat.get_jml_above();
    }

  }

  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(_max_gain_ratio.get_string());
  hsl_split.max_gain = stof(_max_gain.get_string());
  hsl_split.split_info = _max_split_info;
  hsl_split.split_value = _tmp_split_value;
  hsl_split.jml_below = _jml_below;
  hsl_split.jml_above = _jml_above;

  return hsl_split;

}

Tmetric_split_value Tproses_split_stat::get_gain_ratio_kategori()
{
  Tbelow_above_kategori ba;

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    ba.add_stat(tmp_split_stat.get_stat_below());
  }

  Tgain_ratio_kategori hsl = ba.kalkulasi_gain_ratio(_entropy_before_split);

  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(hsl.gain_ratio.get_string());
  hsl_split.max_gain = stof(hsl.gain.get_string());
  hsl_split.split_info = hsl.split_info;

  return hsl_split;

}

void Tproses_split_stat::set_idx_attr(int idx)
{
  _idx_attr = idx;
}

void Tproses_split_stat::set_continue(bool is_continue)
{
  _is_continue = is_continue;
  _tmp_split_value.set_value("-1", _is_continue);
}

void Tproses_split_stat::set_entropy_before_split(Tmy_dttype entropy_before_split)
{
  _entropy_before_split = entropy_before_split;
}

