
#include "Tdataframe.h"
#include <iterator>


Tdataframe::Tdataframe()
{
  _idx_label = -1;
  config = NULL;


}

Tdataframe::Tdataframe(Tconfig* v_config)
{
  _idx_label = -1;
  config = v_config;
  _stat_label.set_config(config);
  _map_col_split.set_config(config);
}

Tdataframe::~Tdataframe()
{
  _stat_label.clear();
  _data.set_child(_child_depth, _child_branch);
}


void Tdataframe::read_header_type()
{
  _data.read_header_type();
  _data_header = _data.get_data_header();
  _data_type = _data.get_data_type();
  _idx_label = _data.get_idx_label();

  is_non_continuous = true;
  _jml_col = _idx_label + 1;


  if (is_non_continuous)
  {
    is_42 = _jml_col == 42;
  }
}

void Tdataframe::set_config(Tconfig* v_config)
{
  config = v_config;
}



Tpot_split Tdataframe::get_pot_split(int id_dt, int jns_dt, string partition, int parent_depth, int parent_branch, int child_depth, int child_branch, int idx)
{
  Tpot_split hsl;
  tb_dataset data;
  data.set_dataset(id_dt, jns_dt, partition);
  data.read_header_type();
  data.set_parent(parent_depth, parent_branch);
  data.set_child(child_depth, child_branch);
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
    string tmp_sql = unique_filter_to_query(is_last);
    _data.filter(tmp_sql,!is_last);
  }

  _jml_row = _data.get_jml_row();


  if (is_stat_label) {
    _stat_label.clear();
    _stat_label = _data.hit_label_stat();
    _stat_label.set_config(config);
  }

}

