
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
    async_worker.push_back(async(std::launch::async, &Tdataframe::get_pot_split, global_config.id_dt_train, global_config.jns_dt_train, global_config.partition_train, _posisi_cabang, i));

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


    Tmy_dttype mid_point("0.0", true), max_gain_ratio("0.0", true), max_gain("0.0", true);
    Tmy_dttype tmp_split_value("-1", true);
    bool first_iteration = true;
    Tlabel_stat _stat_label_below;
    int max_jml_below = 0, max_jml_above = 0;

    auto itr_next = _col_pot_split.begin();
    itr_next++;

    auto itr = _col_pot_split.begin();
    while ((itr != _col_pot_split.end()))
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

        if (((itr != itr_next) and (itr_next != _col_pot_split.end()) ))
        {

          Tmy_dttype tmp1 = (*itr).first;
          Tmy_dttype tmp2 = (*itr_next).first;
          Tmy_dttype tmp = (tmp1 + tmp2) / 2.0;
          mid_point = tmp;

          Tbelow_above ba;

          ba.add_below(_stat_label_below);
          Tlabel_stat tmp_stat = _stat_label - _stat_label_below;
          ba.add_above(tmp_stat);

          Tmy_dttype gain("0.0", true);
          Tmy_dttype gain_ratio("0.0", true);

          //if (ba.cek_valid()) {
          Tmy_dttype entropy_after_split = ba.get_overall_metric();
          float split_info = ba.get_split_info();
          gain = entropy_before_split - entropy_after_split;
          if (abs(split_info) > 0) {
            gain_ratio = gain / split_info;
          }
          //}


          bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

          if ((first_iteration and is_pass) or ((max_gain_ratio < gain_ratio) and is_pass))
          {
            first_iteration = false;
            tmp_split_value = mid_point;
            max_gain_ratio = gain_ratio;
            max_gain = gain;
            max_jml_below = _stat_label_below.get_jml_row();
            max_jml_above = tmp_stat.get_jml_row();
          }



        }

      }

      itr_next++;
      itr++;
    }

    hsl.idx = idx;
    hsl.max_gain_ratio = stof(max_gain_ratio.get_string());
    hsl.max_gain = stof(max_gain.get_string());
    hsl.split_value = tmp_split_value;
    hsl.jml_below = max_jml_below;
    hsl.jml_above = max_jml_above;


    // float tmp_best_overall_metric = 0.0;
    // Tmy_dttype tmp_split_value;
    // calculate_metric(idx, &_col_pot_split, tmp_best_overall_metric, tmp_split_value, _stat_label);

    // current_overall_metric = tmp_best_overall_metric;
    // split_value = tmp_split_value;


  }

  _col_pot_split.clear();
  return hsl;

}

void Tdataframe::gen_kombinasi(map<Tmy_dttype, Tlabel_stat> v_col_pot_split, int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below, Tmetric_split_value& v_split)
{
  auto itr = v_col_pot_split.begin();

  for (int i = 0; i < geser; ++i)
  {
    itr++;
  }

  if (counter == depth)
  {
    Tmy_dttype entropy_before_split;

    if (!global_config.use_credal) {
      entropy_before_split = _stat_label.get_entropy();
    } else {
      entropy_before_split = _stat_label.get_credal_entropy();
    }

    Tmy_dttype  max_gain_ratio("0.0", true), max_gain("0.0", true);
    Tmy_dttype tmp_split_value("-1", false);
    bool first_iteration = true;

    for (auto itr1 = itr; itr1 != v_col_pot_split.end(); ++itr1)
    {
      Tmy_dttype mid_point;
      mid_point.set_value(v_mid_point + ';' + ((Tmy_dttype) (*itr1).first).get_string(), false);

      Tlabel_stat stat_below = v_stat_below + (*itr).second;
      Tlabel_stat stat_above = _stat_label - stat_below;

      Tbelow_above tmp_ba;
      tmp_ba.add_below(stat_below);
      tmp_ba.add_above(stat_above);

      Tmy_dttype gain("0.0", true);
      Tmy_dttype gain_ratio("0.0", true);

      if (tmp_ba.cek_valid()) {
        Tmy_dttype entropy_after_split = tmp_ba.get_overall_metric();
        float split_info = tmp_ba.get_split_info();
        gain = entropy_before_split - entropy_after_split;
        if (abs(split_info) > 0) {
          gain_ratio = gain / split_info;
        }
      }

      bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

      if ((first_iteration and is_pass) or ((max_gain_ratio < gain_ratio) and is_pass))
      {
        first_iteration = false;
        tmp_split_value = mid_point;
        max_gain_ratio = gain_ratio;
        max_gain = gain;
      }
    }

    if (v_split.max_gain_ratio < stof(max_gain_ratio.get_string()))
    {
      v_split.max_gain_ratio = stof(max_gain_ratio.get_string());
      v_split.max_gain = stof(max_gain.get_string());
      v_split.split_value = tmp_split_value;
    }



  } else {
    counter++;
    int tmp_geser = geser;
    for (auto itr2 = itr; itr2 != v_col_pot_split.end(); ++itr2)
    {
      Tlabel_stat tmp_v_stat_below = v_stat_below + (*itr2).second;
      tmp_geser++;
      string tmp_v_mid_point = ((Tmy_dttype) (*itr2).first).get_string();
      if (v_mid_point != "") {
        tmp_v_mid_point = v_mid_point + ';' + ((Tmy_dttype) (*itr2).first).get_string();
      }
      gen_kombinasi(v_col_pot_split, counter, depth, tmp_geser, tmp_v_mid_point, tmp_v_stat_below, v_split);
    }

  }


}

