
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

    proses_split_stat.kalkulasi_sd();    

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

  proses_split_stat.kalkulasi_sd();

  Tmetric_split_value tmp_hsl;

  tmp_hsl = proses_split_stat.get_max_gain_ratio();

  hsl_split.idx = idx;
  hsl_split.max_gain_ratio = tmp_hsl.max_gain_ratio;
  hsl_split.max_gain = tmp_hsl.max_gain;
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


Tdataframe::Tcari_gain_max::Tcari_gain_max()
{
  first_iteration = true;
  max_gain_ratio.set_value("0.0", true);
  max_gain.set_value("0.0", true);
  tmp_split_value.set_value("-1", false);
  idx_attr = 0;
  jml_below = 0;
  jml_above = 0;

  is_multiway = false;
  multiway_gain_ratio.set_value("0.0", true);
}

void Tdataframe::Tcari_gain_max::set_idx_attr(int idx)
{
  idx_attr = idx;
}

void Tdataframe::Tcari_gain_max::set_is_multiway(bool stat)
{
  is_multiway = stat;
}

void Tdataframe::Tcari_gain_max::set_multiway_gain_ratio(Tmy_dttype gain_ratio)
{
  multiway_gain_ratio = gain_ratio;
}


void Tdataframe::Tcari_gain_max::cari_gain_max(Tlabel_stat stat_below, Tlabel_stat stat_above, Tmy_dttype mid_point, Tmy_dttype entropy_before_split)
{
  Tbelow_above tmp_ba;
  tmp_ba.add_below(stat_below);
  tmp_ba.add_above(stat_above);

  Tgain_ratio hsl = tmp_ba.kalkulasi_gain_ratio(entropy_before_split);

  bool is_pass = global_config.use_credal ? true : (hsl.gain_ratio > 0.0);

  if ((first_iteration and is_pass) or (( max_gain_ratio < hsl.gain_ratio) and is_pass))
  {
    bool is_pass1 = true;

    if (is_multiway) {
      is_pass1 = hsl.gain_ratio <= multiway_gain_ratio;
    }

    if (is_pass1)
    {
      first_iteration = false;
      tmp_split_value = mid_point;
      max_gain_ratio = hsl.gain_ratio;
      max_gain = hsl.gain;
      jml_below = stat_below.get_jml_row();
      jml_above = stat_above.get_jml_row();
    }


  }



}

Tmetric_split_value Tdataframe::Tcari_gain_max::get_gain_max()
{
  Tmetric_split_value hsl_split;

  hsl_split.idx = idx_attr;
  hsl_split.max_gain_ratio = stof(max_gain_ratio.get_string());
  hsl_split.max_gain = stof(max_gain.get_string());
  hsl_split.split_value = tmp_split_value;
  hsl_split.jml_below = jml_below;
  hsl_split.jml_above = jml_above;

  return hsl_split;
}

// Tdataframe::Thanlde_split_map::Thanlde_split_map()
// {
//   _rata2 = 0.0;
// }


// Tdataframe::Thanlde_split_map::~Thanlde_split_map()
// {
//   _vec_split_stat.clear();
// }

// void Tdataframe::Thanlde_split_map::set_value(Tmy_dttype entropy_before_split, Tlabel_stat stat_label)
// {
//   _entropy_before_split = entropy_before_split;
//   _stat_label = stat_label;
// }

// bool Tdataframe::Thanlde_split_map::cmp(Tsplit_stat a, Tsplit_stat b)
// {
//   return a.label_stat.get_jml_row() > b.label_stat.get_jml_row();
// }

// void Tdataframe::Thanlde_split_map::konversi_map_vec(map<Tmy_dttype, Tlabel_stat> &map_split_stat)
// {
//   for (auto itr = map_split_stat.begin(); itr != map_split_stat.end(); ++itr)
//   {
//     Tsplit_stat tmp;
//     tmp.split_value = ((Tmy_dttype) (*itr).first);
//     tmp.label_stat = (*itr).second;
//     _vec_split_stat.push_back(tmp);
//   }

//   sort(_vec_split_stat.begin(), _vec_split_stat.end(), Thanlde_split_map::cmp);

