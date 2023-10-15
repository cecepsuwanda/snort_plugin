
#include "Tdataframe.h"
#include <iterator>


Tdataframe::Tdataframe()
{
  _idx_label = -1;
}



Tdataframe::~Tdataframe()
{
  _stat_label.clear();
}

// void Tdataframe::set_search_uniqe_val_off()
// {
//   _search_uniqe_val_on = false;
// }

void Tdataframe::set_shared_memory_on()
{
  _data.set_shared_memory_on();
}

void Tdataframe::set_shared_memory_off()
{
  _data.set_shared_memory_off();
}


void Tdataframe::read_data_type(string nm_f)
{

  Tread_file tmp;
  tmp.setnm_f(nm_f, ": ");

  //cout << "Tdataframe read_data_type setnm_f " << endl;

  int i = 0;
  while (!tmp.is_eof())
  {
    _data_header.push_back(tmp.get_col_val(0));
    _data_type.push_back(tmp.get_col_val(1));

    if (tmp.get_col_val(0) == "label")
    {
      _idx_label = i;
    }

    if ((tmp.get_col_val(0) != "label") and (tmp.get_col_val(1) != "continuous."))
    {
      is_non_continuous = true;
    }

    //cout << "Tdataframe read_data_type next_record " << endl;
    tmp.next_record();
    i++;
  }
  _jml_col = i;
  tmp.close_file();

  if (is_non_continuous)
  {
    is_42 = _jml_col == 42;
  }


  stat_tabel();
}

void Tdataframe::stat_tabel()
{
  _stat_label.clear();

  _data.index_on();
  if (_filter.size() > 0) {
    _data.clear_index();
  } else {
    _data.index_off();
  }

  int i = 0;

  _data.reset_file();
  while (!_data.is_eof())
  {

    if (is_pass())
    {
      if (_filter.size() > 0) {
        _data.add_index();
      }

      _stat_label.add(_data.get_col_val(_idx_label));

      if (global_config.search_uniqe_val)
      {
        while (!_data.is_end_col())
        {
          if (_data.get_idx_col() != _idx_label) {
            
            auto itr = _unique_attr.find(_data.get_idx_col());
            if(itr == _unique_attr.end())
            {
             _map_col_split.add_data(_data.get_idx_col(), _data.get_col_val(), _data_type[_data.get_idx_col()], _data.get_col_val(_idx_label));     
            }           
          
          }
          _data.next_col();
        }
      }

      i++;
    }
    _data.next_record();
  }

  if (_filter.size() > 0) {
    _data.clear_memory();
    _data.save_to_memory();
    _data.clear_index();
  }

  _data.index_on();
  _jml_row = i;

  if (global_config.search_uniqe_val)
  {
    _map_col_split.cek_valid_attr(_jml_row);
  }

}

int Tdataframe::get_jml_stat(string label)
{
  return _stat_label.get_jml_stat(label);
}

