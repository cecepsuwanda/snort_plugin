
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
    auto itr = _map_filter.find(i);
    if (itr == _map_filter.end()) {
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


void Tdataframe::calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, Tmy_dttype & split_value, Tlabel_stat & stat_label)
{
  Tmy_dttype entropy_before_split;

  if (!global_config.use_credal) {
    entropy_before_split = stat_label.get_entropy();
  } else {
    entropy_before_split = stat_label.get_credal_entropy();
  }


  struct Tcontainer
  {
    Tmy_dttype mid_point;
    float entropy_after_split;
    float split_info;
    float gain;
  };

  vector<Tcontainer> v_container;
  int jml_data = 0;
  int sum_po = 0, sum_ne = 0;

  Tmy_dttype mid_point("0.0", true);

  Tlabel_stat _stat_label_below;

  auto itr_next = _col_pot_split->begin();
  itr_next++;



  auto itr = _col_pot_split->begin();
  while ((itr != _col_pot_split->end()))
  {

    _stat_label_below = _stat_label_below + (*itr).second;

    bool is_pass = true;

    // if (_stat_label_below.is_single_label() and (*itr_next).second.is_single_label())
    // {
    //   string label_below = _stat_label_below.get_max_label();
    //   string label_next = (*itr_next).second.get_max_label();

    //   is_pass = label_below != label_next;

    // }

    if (is_pass)
    {

      if (((itr != itr_next) and (itr_next != _col_pot_split->end()) ))
      {

        Tmy_dttype tmp1 = (*itr).first;
        Tmy_dttype tmp2 = (*itr_next).first;
        Tmy_dttype tmp = (tmp1 + tmp2) / 2.0;
        mid_point = tmp;

        Tbelow_above ba;

        ba.add_below(_stat_label_below);
        Tlabel_stat tmp_stat = stat_label - _stat_label_below;

        ba.add_above(tmp_stat);

        if (ba.cek_valid()) {
          Tcontainer tmp;
          tmp.entropy_after_split = stof(ba.get_overall_metric().get_string());
          tmp.split_info = ba.get_split_info();
          tmp.gain = stof(entropy_before_split.get_string()) - tmp.entropy_after_split;

          if (tmp.gain > 0)
          {
            sum_po += tmp.gain;
          } else {
            if (tmp.gain < 0)
            {
              sum_ne += tmp.gain;
            }
          }

          tmp.mid_point = mid_point;

          v_container.push_back(tmp);

          jml_data++;
        }
      }

    }

    itr_next++;
    itr++;
  }

  float max_entropi = 0.0;

  float rata_gain = 0.0;
  if (v_container.size() > 0) {
    rata_gain = (sum_po - abs(sum_ne)) / jml_data;
  }
  Tmy_dttype tmp_split_value("0.0", true);
  bool first_iteration = true;

  for (size_t i = 0; i < v_container.size(); ++i)
  {
    if (rata_gain < v_container[i].gain)
    {
      float gain_ratio = 0.0;

      if (abs(v_container[i].split_info) > 0) {
        gain_ratio = v_container[i].gain / v_container[i].split_info;
      }

      bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

      if ((first_iteration and is_pass) or ((max_entropi < gain_ratio) and is_pass))
      {
        first_iteration = false;
        tmp_split_value = v_container[i].mid_point;
        max_entropi = gain_ratio;

      }
    }

  }

  v_container.clear();
  v_container.shrink_to_fit();

  current_overall_metric = max_entropi;
  split_value = tmp_split_value;

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

        proses_split_stat.insert_tmp_split_stat(mid_point, stat_label_below, stat_label_above);
        proses_split_stat.insert_split_stat();
        proses_split_stat.clear_tmp();
      }

      itr_next++;
      itr++;
    }

    _col_pot_split.clear();

    //proses_split_stat.kalkulasi_sd();

    Tmetric_split_value tmp_hsl;

    tmp_hsl = proses_split_stat.get_max_gain_ratio();

    hsl.idx = idx;
    hsl.max_gain_ratio = tmp_hsl.max_gain_ratio;
    hsl.max_gain = tmp_hsl.max_gain;
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

  if (!global_config.use_credal) {
    entropy_before_split = _stat_label.get_entropy();
  } else {
    entropy_before_split = _stat_label.get_credal_entropy();
  }

  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  Tproses_split_stat proses_split_stat;
  proses_split_stat.set_entropy_before_split(entropy_before_split);
  proses_split_stat.set_continue(false);
  proses_split_stat.set_idx_attr(idx);


  Tbelow_above_kategori ba;

  auto itr = _col_pot_split.begin();
  while ((itr != _col_pot_split.end()))
  {
    Tmy_dttype mid_point = (*itr).first;

    Tlabel_stat stat_below = (*itr).second;
    Tlabel_stat stat_above = _stat_label - stat_below;

    ba.add_stat(stat_below);

    if (idx == 2)
    {
      if ((mid_point == "private") or (mid_point == "ecri") or (mid_point == "ecr_i") or (mid_point == "http"))
      {
        proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
      }
    }

    if (idx == 3)
    {
      if (mid_point == "SF")
      {
        proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
      }
    }

    if (idx == 6)
    {
      if (mid_point == "0")
      {
        proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
      }
    }

    if (idx == 1)
    {
      if ((mid_point == "tcp")  or (mid_point == "udp")) //or (mid_point == "icmp")
      {
        proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
      }
    }

    //else {
    // proses_split_stat.insert_tmp_split_stat(mid_point, stat_below, stat_above);
    // proses_split_stat.insert_split_stat();
    // proses_split_stat.clear_tmp();
    //}

    itr++;
  }

  // if (idx == 1)
  // {
  proses_split_stat.insert_split_stat();
  proses_split_stat.clear_tmp();
  // }

  _col_pot_split.clear();

  //proses_split_stat.split_by_label();
  //proses_split_stat.gen_split_attr();

  //proses_split_stat.kalkulasi_sd();

  Tmetric_split_value tmp_hsl;

  tmp_hsl = proses_split_stat.get_max_gain_ratio();

  Tgain_ratio_kategori hsl = ba.kalkulasi_gain_ratio(entropy_before_split);

  //cout << " [" << idx << "," << hsl.gain.get_string() << "," << tmp_hsl.max_gain << "," << tmp_hsl.split_value.get_string() << "] ";

  hsl_split.idx = idx;
  hsl_split.max_gain_ratio = tmp_hsl.max_gain_ratio;//stof(hsl.gain_ratio.get_string());
  hsl_split.max_gain = tmp_hsl.max_gain;//stof(hsl.gain.get_string());
  hsl_split.split_value = tmp_hsl.split_value;
  hsl_split.jml_below = tmp_hsl.jml_below;
  hsl_split.jml_above = tmp_hsl.jml_above;

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

