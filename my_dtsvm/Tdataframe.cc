
#include "Tdataframe.h"
#include <iterator>

Tdataframe::Tdataframe()
{

}

Tdataframe::~Tdataframe()
{

}

void Tdataframe::split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above)
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

void Tdataframe::get_col_pot_split(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split)
{
  //cetak(" get_col_pot_split ");
  //cout << "get_col_pot_split " << idx <<endl;
  // map<Tmy_dttype, Tlabel_stat>::iterator it;
  // map<string, int>::iterator it_pot_struct;
  // vector<string> tmp_data;
  //pot_struct p;

  /*_data.reset_file();
  while (!_data.is_eof())
  {
    // tmp_data = _data.get_record();

    Tmy_dttype pot_split_holder(_data.get_col_val(idx), _data_type[idx] == "continuous.");

    it = _col_pot_split.find(pot_split_holder);
    if (it == _col_pot_split.end())
    {
      Tlabel_stat p(_credal_s);
      p.add(_data.get_col_val(_idx_label));
      _col_pot_split.insert(pair<Tmy_dttype, Tlabel_stat>(pot_split_holder, p));
    } else {
      it->second.add(_data.get_col_val(_idx_label));
    }

    // tmp_data.clear();
    // tmp_data.shrink_to_fit();
    _data.next_record();
  }*/
}