void Tdataframe::gen_kombinasi_normal(map<Tmy_dttype, Tlabel_stat> v_col_pot_split, int counter, int depth, int geser, string v_mid_point, Tlabel_stat v_stat_below, Tmetric_split_value& v_split)
{
  auto itr = v_col_pot_split.begin();

  for (int i = 0; i < geser; ++i)
  {
    itr++;
  }

  if (counter == depth)
  {
    Tmy_dttype entropy_before_split;

    if (!global_config.use_credal) {
      entropy_before_split = _stat_label.get_entropy();
    } else {
      entropy_before_split = _stat_label.get_credal_entropy();
    }

    Tmy_dttype  max_gain_ratio("0.0", true), max_gain("0.0", true);
    Tmy_dttype tmp_split_value("-1", false);
    bool first_iteration = true;

    for (auto itr1 = itr; itr1 != v_col_pot_split.end(); ++itr1)
    {
      Tmy_dttype mid_point;
      mid_point.set_value(v_mid_point + ';' + ((Tmy_dttype) (*itr1).first).get_string(), false);

      Tlabel_stat stat_below = v_stat_below + (*itr).second;
      Tlabel_stat stat_above = _stat_label - stat_below;

      if (stat_below.get_max_label()=="normal")
      {
        Tbelow_above tmp_ba;
        tmp_ba.add_below(stat_below);
        tmp_ba.add_above(stat_above);

        Tmy_dttype gain("0.0", true);
        Tmy_dttype gain_ratio("0.0", true);

        if (tmp_ba.cek_valid()) {
          Tmy_dttype entropy_after_split = tmp_ba.get_overall_metric();
          float split_info = tmp_ba.get_split_info();
          gain = entropy_before_split - entropy_after_split;
          if (abs(split_info) > 0) {
            gain_ratio = gain / split_info;
          }
        }

        bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

        if ((first_iteration and is_pass) or ((max_gain_ratio < gain_ratio) and is_pass))
        {
          first_iteration = false;
          tmp_split_value = mid_point;
          max_gain_ratio = gain_ratio;
          max_gain = gain;
        }
      }

    }

    if (v_split.max_gain_ratio < stof(max_gain_ratio.get_string()))
    {
      v_split.max_gain_ratio = stof(max_gain_ratio.get_string());
      v_split.max_gain = stof(max_gain.get_string());
      v_split.split_value = tmp_split_value;
    }



  } else {
    counter++;
    int tmp_geser = geser;
    for (auto itr2 = itr; itr2 != v_col_pot_split.end(); ++itr2)
    {
      Tlabel_stat tmp_v_stat_below = v_stat_below + (*itr2).second;
      tmp_geser++;
      if (tmp_v_stat_below.get_max_label() == "normal")
      {
        string tmp_v_mid_point = ((Tmy_dttype) (*itr2).first).get_string();
        if (v_mid_point != "") {
          tmp_v_mid_point = v_mid_point + ';' + ((Tmy_dttype) (*itr2).first).get_string();
        }
        gen_kombinasi_normal(v_col_pot_split, counter, depth, tmp_geser, tmp_v_mid_point, tmp_v_stat_below, v_split);
      }

    }

  }


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

  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  Tbelow_above_kategori ba;
  Tmy_dttype mid_point("-1", false), max_gain_ratio("0.0", true), max_gain("0.0", true);
  Tmy_dttype tmp_split_value("-1", false);
  bool first_iteration = true;

  auto itr = _col_pot_split.begin();
  while (itr != _col_pot_split.end())
  {
    mid_point = ((Tmy_dttype) (*itr).first);

    ba.add_stat((*itr).second);

    Tlabel_stat stat_below, stat_above;
    stat_below = (*itr).second;
    stat_above = _stat_label - stat_below;

    Tbelow_above tmp_ba;
    tmp_ba.add_below(stat_below);
    tmp_ba.add_above(stat_above);

    Tmy_dttype gain("0.0", true);
    Tmy_dttype gain_ratio("0.0", true);

    //if (tmp_ba.cek_valid()) {
    Tmy_dttype entropy_after_split = tmp_ba.get_overall_metric();
    float split_info = tmp_ba.get_split_info();
    gain = entropy_before_split - entropy_after_split;
    if (abs(split_info) > 0) {
      gain_ratio = gain / split_info;
    }
    //}

    bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

    if ((first_iteration and is_pass) or ((max_gain_ratio < gain_ratio) and is_pass))
    {
      first_iteration = false;
      tmp_split_value = mid_point;
      max_gain_ratio = gain_ratio;
      max_gain = gain;
    }



    itr++;
  }

  Tmy_dttype entropy_after_split = ba.get_overall_metric();
  float split_info = ba.get_split_info();

  Tmy_dttype gain, gain_ratio;
  gain.set_value("0.0", true);
  gain_ratio.set_value("0.0", true);
  gain = (entropy_before_split - entropy_after_split);
  if (split_info > 0.0) {
    gain_ratio = gain / split_info;
  }


  if (max_gain_ratio < gain_ratio) {

    size_t jml_kombinasi = 2;
    while ((_col_pot_split.size() > (jml_kombinasi * 2)))
    {

      if (jml_kombinasi < 6)
      {
        Tlabel_stat tmp_stat;
        Tmetric_split_value tmp_split;

        gen_kombinasi(_col_pot_split, 0, jml_kombinasi - 1, 0, "", tmp_stat, tmp_split);

        if (max_gain_ratio < tmp_split.max_gain_ratio)
        {
          tmp_split_value = tmp_split.split_value;
          max_gain_ratio.set_value(to_string(tmp_split.max_gain_ratio), true);
          max_gain.set_value(to_string(tmp_split.max_gain), true);
        }
      }

      jml_kombinasi++;
    }

  }

  if (max_gain_ratio != "0.0") {
    hsl.idx = idx;
    hsl.max_gain_ratio = stof(gain_ratio.get_string());
    hsl.max_gain = stof(gain.get_string());
    hsl.split_value = tmp_split_value;
  }

  return hsl;

}