bool Tsplit_stat::is_normal()
{
  return (_stat_below.get_max_label() == "normal") and _stat_below.is_single_label();
}

bool Tsplit_stat::is_known()
{
  return (_stat_below.get_max_label() == "known") and _stat_below.is_single_label();
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



Tproses_split_stat::Tproses_split_stat()
{
  _idx_attr = -1;

  _entropy_before_split.set_value("0.0", true);
  _sum_gain_po = 0.0;
  _sum_gain_neg = 0.0;
  _rata2 = 0.0;
  _sd = 0.0;

  _first_iteration = true;
  _max_gain_ratio.set_value("0.0", true);
  _max_gain.set_value("0.0", true);

  _is_continue = false;
  _tmp_split_value.set_value("-1", _is_continue);

  _jml_below = 0.0;
  _jml_above = 0.0;

  _jml_attr = 0;

  _vec_split_stat.clear();
  _tmp_vec_split_stat.clear();

  _idx_max_gain_ratio.clear();
  _idx_rata2.clear();

  _idx_normal.clear();
  _idx_known.clear();
}

Tproses_split_stat::~Tproses_split_stat()
{
  _vec_split_stat.clear();
  _tmp_vec_split_stat.clear();

  _idx_max_gain_ratio.clear();
  _idx_rata2.clear();

  _idx_normal.clear();
  _idx_known.clear();
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

    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    _sum_gain_po += (stod(hsl.gain.get_string()) > 0.0) ? stod(hsl.gain.get_string()) : 0.0;
    _sum_gain_neg += (stod(hsl.gain.get_string()) < 0.0) ? stod(hsl.gain.get_string()) : 0.0;

    _vec_split_stat.push_back(tmp_split_stat);

    _rata2 = (_sum_gain_po - abs(_sum_gain_neg)) / _vec_split_stat.size();
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

void Tproses_split_stat::kalkulasi_sd()
{
  double tmp_sum = 0.0;
  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();
    tmp_sum += pow((stod(hsl.gain.get_string()) - _rata2), 2);
  }

  if (_vec_split_stat.size() > 1)
  {
    _sd = sqrt(tmp_sum / (_vec_split_stat.size() - 1));
  }

}

void Tproses_split_stat::gen_split_attr_rec(int counter, int depth, int geser)
{
  if (counter == depth)
  {

    for (size_t i = geser; i < _jml_attr; ++i)
    {
      _tmp_vec_split_stat.push_back(_vec_split_stat[i]);
      insert_split_stat();
      del_last_tmp();
    }

  } else {

    counter++;

    for (size_t i = geser; i < _jml_attr; ++i)
    {
      _tmp_vec_split_stat.push_back(_vec_split_stat[i]);
      gen_split_attr_rec(counter, depth, i + 1);
      del_last_tmp();
    }


  }
}


void Tproses_split_stat::gen_split_attr()
{
  //_jml_attr = 0;
  clear_tmp();

  if ( (_vec_split_stat.size() > 3) and global_config.buat_kombinasi )
  {

    //_jml_attr = _vec_split_stat.size();

    size_t jml_kombinasi = ceil(_jml_attr / 2);

    //jml_kombinasi = (jml_kombinasi > 5 ? 5 : jml_kombinasi);

    for (size_t i = 2; i <= jml_kombinasi; ++i)
    {
      gen_split_attr_rec(0, i - 1, 0);
    }

    // for (size_t i = 0; i < _jml_attr; ++i)
    // {
    //   Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    //   Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    //   _sum_gain_po -= (stod(hsl.gain.get_string()) > 0.0) ? stod(hsl.gain.get_string()) : 0.0;
    //   _sum_gain_neg -= (stod(hsl.gain.get_string()) < 0.0) ? stod(hsl.gain.get_string()) : 0.0;
    // }

    // auto itr1 = _vec_split_stat.begin();
    // auto itr2 = _vec_split_stat.begin() + (_jml_attr);
    // _vec_split_stat.erase(itr1, itr2);

    // _rata2 = (_sum_gain_po - abs(_sum_gain_neg)) / _vec_split_stat.size();

  }
}

void Tproses_split_stat::split_by_label()
{
  _jml_attr = _vec_split_stat.size();

  if (_vec_split_stat.size() > 2)
  {
    for (size_t i = 0; i < _vec_split_stat.size(); ++i)
    {
      if (_vec_split_stat[i].is_normal())
      {
        _idx_normal.push_back(i);
      }

      if (_vec_split_stat[i].is_known())
      {
        _idx_known.push_back(i);
      }
    }

    if (_idx_normal.size() > 1)
    {
      Tsplit_stat tmp_split_stat = _vec_split_stat[_idx_normal[0]];

      for (size_t i = 1; i < _idx_normal.size(); ++i)
      {
        tmp_split_stat = tmp_split_stat + _vec_split_stat[_idx_normal[i]];
      }

      _vec_split_stat.push_back(tmp_split_stat);

      for (size_t i = _idx_normal.size() - 1; i > 0 ; --i)
      {
        auto itr = _vec_split_stat.begin() + _idx_normal[i];
        _vec_split_stat.erase(itr);
        _jml_attr--;
      }

      auto itr = _vec_split_stat.begin() + _idx_normal[0];
      _vec_split_stat.erase(itr);
      _jml_attr--;

    }

    if (_idx_known.size() > 1)
    {
      Tsplit_stat tmp_split_stat = _vec_split_stat[_idx_known[0]];

      for (size_t i = 1; i < _idx_known.size(); ++i)
      {
        tmp_split_stat = tmp_split_stat + _vec_split_stat[_idx_known[i]];
      }

      _vec_split_stat.push_back(tmp_split_stat);

      for (size_t i = _idx_known.size() - 1; i > 0 ; --i)
      {
        auto itr = _vec_split_stat.begin() + _idx_known[i];
        _vec_split_stat.erase(itr);
        _jml_attr--;
      }

      auto itr = _vec_split_stat.begin() + _idx_known[0];
      _vec_split_stat.erase(itr);
      _jml_attr--;
    }

    if ((_idx_normal.size() > 1) or (_idx_known.size() > 1))
    {
      Tsplit_stat tmp_split_stat = _vec_split_stat[0];

      for (size_t i = 1; i < _jml_attr; ++i)
      {
        tmp_split_stat = tmp_split_stat + _vec_split_stat[i];
      }

      _vec_split_stat.push_back(tmp_split_stat);

      for (size_t i = _jml_attr; i > 0 ; --i)
      {
        auto itr = _vec_split_stat.begin() + i;
        _vec_split_stat.erase(itr);
      }

      auto itr = _vec_split_stat.begin() + 0;
      _vec_split_stat.erase(itr);
    }
  }
}

Tmetric_split_value Tproses_split_stat::get_max_gain_ratio()
{

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    // double z_score = (stod(hsl.gain.get_string()) - _rata2) / _sd;

    // if ((z_score >= 0.0) and (z_score < 3.0)) //and (z_score < 3.0)
    // {
    bool is_pass = true;

    is_pass = global_config.use_credal ? true : (hsl.gain > 0.0); //global_config.use_credal ? true :

    if ((_first_iteration and is_pass) or (( _max_gain < hsl.gain) and is_pass))
    {
      //_idx_max_gain_ratio.push_back(i);

      _first_iteration = false;
      _tmp_split_value = tmp_split_stat.get_split_value();
      _max_gain_ratio = hsl.gain_ratio;
      _max_gain = hsl.gain;
      _jml_below = tmp_split_stat.get_jml_below();
      _jml_above = tmp_split_stat.get_jml_above();
    }

    //}

  }

  // int idx_gain_ratio_max = -1;

  // _max_gain_ratio.set_value("0.0", true);
  // _max_gain.set_value("0.0", true);
  // _tmp_split_value.set_value("-1", _is_continue);
  // _jml_below = 0.0;
  // _jml_above = 0.0;

  // if (_idx_max_gain_ratio.size() > 0)
  // {
  //   idx_gain_ratio_max = _idx_max_gain_ratio[_idx_max_gain_ratio.size() - 1];

  //   Tsplit_stat tmp_split_stat = _vec_split_stat[idx_gain_ratio_max];
  //   Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();
  //   _tmp_split_value = tmp_split_stat.get_split_value();
  //   _max_gain_ratio = hsl.gain_ratio;
  //   _max_gain = hsl.gain;
  //   _jml_below = tmp_split_stat.get_jml_below();
  //   _jml_above = tmp_split_stat.get_jml_above();
  // }

  // if (global_config.gunakan_rata2gain_pot_split and (_vec_split_stat.size() > 1))
  // {

  //   for (size_t i = 0; i < _idx_max_gain_ratio.size(); ++i)
  //   {
  //     size_t tmp_idx = _idx_max_gain_ratio[i];

  //     Tsplit_stat tmp_split_stat = _vec_split_stat[i];
  //     Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

  //     double z_score = 0.0;

  //     if (_sd != 0)
  //     {
  //       z_score = (stod(hsl.gain.get_string()) - _rata2) / _sd;
  //     }

  //     bool pass = (z_score > 0.0); //and (z_score < 3.0)
  //     if (pass)
  //     {
  //       _idx_rata2.push_back(tmp_idx);
  //     }
  //   }

  //   idx_gain_ratio_max = -1;

  //   _max_gain_ratio.set_value("0.0", true);
  //   _max_gain.set_value("0.0", true);
  //   _tmp_split_value.set_value("-1", _is_continue);
  //   _jml_below = 0.0;
  //   _jml_above = 0.0;

  //   if (_idx_rata2.size() > 0)
  //   {
  //     idx_gain_ratio_max = _idx_rata2[_idx_rata2.size() - 1];
  //     Tsplit_stat tmp_split_stat = _vec_split_stat[idx_gain_ratio_max];
  //     Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

  //     _tmp_split_value = tmp_split_stat.get_split_value();
  //     _max_gain_ratio = hsl.gain_ratio;
  //     _max_gain = hsl.gain;
  //     _jml_below = tmp_split_stat.get_jml_below();
  //     _jml_above = tmp_split_stat.get_jml_above();

  //   }

  // }

  // bool pass = true;

  // if (idx_gain_ratio_max != -1)
  // {
  //   pass = _vec_split_stat[idx_gain_ratio_max].cek_valid();
  // }

  // if (!pass)
  // {
  //   if (global_config.find_other_pot_split)
  //   {

  //     idx_gain_ratio_max = -1;

  //     _max_gain_ratio.set_value("0.0", true);
  //     _max_gain.set_value("0.0", true);
  //     _tmp_split_value.set_value("-1", _is_continue);
  //     _jml_below = 0.0;
  //     _jml_above = 0.0;

  //     if (global_config.gunakan_rata2gain_pot_split and (_idx_rata2.size() > 0))
  //     {
  //       int tmp_idx = -1;
  //       size_t i = 0;
  //       while ((i < (_idx_rata2.size() - 1)) and !pass)
  //       {
  //         tmp_idx = _idx_rata2[_idx_rata2.size() - (i + 1)];
  //         pass = _vec_split_stat[tmp_idx].cek_valid();
  //         i++;
  //       }

  //       if (pass and (tmp_idx != -1))
  //       {
  //         idx_gain_ratio_max = tmp_idx;
  //         Tsplit_stat tmp_split_stat = _vec_split_stat[idx_gain_ratio_max];
  //         Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

  //         _tmp_split_value = tmp_split_stat.get_split_value();
  //         _max_gain_ratio = hsl.gain_ratio;
  //         _max_gain = hsl.gain;
  //         _jml_below = tmp_split_stat.get_jml_below();
  //         _jml_above = tmp_split_stat.get_jml_above();
  //       }


  //     } else {

  //       if (_idx_max_gain_ratio.size() > 0)
  //       {
  //         int tmp_idx = -1;
  //         size_t i = 0;
  //         while ( (i < (_idx_max_gain_ratio.size() - 1)) and !pass)
  //         {
  //           tmp_idx = _idx_max_gain_ratio[_idx_max_gain_ratio.size() - (i + 1)];
  //           pass = _vec_split_stat[tmp_idx].cek_valid();
  //           i++;
  //         }

  //         if (pass and (tmp_idx != -1))
  //         {
  //           idx_gain_ratio_max = tmp_idx;
  //           Tsplit_stat tmp_split_stat = _vec_split_stat[idx_gain_ratio_max];
  //           Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

  //           _tmp_split_value = tmp_split_stat.get_split_value();
  //           _max_gain_ratio = hsl.gain_ratio;
  //           _max_gain = hsl.gain;
  //           _jml_below = tmp_split_stat.get_jml_below();
  //           _jml_above = tmp_split_stat.get_jml_above();
  //         }
  //       }
  //     }
  //   }
  // }



  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(_max_gain_ratio.get_string());
  hsl_split.max_gain = stof(_max_gain.get_string());
  hsl_split.split_value = _tmp_split_value;
  hsl_split.jml_below = _jml_below;
  hsl_split.jml_above = _jml_above;


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