void Tdataframe::search_col_split()
{
  vector<future<Tpot_split>> async_worker;

  for (size_t i = 0; i < (_data_header.size() - 1); ++i)
  {
    async_worker.push_back(async(std::launch::async, &Tdataframe::get_pot_split, config->id_dt_train, config->jns_dt_train, config->partition_train, _parent_depth, _parent_branch, _child_depth, _child_branch, i));

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

  _map_col_split.cek_valid_attr(_jml_row);
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

map<int, map<int, string>> Tdataframe::get_unique_filter()
{
  return _unique_filter;
}

int Tdataframe::getjmlcol_svm()
{
  if (is_non_continuous)
  {
    return  (is_42 ? 46 : 33);
  } else {
    return _jml_col;
  }
}

int Tdataframe::getjmlrow_svm()
{
  if (config->normal_only)
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
    return _data.get_record();

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

vector<vector<string>> Tdataframe::get_all_record_svm()
{
  //std::lock_guard<std::mutex> lock(v_mutex);
  // ReFilter();

  vector<vector<string>> Table;

  // vector<string> tmp_data;

  // _data.reset_file();
  // while (!_data.is_eof())
  //{
  //cout << " get_all_record_svm get_record_svm " << endl;
  //tmp_data = get_record_svm();

  //bool is_pass = (config->normal_only ? (tmp_data[tmp_data.size() - 1].compare("normal") == 0) : true);
  //if (is_pass) {
  //Table.push_back(tmp_data);
  //}
  //cout << " get_all_record_svm next_record " << endl;
  //_data.next_record();
  //}

  return Table;
}

map<Tmy_dttype, Tlabel_stat> Tdataframe::get_col_split(int idx)
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


string Tdataframe::unique_filter_to_query(bool is_last)
{
  string tmp = "";

  if (_unique_filter.size() > 0)
  {

    if (is_last)
    {
      string tmp1 = "(";

      switch (_filter[_filter.size() - 1].idx_opt)
      {
      case 0 :
        tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + "<=" + _filter[_filter.size() - 1].value + ")" ;
        break;
      case 1 :
        tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] +  ">" + _filter[_filter.size() - 1].value + ")";
        break;
      case 2 :
        tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + "='" + _filter[_filter.size() - 1].value + "')";
        break;
      case 3 :
        tmp1 = tmp1 + " dataset." + _data_header[_filter[_filter.size() - 1].idx_col] + "!='" + _filter[_filter.size() - 1].value + "')";;
        break;
      }

      tmp = tmp1;

    } else {
      for (auto itr = _unique_filter.begin(); itr != _unique_filter.end(); ++itr) {

        for (auto itr1 = itr->second.begin(); itr1 != itr->second.end(); ++itr1) {

          string tmp1 = "(";

          if (tmp != "")
          {
            tmp1 = " and (";
          }

          switch (itr1->first)
          {
          case 0 :
            tmp1 = tmp1 + " dataset." + _data_header[itr->first] + "<=" + itr1->second + ")" ;
            break;
          case 1 :
            tmp1 = tmp1 + " dataset." + _data_header[itr->first]  + ">" +  itr1->second + ")";
            break;
          case 2 :
            tmp1 = tmp1  + " dataset." + _data_header[itr->first] + "='" + itr1->second + "')";
            break;
          case 3 :
            tmp1 = tmp1  + " dataset." + _data_header[itr->first] + "!='" + itr1->second + "')";
            break;
          }
          tmp = tmp + tmp1;
        }

      }

    }

  }
  return tmp;
}


void Tdataframe::add_filter(int idx_col, int idx_opt, string value, bool is_filter, bool is_last)
{
  field_filter f;
  f.idx_col = idx_col;
  f.idx_opt = idx_opt;
  f.value = value;
  _filter.push_back(f);

  auto itr = _unique_filter.find(idx_col);
  if (itr == _unique_filter.end()) {
    _unique_filter.insert(make_pair(idx_col, map<int, string> {make_pair(idx_opt, value)}));
  } else {
    auto itr1 = itr->second.find(idx_opt);
    if (itr1 == itr->second.end()) {
      _unique_filter[idx_col].insert(make_pair(idx_opt, value));
    } else {
      _unique_filter[idx_col][idx_opt] = value;
    }
  }

  if (is_filter) {
    string sql = filter_to_query(is_last);
    _data.filter(sql,!is_last);
    stat_tabel(false, is_last, true);
  }

}

void Tdataframe::ReFilter(bool is_last)
{
  string sql = filter_to_query(is_last);
  if (sql != "") {
    _data.filter(sql,!is_last);
  }

  stat_tabel(false, is_last, true);
}

void Tdataframe::clear_map_col_split()
{
  _map_col_split.clear();
}

void Tdataframe::add_filter(field_filter filter, bool is_filter, bool is_last)
{
  _filter.push_back(filter);

  auto itr = _unique_filter.find(filter.idx_col);
  if (itr == _unique_filter.end()) {
    _unique_filter.insert(make_pair(filter.idx_col, map<int, string> {make_pair(filter.idx_opt, filter.value)}));
  } else {
    auto itr1 = itr->second.find(filter.idx_opt);
    if (itr1 == itr->second.end()) {
      _unique_filter[filter.idx_col].insert(make_pair(filter.idx_opt, filter.value));
    } else {
      _unique_filter[filter.idx_col][filter.idx_opt] = filter.value;
    }
  }

  if (is_filter) {
    string sql = filter_to_query(is_last);
    _data.filter(sql,!is_last);
    stat_tabel(false, is_last, true);
  }

}


void Tdataframe::info()
{
  cout << " Info" << endl;
  //cout << " Nama File   : " << _nm_file << endl;
  cout << " Jumlah Data : " << _jml_row << endl;

  cout << _stat_label << endl ;
}

void Tdataframe::split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above)
{
  if (split_value != "-1") {
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


void Tdataframe::calculate_metric(int idx, map<Tmy_dttype, Tlabel_stat>* _col_pot_split, float & current_overall_metric, string & split_value, Tlabel_stat & stat_label)
{
  float entropy_before_split;

  if (!config->use_credal) {
    entropy_before_split = stat_label.get_entropy();
  } else {
    entropy_before_split = stat_label.get_credal_entropy();
  }

  float best_overall_metric = 0.0;
  float gain = 0, gain_max = 0;
  string mid_point = "0.0";
  string tmp_split_value = "-1";

  bool first_iteration = true;

  int jml_row = stat_label.get_jml_row();
  float prosen = 0.0;

  if (_col_pot_split->size() > (0.15 * jml_row))
  {
    prosen = 0.0;
  }

  Tlabel_stat _stat_label_below;

  auto itr_next = _col_pot_split->begin();
  itr_next++;

  auto itr = _col_pot_split->begin();

  size_t i = 1;
  while ((itr != _col_pot_split->end()))
  {
    (*itr).second.set_config(config);
    (*itr_next).second.set_config(config);

    _stat_label_below = _stat_label_below + (*itr).second;
    _stat_label_below.set_config(config);

    if (_stat_label_below.get_jml_row() >= (prosen * i * jml_row)) {
      if (((itr != itr_next) and (itr_next != _col_pot_split->end()) ))
      {

        Tmy_dttype tmp1 = (*itr).first;
        Tmy_dttype tmp2 = (*itr_next).first;
        try {
          mid_point = to_string((stof(tmp1.get_string()) + stof(tmp2.get_string())) / 2);
        }
        catch (const std::invalid_argument& ia) {
          cout << tmp1.get_string() << "+" << tmp2.get_string() << " ";
        }

        Tbelow_above ba(config);
        //ba.set_value(mid_point);

        ba.add_below(_stat_label_below);
        Tlabel_stat tmp_stat = stat_label - _stat_label_below;
        tmp_stat.set_config(config);
        ba.add_above(tmp_stat);

        gain = 0;
        if (ba.cek_valid()) {
          float entropy_after_split = ba.get_overall_metric();
          float split_info = ba.get_split_info();
          gain = (entropy_before_split - entropy_after_split) / split_info;
        }

      }

      if ((first_iteration and (gain > 0)) or (gain_max < gain))
      {
        first_iteration = false;
        gain_max = gain;
        tmp_split_value = mid_point;
        best_overall_metric = gain_max;
      }
      i++;
    }

    itr_next++;
    itr++;


  }

  current_overall_metric = best_overall_metric;
  split_value = tmp_split_value;

}

void Tdataframe::handle_continuous(int idx, float & current_overall_metric, string & split_value)
{
  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  if (_col_pot_split.size() > 0)
  {
    if (_col_pot_split.size() == 1)
    {
      float entropy_before_split;

      if (!config->use_credal) {
        entropy_before_split = _stat_label.get_entropy();
      } else {
        entropy_before_split = _stat_label.get_credal_entropy();
      }

      //cetak("{ ==1 start ");
      auto itr = _col_pot_split.begin();


      Tlabel_stat _stat_label_below = (*itr).second;
      _stat_label_below.set_config(config);

      Tmy_dttype tmp1 = (*itr).first;

      Tbelow_above ba(config);
      ba.set_value(tmp1);
      ba.add_below(_stat_label_below);

      Tlabel_stat tmp_stat = _stat_label - _stat_label_below;
      tmp_stat.set_config(config);
      ba.add_above(tmp_stat);

      float gain = 0;
      if (ba.cek_valid()) {
        float entropy_after_split = ba.get_overall_metric();
        float split_info = ba.get_split_info();
        gain = (entropy_before_split - entropy_after_split) / split_info;// 0;
      }

      if (gain > 0) {
        current_overall_metric = gain;
        split_value = tmp1.get_string();
      }
      //cetak(" ==1 end }");

    } else {
      float tmp_best_overall_metric = 0.0;
      string tmp_split_value = "-1";
      calculate_metric(idx, &_col_pot_split, tmp_best_overall_metric, tmp_split_value, _stat_label);

      current_overall_metric = tmp_best_overall_metric;
      split_value = tmp_split_value;
    }

  }
}

void Tdataframe::handle_non_continuous(int idx, float & current_overall_metric, string & split_value)
{
  float entropy_before_split;

  if (!config->use_credal) {
    entropy_before_split = _stat_label.get_entropy();
  } else {
    entropy_before_split = _stat_label.get_credal_entropy();
  }

  float best_overall_metric = 0.0;
  float gain = 0, gain_max = 0;
  string tmp_split_value = "-1";
  string mid_point = "0.0";

  bool first_iteration = true;

  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  auto itr = _col_pot_split.begin();
  while (itr != _col_pot_split.end())
  {
    Tbelow_above ba(config);

    mid_point = ((Tmy_dttype) (*itr).first).get_string();

    //ba.set_value(mid_point);
    ba.add_below((*itr).second);
    Tlabel_stat tmp_stat = _stat_label - (*itr).second;
    tmp_stat.set_config(config);
    ba.add_above(tmp_stat);

    gain = 0;
    if (ba.cek_valid()) {
      float entropy_after_split = ba.get_overall_metric();
      float split_info = ba.get_split_info();
      gain = (entropy_before_split - entropy_after_split) / split_info;
    }

    itr++;

    if ((first_iteration and (gain > 0)) or (gain_max < gain))
    {
      first_iteration = false;
      gain_max = gain;
      tmp_split_value = mid_point;
      best_overall_metric = gain_max;
    }

  }

  current_overall_metric = best_overall_metric;
  split_value = tmp_split_value;
}

void Tdataframe::calculate_overall_metric(int idx, float & current_overall_metric, string & split_value)
{
  std::lock_guard<std::mutex> lock(v_mutex);
  split_value = "-1";
  current_overall_metric = -1;

  //if (_map_col_split.is_valid_attr(idx)) {

  if (_data_type[idx] == "continuous.") {
    handle_continuous(idx, current_overall_metric, split_value);
  } else {
    handle_non_continuous(idx, current_overall_metric, split_value);
  }
  //}

  //cetak(" split value %s ",split_value.c_str());

  //_map_col_split.clear();
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


float Tdataframe::get_entropy()
{
  return _stat_label.get_entropy();
}