map<string, int> Tdataframe::get_stat_label()
{
  return _stat_label.get_map();
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

map<int, int> Tdataframe::get_unique_attr()
{
  return _unique_attr;
}

int Tdataframe::getjmlcol_svm()
{
  if (is_non_continuous)
  {
    if (global_config.feature_selection)
    {
      int jml = 0;
      for (auto itr = _unique_attr.begin(); itr != _unique_attr.end(); ++itr)
      {
        switch (itr->first) {
        case 1:
          jml = jml + 3;
          break;
        case 2:
          jml = jml + 2;
          break;
        case 3:
          jml = jml + 2;
          break;
        default:
          jml = jml + 1;
          break;
        }
      }
      return jml + 1;

    } else {
      return  (is_42 ? 46 : 33);
    }
  } else {
    if (global_config.feature_selection)
    {
      return _unique_attr.size() + 1;
    } else {
      return _jml_col;
    }
  }
}

int Tdataframe::getjmlrow_svm()
{
  if (global_config.normal_only)
  {
    return _stat_label.get_value("normal");
  } else {
    return _jml_row;
  }
}

vector<string> Tdataframe::get_record_svm()
{
  if (!is_non_continuous)
  {
    if (global_config.feature_selection) {

      vector<string> vec;
      for (auto itr = _unique_attr.begin(); itr != _unique_attr.end(); ++itr)
      {
        vec.push_back(_data.get_col_val(itr->first));
      }

      vec.push_back(_data.get_col_val(_idx_label));

      return vec;

    } else {
      return _data.get_record();
    }

  } else {

    if (global_config.feature_selection) {

      vector<string> vec;
      for (auto itr = _unique_attr.begin(); itr != _unique_attr.end(); ++itr)
      {
        switch (itr->first) {
        case 1:
          vec.push_back((_data.get_col_val(itr->first) == "tcp" ? "1" : "0" ));
          vec.push_back((_data.get_col_val(itr->first) == "udp" ? "1" : "0" ));
          vec.push_back((_data.get_col_val(itr->first) == "icmp" ? "1" : "0" ));
          break;
        case 2:
          vec.push_back(((_data.get_col_val(itr->first) == "private") or (_data.get_col_val(itr->first) == "ecri") or (_data.get_col_val(itr->first) == "ecr_i") or (_data.get_col_val(itr->first) == "http")) ? "0" : "1");
          vec.push_back(((_data.get_col_val(itr->first) == "private") or (_data.get_col_val(itr->first) == "ecri") or (_data.get_col_val(itr->first) == "ecr_i") or (_data.get_col_val(itr->first) == "http")) ? "1" : "0");
          break;
        case 3:
          vec.push_back((_data.get_col_val(itr->first) == "SF") ? "0" : "1");
          vec.push_back((_data.get_col_val(itr->first) == "SF") ? "1" : "0");
          break;
        default:
          vec.push_back(_data.get_col_val(itr->first));
          break;
        }
      }

      vec.push_back(_data.get_col_val(_idx_label));

      return vec;
    } else {

      vector<string> vec;//, tmp_data = _data.get_record();

      for (int i = 0; i < _jml_col; ++i)
      {

        switch (i) {
        case 1:
          vec.push_back((_data.get_col_val(i) == "tcp" ? "1" : "0" ));
          vec.push_back((_data.get_col_val(i) == "udp" ? "1" : "0" ));
          vec.push_back((_data.get_col_val(i) == "icmp" ? "1" : "0" ));
          break;
        case 2:
          vec.push_back(((_data.get_col_val(i) == "private") or (_data.get_col_val(i) == "ecri") or (_data.get_col_val(i) == "ecr_i") or (_data.get_col_val(i) == "http")) ? "0" : "1");
          vec.push_back(((_data.get_col_val(i) == "private") or (_data.get_col_val(i) == "ecri") or (_data.get_col_val(i) == "ecr_i") or  (_data.get_col_val(i) == "http")) ? "1" : "0");
          break;
        case 3:
          vec.push_back((_data.get_col_val(i) == "SF") ? "0" : "1");
          vec.push_back((_data.get_col_val(i) == "SF") ? "1" : "0");
          break;
        default:
          vec.push_back(_data.get_col_val(i));
          break;
        }

      }

      //tmp_data.clear();
      //tmp_data.shrink_to_fit();

      return vec;
    }
  }
}

vector<vector<string>> Tdataframe::get_all_record_svm()
{
  //std::lock_guard<std::mutex> lock(v_mutex);
  ReFilter();
  if (global_config.search_uniqe_val) {
    clear_col_split();
  }

  vector<vector<string>> Table;

  vector<string> tmp_data;

  _data.reset_file();
  while (!_data.is_eof())
  {
    //cout << " get_all_record_svm get_record_svm " << endl;
    tmp_data = get_record_svm();

    bool is_pass = (global_config.normal_only ? (tmp_data[tmp_data.size() - 1].compare("normal") == 0) : true);
    if (is_pass) {
      Table.push_back(tmp_data);
    }
    //cout << " get_all_record_svm next_record " << endl;
    _data.next_record();
  }

  clear_memory();

  return Table;
}

map<Tmy_dttype, Tlabel_stat>* Tdataframe::get_col_split(int idx)
{
  return _map_col_split.get_pot_split(idx);
}


void Tdataframe::clear_col_split()
{
  _map_col_split.clear();
}

// void Tdataframe::set_config(Tconfig v_config)
// {
//   config = v_config;
//   _stat_label.set_config(config);
//   _map_col_split.set_config(config);
// }

int Tdataframe::get_jml_valid_attr()
{
  return _map_col_split.get_jml_valid_attr();
}

int Tdataframe::get_valid_attr(int idx)
{
  return _map_col_split.get_valid_attr(idx);
}

void Tdataframe::add_filter(int idx_col, int idx_opt, Tmy_dttype value)
{
  field_filter f;
  f.idx_col = idx_col;
  f.idx_opt = idx_opt;
  f.value = value;
  _filter.push_back(f);

  auto itr = _unique_attr.find(idx_col);
  if (itr == _unique_attr.end()) {
    _unique_attr.insert(pair<int, int>(idx_col, 1));
  } else {
    itr->second += 1;
  }

  stat_tabel();
}

void Tdataframe::ReFilter()
{
  stat_tabel();
}

void Tdataframe::clear_map_col_split()
{
  _map_col_split.clear();
}

void Tdataframe::add_filter(field_filter filter)
{
  _filter.push_back(filter);

  auto itr = _unique_attr.find(filter.idx_col);
  if (itr == _unique_attr.end()) {
    _unique_attr.insert(pair<int, int>(filter.idx_col, 1));
  } else {
    itr->second += 1;
  }

  stat_tabel();
}


void Tdataframe::info()
{
  cout << " Info" << endl;
  cout << " Nama File   : " << _nm_file << endl;
  cout << " Jumlah Data : " << _jml_row << endl;

  cout << _stat_label << endl ;
}

void Tdataframe::split_data(int split_column, Tmy_dttype split_value, Tdataframe &data_below, Tdataframe &data_above)
{
  _data.clear_memory();
  if (split_value != "-1") {
    if (_data_type[split_column] == "continuous.")
    {
      data_below.add_filter(split_column, 0, split_value);
      data_above.add_filter(split_column, 1, split_value);

    } else {
      data_below.add_filter(split_column, 2, split_value);
      data_above.add_filter(split_column, 3, split_value);
    }
  }
}

void Tdataframe::get_col_pot_split(int idx)
{
  std::lock_guard<std::mutex> lock(v_mutex);
  _data.reset_file();
  while (!_data.is_eof())
  {
    _map_col_split.add_data(idx, _data.get_col_val(idx), _data_type[idx], _data.get_col_val(_idx_label));
    _data.next_record();
  }

  _map_col_split.cek_valid_attr(_jml_row);
}


void Tdataframe::calculate_metric(map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, string & split_value, Tlabel_stat & stat_label)
{
  // float entropy_before_split;

  // if (!global_config.use_credal) {
  //   entropy_before_split = stat_label.get_entropy();
  // } else {
  //   entropy_before_split = stat_label.get_credal_entropy();
  // }

  // float best_overall_metric = 0.0;
  // float gain = 0, gain_max = 0;
  // string mid_point = "0.0";
  // string tmp_split_value = "-1";

  // bool first_iteration = true;

  // Tlabel_stat _stat_label_below;


  // auto itr_next = _col_pot_split->begin();
  // itr_next++;

  // auto itr = _col_pot_split->begin();

  // size_t i = 1;
  // while ((itr != _col_pot_split->end()))
  // {
  //   _stat_label_below = _stat_label_below + (*itr).second;


  //   if (((itr != itr_next) and (itr_next != _col_pot_split->end()) ))
  //   {
  //     Tmy_dttype tmp1 = (*itr).first;
  //     Tmy_dttype tmp2 = (*itr_next).first;
  //     try {
  //       mid_point = to_string((stof(tmp1.get_string()) + stof(tmp2.get_string())) / 2);
  //     }
  //     catch (const std::invalid_argument& ia) {
  //       cout << tmp1.get_string() << "+" << tmp2.get_string() << " ";

  //     }

  //     Tbelow_above ba;
  //     //ba.set_value(mid_point);

  //     ba.add_below(_stat_label_below);
  //     Tlabel_stat tmp_stat = stat_label - _stat_label_below;
  //     ba.add_above(tmp_stat);

  //     gain = 0;
  //     if (ba.cek_valid()) {
  //       float entropy_after_split = ba.get_overall_metric();
  //       float split_info = ba.get_split_info();
  //       gain = (entropy_before_split - entropy_after_split) / split_info;
  //     }

  //   }

  //   itr_next++;
  //   itr++;
  //   i++;

  //   if ((first_iteration and (gain > 0)) or (gain_max < gain))
  //   {
  //     first_iteration = false;
  //     gain_max = gain;
  //     tmp_split_value = mid_point;
  //     best_overall_metric = gain_max;
  //   }

  // }

  // current_overall_metric = best_overall_metric;
  // split_value = tmp_split_value;

}

Tmetric_split_value Tdataframe::handle_continuous(int idx)
{

  map<Tmy_dttype, Tlabel_stat>* _col_pot_split = _map_col_split.get_pot_split(idx);

  Tmetric_split_value hsl;

  if (_col_pot_split->size() > 0)
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

    auto itr_next = _col_pot_split->begin();
    itr_next++;

    auto itr = _col_pot_split->begin();
    while ((itr != _col_pot_split->end()))
    {

      stat_label_below = stat_label_below + (*itr).second;
      stat_label_above = _stat_label - stat_label_below;

      if (((itr != itr_next) and (itr_next != _col_pot_split->end()) ))
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

    Tmetric_split_value tmp_hsl;

    tmp_hsl = proses_split_stat.get_max_gain_ratio();

    hsl.idx = idx;
    hsl.max_gain_ratio = tmp_hsl.max_gain_ratio;
    hsl.max_gain = tmp_hsl.max_gain;
    hsl.split_value = tmp_hsl.split_value;
    hsl.jml_below = tmp_hsl.jml_below;
    hsl.jml_above = tmp_hsl.jml_above;
  }

  return hsl;
}

Tmetric_split_value Tdataframe::handle_non_continuous(int idx)
{
  Tmy_dttype entropy_before_split;
  Tmetric_split_value hsl;

  if (!global_config.use_credal) {
    entropy_before_split = _stat_label.get_entropy();
  } else {
    entropy_before_split = _stat_label.get_credal_entropy();
  }



  map<Tmy_dttype, Tlabel_stat>* _col_pot_split = _map_col_split.get_pot_split(idx);

  Tproses_split_stat proses_split_stat;
  proses_split_stat.set_entropy_before_split(entropy_before_split);
  proses_split_stat.set_continue(false);
  proses_split_stat.set_idx_attr(idx);


  auto itr = _col_pot_split->begin();
  while (itr != _col_pot_split->end())
  {
    Tmy_dttype mid_point = (*itr).first;

    Tlabel_stat stat_below = (*itr).second;
    Tlabel_stat stat_above = _stat_label - stat_below;

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

    itr++;
  }


  proses_split_stat.insert_split_stat();
  proses_split_stat.clear_tmp();

  Tmetric_split_value tmp_hsl;

  tmp_hsl = proses_split_stat.get_max_gain_ratio();

  hsl.idx = idx;
  hsl.max_gain_ratio = tmp_hsl.max_gain_ratio;
  hsl.max_gain = tmp_hsl.max_gain;
  hsl.split_value = tmp_hsl.split_value;
  hsl.jml_below = tmp_hsl.jml_below;
  hsl.jml_above = tmp_hsl.jml_above;

  return hsl;

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