//   int jml_total = _stat_label.get_jml_row();
//   for (size_t i = 0; i < _vec_split_stat.size(); ++i)
//   {
//     _rata2 = _rata2 + ((float) _vec_split_stat[i].label_stat.get_jml_row() / jml_total);
//   }
// }

// void Tdataframe::Thanlde_split_map::gen_kombinasi_normal(int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below)
// {

//   if (counter == depth)
//   {

//     Tcari_gain_max cari_gain_max;

//     for (size_t i = geser; i < _vec_split_stat.size(); ++i)
//     {
//       Tlabel_stat tmp_stat = _vec_split_stat[i].label_stat;

//       Tmy_dttype mid_point;
//       mid_point.set_value(v_mid_point + ';' + _vec_split_stat[i].split_value.get_string(), false);

//       Tlabel_stat stat_below = v_stat_below + tmp_stat;
//       Tlabel_stat stat_above = _stat_label - stat_below;

//       _cari_gain_max.cari_gain_max(stat_below, stat_above, mid_point, _entropy_before_split);

//     }

//   } else {

//     counter++;

//     for (size_t i = geser; i < (_vec_split_stat.size() - 1); ++i)
//     {
//       Tlabel_stat tmp_stat = _vec_split_stat[i].label_stat;
//       Tlabel_stat tmp_v_stat_below = v_stat_below + tmp_stat;
//       string tmp_v_mid_point = _vec_split_stat[i].split_value.get_string();
//       if (v_mid_point != "") {
//         tmp_v_mid_point = v_mid_point + ';' + _vec_split_stat[i].split_value.get_string();
//       }
//       gen_kombinasi_normal(counter, depth, i + 1, tmp_v_mid_point, tmp_v_stat_below);
//     }


//   }


// }

// Tmetric_split_value Tdataframe::Thanlde_split_map::cari_gain(int idx)
// {
//   Tmetric_split_value tmp_hsl;
//   tmp_hsl.split_value.set_value("-1", false);

//_cari_gain_max.set_idx_attr(idx);

//Tbelow_above_kategori ba;
// Tmy_dttype mid_point("-1", false);
// Tmy_dttype tmp_split_value("-1", false);

//Tstat_gain_split_holder data_svm;
// Tstat_gain_split_holder data_known;
// Tstat_gain_split_holder data_normal;
// Tstat_gain_split_holder data_rata2;

//Tlabel_stat stat_below;

// for (size_t i = 0; i < _vec_split_stat.size(); ++i)
// {
//   mid_point = _vec_split_stat[i].split_value;

//   Tlabel_stat stat_below = _vec_split_stat[i].label_stat;

//ba.add_stat(_vec_split_stat[i].label_stat);

// if (idx == 2)
// {
//   if ((mid_point == "private") or (mid_point == "ecri") or (mid_point == "ecr_i") or (mid_point == "http"))
//   {
//     data_svm.set_value(mid_point, stat_below, _stat_label);
//   }
// }

// if (idx == 3)
// {
//   if (mid_point == "SF")
//   {
//     data_svm.set_value(mid_point, stat_below, _stat_label);
//   }
// }

// if (idx == 6)
// {
//   if (mid_point == "0")
//   {
//     data_svm.set_value(mid_point, stat_below, _stat_label);
//   }
// }

// if (_vec_split_stat.size() > 3)
// {
//   if (stat_below.get_max_label() == "normal")
//   {
//     data_normal.set_value(mid_point, stat_below, _stat_label);
//   }

//   if (stat_below.get_max_label() == "known")
//   {
//     data_known.set_value(mid_point, stat_below, _stat_label);
//   }

//   if (_rata2 < _vec_split_stat[i].label_stat.get_jml_row())
//   {
//     data_rata2.set_value(mid_point, stat_below, _stat_label);
//   }
// }

// }

// Tgain_ratio_kategori hsl_kategori = ba.kalkulasi_gain_ratio(_entropy_before_split);

// if (_vec_split_stat.size() > 2)
// {
//   _cari_gain_max.set_is_multiway(global_config.binary_gain_ratio_limited_by_non_binary_gain_ratio);
//   _cari_gain_max.set_multiway_gain_ratio(hsl_kategori.gain_ratio);
// } else {
//   _cari_gain_max.set_is_multiway(false);
// }