void Tdataframe::calculate_metric(size_t start, size_t end, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value, Tlabel_stat & stat_label, Tconfig v_config)
{

  float entropy_before_split;

  if (!v_config.use_credal) {
    entropy_before_split = stat_label.get_entropy();
  } else {
    entropy_before_split = stat_label.get_credal_entropy();
  }

  float best_overall_metric = 0.0;
  float gain = 0, gain_max = 0;
  string mid_point = "0.0";
  string tmp_split_value = "-1";

  bool first_iteration = true;

  Tlabel_stat _stat_label_below;


  auto itr_next = _col_pot_split.begin();
  itr_next++;

  auto itr = _col_pot_split.begin();

  size_t i = 1;
  while ((itr != _col_pot_split.end()) and (i <= end))
  {
    _stat_label_below = _stat_label_below + (*itr).second;
    _stat_label_below.set_config(v_config);

    if ( (i >= start) and ((itr != itr_next) and (itr_next != _col_pot_split.end()) ))
    {
      Tmy_dttype tmp1 = (*itr).first;
      Tmy_dttype tmp2 = (*itr_next).first;
      try {
        mid_point = to_string((stof(tmp1.get_string()) + stof(tmp2.get_string())) / 2);
      }
      catch (const std::invalid_argument& ia) {
        cout << tmp1.get_string() << "+" << tmp2.get_string() << " ";

      }

      Tbelow_above ba;
      ba.set_config(v_config);
      //ba.set_value(mid_point);

      ba.add_below(_stat_label_below);
      Tlabel_stat tmp_stat = stat_label - _stat_label_below;
      tmp_stat.set_config(v_config);
      ba.add_above(tmp_stat);

      gain = 0;
      if (ba.cek_valid()) {
        float entropy_after_split = ba.get_overall_metric();
        float split_info = ba.get_split_info();
        gain = (entropy_before_split - entropy_after_split) / split_info;
      }

    }

    itr_next++;
    itr++;
    i++;

    if ((first_iteration and (gain >= 0)) or (gain_max < gain))
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

void Tdataframe::handle_continuous(map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value)
{
  if (_col_pot_split.size() > 0)
  {


    if (_col_pot_split.size() == 1)
    {

      float entropy_before_split;

      if (!config.use_credal) {
        entropy_before_split = _stat_label.get_entropy();
      } else {
        entropy_before_split = _stat_label.get_credal_entropy();
      }

      //cetak("{ ==1 start ");
      auto itr = _col_pot_split.begin();

      
      Tlabel_stat _stat_label_below=(*itr).second;
      _stat_label_below.set_config(config);

      Tmy_dttype tmp1 = (*itr).first;

      Tbelow_above ba;
      ba.set_config(config);
      ba.set_value(tmp1);
      ba.add_below(_stat_label_below);

      Tlabel_stat tmp_stat = _stat_label - _stat_label_below;
      tmp_stat.set_config(config);
      ba.add_above(tmp_stat);

      float entropy_after_split = ba.get_overall_metric();
      float split_info = ba.get_split_info();
      float gain = (entropy_before_split - entropy_after_split) / split_info;// 0; 

      if (gain >= 0) {
        current_overall_metric = gain;
        split_value = tmp1.get_string();
      }
      //cetak(" ==1 end }");

    } else {
      //cetak("{ ==2 start ");

      const int dt_per_page = 1000;

      if (_col_pot_split.size() < dt_per_page)
      {
        float tmp_best_overall_metric = 0.0;
        string tmp_split_value = "-1";
        thread t1(&Tdataframe::calculate_metric, 1, _col_pot_split.size(), ref(_col_pot_split), ref(tmp_best_overall_metric), ref(tmp_split_value), ref(_stat_label), config);
        t1.join();

        current_overall_metric = tmp_best_overall_metric;
        split_value = tmp_split_value;

      } else {

        size_t jml_loop = _col_pot_split.size() / dt_per_page;

        if ((jml_loop * dt_per_page) < _col_pot_split.size())
        {
          jml_loop += 1;
        }

        const int jml_thread = 10;

        thread th[jml_thread];
        float arr_gain[jml_thread];
        string arr_split_value[jml_thread];


        float gain_max = 0;
        string tmp_split_value = "-1";

        // cetak("{ idx:");
        // cetak(to_string(idx).c_str());
        // cetak(" loop:");
        // cetak(to_string(jml_loop).c_str());
        // cetak(" ");

        size_t i = 0;
        while (i < jml_loop)
        {
          size_t j = 0;
          size_t k = i;
          bool cthread = true;
          while ((j < jml_thread) and cthread)
          {
            size_t _begin = (k * dt_per_page) + 1;
            size_t _end = (dt_per_page * (k + 1)) > _col_pot_split.size() ? _col_pot_split.size() : (dt_per_page * (k + 1));

            if (_begin <= _col_pot_split.size()) {
              // cetak("{start th:");
              // cetak(to_string(j).c_str());
              th[j] = thread(&Tdataframe::calculate_metric, _begin , _end, ref(_col_pot_split), ref(arr_gain[j]), ref(arr_split_value[j]), ref(_stat_label), config);
              j++;
              // cetak("} ");
            } else {
              cthread = false;
            }

            k++;
          }

          i += j;

          size_t l = 0;
          while (l < j)
          {

            // cetak("{end th:");
            // cetak(to_string(l).c_str());
            th[l].join();
            // cetak("} ");

            if (gain_max < arr_gain[l])
            {
              gain_max = arr_gain[l];
              tmp_split_value = arr_split_value[l];
            }
            l++;
          }
        }


        current_overall_metric = gain_max;
        split_value = tmp_split_value;

        // cetak("}");

      }

      //cetak(" ==2 end }");
    }

    // auto it = _col_pot_split.begin();
    // while (it != _col_pot_split.end())
    // {
    //   it->second.clear();
    //   it++;
    // }

    // _col_pot_split.clear();

  }
}

void Tdataframe::handle_non_continuous(map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value)
{
  float entropy_before_split;

  if (!config.use_credal) {
    entropy_before_split = _stat_label.get_entropy();
  } else {
    entropy_before_split = _stat_label.get_credal_entropy();
  }

  float best_overall_metric = 0.0;
  float gain = 0, gain_max = 0;
  string tmp_split_value = "-1";
  string mid_point = "0.0";

  bool first_iteration = true;

  auto itr = _col_pot_split.begin();
  while (itr != _col_pot_split.end())
  {
    Tbelow_above ba;
    ba.set_config(config);

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

  // auto it = _col_pot_split.begin();
  // while (it != _col_pot_split.end())
  // {
  //   it->second.clear();
  //   it++;
  // }

  // _col_pot_split.clear();

}

void Tdataframe::calculate_overall_metric(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value)
{
  //cetak(" calculate_overall_metric ");
  //cout << "          calculate_overall_metric " << get_nm_header(idx) << endl;
  if (_data_type[idx] == "continuous.") {
    handle_continuous(_col_pot_split, current_overall_metric, split_value);
  } else {
    handle_non_continuous(_col_pot_split, current_overall_metric, split_value);
  }
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