Tmetric_split_value Tdataframe::handle_non_continuous_1(int idx)
{
  map<Tmy_dttype, Tlabel_stat> _col_pot_split = _map_col_split.get_pot_split(idx);

  Tmetric_split_value hsl;
  if (_col_pot_split.size() > 0) {

    Tmy_dttype entropy_before_split;

    if (!global_config.use_credal) {
      entropy_before_split = _stat_label.get_entropy();
    } else {
      entropy_before_split = _stat_label.get_credal_entropy();
    }


    Tmy_dttype mid_point("-1", false), max_gain_ratio("0.0", true), max_gain("0.0", true);
    Tmy_dttype tmp_split_value("-1", false);
    bool first_iteration = true;
    Tlabel_stat _stat_label_below;
    int max_jml_below = 0, max_jml_above = 0;

    auto itr = _col_pot_split.begin();
    while (itr != _col_pot_split.end())
    {
      Tmy_dttype mid_point = ((Tmy_dttype) (*itr).first);

      Tlabel_stat stat_below, stat_above;
      stat_below = (*itr).second;
      stat_above = _stat_label - stat_below;

      if (stat_below.get_max_label() == "normal") {

        Tbelow_above ba;
        ba.add_below(stat_below);
        ba.add_above(stat_above);

        Tmy_dttype gain("0.0", true);
        Tmy_dttype gain_ratio("0.0", true);

        //if (ba.cek_valid()) {
        Tmy_dttype entropy_after_split = ba.get_overall_metric();
        float split_info = ba.get_split_info();
        gain = entropy_before_split - entropy_after_split;
        if (abs(split_info) > 0) {
          gain_ratio = gain / split_info;
        }
        //}

        bool is_pass = global_config.use_credal ? true : (gain_ratio > 0.0);

        if ((first_iteration and is_pass) or ((max_gain_ratio < gain_ratio) and is_pass))
        {
          first_iteration = false;
          tmp_split_value = mid_point;
          max_gain_ratio = gain_ratio;
          max_gain = gain;
          max_jml_above = stat_below.get_jml_row();
          max_jml_below = stat_above.get_jml_row();
        }

      }

      itr++;
    }


    size_t jml_kombinasi = 2;
    while ((_col_pot_split.size() > (jml_kombinasi * 2)))
    {

      if (jml_kombinasi < 10)
      {
        Tlabel_stat tmp_stat;
        Tmetric_split_value tmp_split;

        gen_kombinasi_normal(_col_pot_split, 0, jml_kombinasi - 1, 0, "", tmp_stat, tmp_split);

        if (max_gain_ratio < tmp_split.max_gain_ratio)
        {
          tmp_split_value = tmp_split.split_value;
          max_gain_ratio.set_value(to_string(tmp_split.max_gain_ratio), true);
          max_gain.set_value(to_string(tmp_split.max_gain), true);
        }
      }

      jml_kombinasi++;
    }


    hsl.idx = idx;
    hsl.max_gain_ratio = stof(max_gain_ratio.get_string());
    hsl.max_gain = stof(max_gain.get_string());
    hsl.split_value = tmp_split_value;
    hsl.jml_below = max_jml_below;
    hsl.jml_above = max_jml_above;

  }

  _col_pot_split.clear();
  return hsl;
}

Tmetric_split_value Tdataframe::calculate_overall_metric(int idx)
{
  std::lock_guard<std::mutex> lock(v_mutex);
  Tmetric_split_value hsl;
  if (_data_type[idx] == "continuous.") {
    hsl = handle_continuous(idx);
  } else {
    hsl = handle_non_continuous_1(idx);
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