// if (_vec_split_stat.size() == 2)
// {
//   for (size_t i = 0; i < _vec_split_stat.size(); ++i)
//   {
//     mid_point = _vec_split_stat[i].split_value;

//     Tlabel_stat stat_below = _vec_split_stat[i].label_stat;
//     Tlabel_stat stat_above = _stat_label - stat_below;

//     _cari_gain_max.cari_gain_max(stat_below, stat_above, mid_point, _entropy_before_split);

//   }
// }


// if (!global_config.one_agains_many_only)
// {
// if (!data_normal.is_empty() and !data_known.is_empty())
// {
//   if (data_normal.get_jml() < data_known.get_jml())
//   {
//     _cari_gain_max.cari_gain_max(data_normal.get_stat_below(), data_normal.get_stat_above(), data_normal.get_mid_point(), _entropy_before_split);

//   } else {

//     _cari_gain_max.cari_gain_max(data_known.get_stat_below(), data_known.get_stat_above(), data_known.get_mid_point(), _entropy_before_split);

//   }
// }

// if (!data_rata2.is_empty()) {
//   _cari_gain_max.cari_gain_max(data_rata2.get_stat_below(), data_rata2.get_stat_above(), data_rata2.get_mid_point(), _entropy_before_split);
// }

// if (!data_svm.is_empty())
// {
//   _cari_gain_max.cari_gain_max(data_svm.get_stat_below(), data_svm.get_stat_above(), data_svm.get_mid_point(), _entropy_before_split);
// }

// }


// if (global_config.buat_kombinasi and !global_config.one_agains_many_only)
// {
//   size_t jml_kombinasi = 2;
//   while ((_vec_split_stat.size() > (jml_kombinasi * 2)))
//   {
//     if (jml_kombinasi < 5)
//     {
//       Tlabel_stat tmp_stat;
//       Tmetric_split_value tmp_split;

//       gen_kombinasi_normal(0, jml_kombinasi - 1 , 0, "", tmp_stat);

//     }
//     jml_kombinasi++;
//   }
// }


//tmp_hsl = _cari_gain_max.get_gain_max();

// if (global_config.use_non_binary_gain_ratio) {

//   if ( (stof(hsl_kategori.gain_ratio.get_string()) != 0.0) and ((tmp_hsl.max_gain_ratio == 0) or (tmp_hsl.max_gain_ratio == -1)))
//   {
//     cout << "[ " << idx << " " << tmp_hsl.max_gain_ratio << " " << hsl_kategori.gain_ratio.get_string();
//     tmp_hsl.split_value.set_value("-1", false);
//     cout << " 1 big problem !!! ";
//     if (_vec_split_stat.size() == 2)
//     {
//       cout << " 2 big problem !!! ";
//     }
//     cout << "]" << endl;
//   } else {
//     tmp_hsl.max_gain_ratio = stof(hsl_kategori.gain_ratio.get_string());
//     tmp_hsl.max_gain = stof(hsl_kategori.gain.get_string());
//   }
// }

//   return tmp_hsl;
// }




Tdataframe::Tstat_gain_split_holder::Tstat_gain_split_holder()
{
  mid_point.set_value("", false);
  jml = 0;
}

Tdataframe::Tstat_gain_split_holder::~Tstat_gain_split_holder()
{

}

void Tdataframe::Tstat_gain_split_holder::set_value(Tmy_dttype v_mid_point, Tlabel_stat v_stat_below, Tlabel_stat stat)
{
  if (mid_point == "")
  {
    mid_point = v_mid_point;
  } else {
    Tmy_dttype separator(";", false);
    mid_point = mid_point + separator + v_mid_point;
  }

  stat_below = stat_below + v_stat_below;
  stat_above = stat - stat_below;

  jml++;
}

Tmy_dttype Tdataframe::Tstat_gain_split_holder::get_mid_point()
{
  return mid_point;
}

Tlabel_stat Tdataframe::Tstat_gain_split_holder::get_stat_below()
{
  return stat_below;
}

Tlabel_stat Tdataframe::Tstat_gain_split_holder::get_stat_above()
{
  return stat_above;
}

bool Tdataframe::Tstat_gain_split_holder::is_empty()
{
  return jml == 0;
}

int Tdataframe::Tstat_gain_split_holder::get_jml()
{
  return jml;
}

Tgain_ratio Tdataframe::Tstat_gain_split_holder::kalkulasi_gain_ratio(Tmy_dttype entropy_before_split)
{
  Tbelow_above tmp_ba;
  tmp_ba.add_below(stat_below);
  tmp_ba.add_above(stat_above);

  Tgain_ratio hsl = tmp_ba.kalkulasi_gain_ratio(entropy_before_split);

  return hsl;
}

Tdataframe::Tcari_pencilan::Tcari_pencilan()
{
  sum_neg = 0.0;
  sum_po = 0.0;
  rata2 = 0.0;
  sd = 0.0;
  jml = 0;

  map_gain.clear();
  map_z_score.clear();
}

Tdataframe::Tcari_pencilan::~Tcari_pencilan()
{
  map_gain.clear();
  map_z_score.clear();
}

void Tdataframe::Tcari_pencilan::insert_gain(int idx, double gain)
{
  map_gain.insert({idx, gain});
  sum_po += (gain > 0.0) ? gain : 0.0;
  sum_neg += (gain < 0.0) ? gain : 0.0;
  jml++;

  rata2 = (sum_po - abs(sum_neg)) / jml;

  double tmp_sum = 0.0;
  for (auto itr = map_gain.begin(); itr != map_gain.end(); ++itr)
  {
    tmp_sum += pow((itr->second - rata2), 2);
  }

  sd = sqrt(tmp_sum / (jml - 1));

  map_z_score.clear();
  for (auto itr = map_gain.begin(); itr != map_gain.end(); ++itr)
  {
    map_z_score.insert({itr->first, ((itr->second - rata2) / sd)});
  }

}


bool Tdataframe::Tcari_pencilan::cek_valid(int idx)
{
  bool pass = true;
  pass = (map_z_score[idx] > 0.0) and (map_z_score[idx] < 3.0) ;
  return pass;
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
  _entropy_before_split.set_value("0.0", true);
  _sum_gain_po = 0.0;
  _sum_gain_neg = 0.0;
  _rata2 = 0.0;
  _sd = 0.0;

  _first_iteration = true;
  _max_gain_ratio.set_value("0.0", true);
  _max_gain.set_value("0.0", true);
  _tmp_split_value.set_value("-1", false);
  _jml_below = 0.0;
  _jml_above = 0.0;
}

Tproses_split_stat::~Tproses_split_stat()
{
  _vec_split_stat.clear();
  _tmp_vec_split_stat.clear();
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

void Tproses_split_stat::clear_tmp()
{
  _tmp_vec_split_stat.clear();
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

  _sd = sqrt(tmp_sum / (_vec_split_stat.size() - 1));
}

Tmetric_split_value Tproses_split_stat::get_max_gain_ratio()
{

  for (size_t i = 0; i < _vec_split_stat.size(); ++i)
  {
    Tsplit_stat tmp_split_stat = _vec_split_stat[i];
    Tgain_ratio hsl = tmp_split_stat.kalkulasi_gain_ratio();

    double z_score = (stod(hsl.gain.get_string()) - _rata2) / _sd;

    if ((z_score > 0.0) and (z_score < 3.0) )
    {
      bool is_pass = global_config.use_credal ? true : (hsl.gain_ratio > 0.0);

      if ((_first_iteration and is_pass) or (( _max_gain_ratio < hsl.gain_ratio) and is_pass))
      {
        _first_iteration = false;
        _tmp_split_value = tmp_split_stat.get_split_value();
        _max_gain_ratio = hsl.gain_ratio;
        _max_gain = hsl.gain;
        _jml_below = tmp_split_stat.get_jml_below();
        _jml_above = tmp_split_stat.get_jml_above();
      }

    }

  }

  Tmetric_split_value hsl_split;

  hsl_split.max_gain_ratio = stof(_max_gain_ratio.get_string());
  hsl_split.max_gain = stof(_max_gain.get_string());
  hsl_split.split_value = _tmp_split_value;
  hsl_split.jml_below = _jml_below;
  hsl_split.jml_above = _jml_above;


  return hsl_split;

}

void Tproses_split_stat::set_entropy_before_split(Tmy_dttype entropy_before_split)
{
  _entropy_before_split = entropy_before_split;
}

