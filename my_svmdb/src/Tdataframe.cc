
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

}

Tdataframe::~Tdataframe()
{
  _stat_label.clear();
  _data.set_child(_child_depth, _child_branch, _child_branch_number);
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
    _stat_label.set_config(config);
  }

}

map<string, int> Tdataframe::get_stat_label()
{
  return _stat_label.get_map();
}



string Tdataframe::get_max_label()
{
  return _stat_label.get_max_label();
}

bool Tdataframe::is_single_label()
{
  return _stat_label.is_single_label();
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

  _data.read_hsl_filter();

  vector<vector<string>> Table;

  vector<string> tmp_data;

  _data.reset_file();
  while (!_data.is_eof())
  {
    //cout << " get_all_record_svm get_record_svm " << endl;
    tmp_data = get_record_svm();

    //bool is_pass = (config->normal_only ? (tmp_data[tmp_data.size() - 1].compare("normal") == 0) : true);
    //if (is_pass) {
    Table.push_back(tmp_data);
    //}
    //cout << " get_all_record_svm next_record " << endl;
    _data.next_record();
  }

  return Table;
}



map<string, vector<string>> Tdataframe::get_all_record_svm_map()
{
  //std::lock_guard<std::mutex> lock(v_mutex);
  // ReFilter();

  _data.read_hsl_filter();

  map<string, vector<string>> Table;

  _data.reset_file();
  while (!_data.is_eof())
  {
    //cout << " get_all_record_svm get_record_svm " << endl;
    string record_id = _data.get_id_row();
    vector<string> tmp_data = get_record_svm();

    auto itr = Table.find(record_id);
    if (itr == Table.end()) {
      Table.insert({record_id, tmp_data});
    } else {
      cetak(" record ganda !!! \n ");
    }

    //bool is_pass = (config->normal_only ? (tmp_data[tmp_data.size() - 1].compare("normal") == 0) : true);
    //if (is_pass) {
    //Table.push_back(tmp_data);
    //}
    //cout << " get_all_record_svm next_record " << endl;
    _data.next_record();
  }

  return Table;
}



// void Tdataframe::set_config(Tconfig v_config)
// {
//   config = v_config;
//   _stat_label.set_config(config);
//   _map_col_split.set_config(config);
// }

void Tdataframe::add_filter(int idx_col, int idx_opt, string value, bool is_filter, bool is_last)
{
  field_filter f;
  f.idx_col = idx_col;
  f.idx_opt = idx_opt;
  f.value = value;
  _filter.push_back(f);


  if (is_filter) {
    string sql = filter_to_query(is_last);
    _data.filter(sql, !is_last);
    stat_tabel(false, is_last, true);
  }

}

void Tdataframe::ReFilter(bool is_last)
{
  string sql = filter_to_query(is_last);
  if (sql != "") {
    _data.filter(sql, !is_last);
  }

  stat_tabel(false, is_last, true);
}



void Tdataframe::add_filter(field_filter filter, bool is_filter, bool is_last)
{
  _filter.push_back(filter);

  if (is_filter) {
    string sql = filter_to_query(is_last);
    _data.filter(sql, !is_last);
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

void Tdataframe::dtsvm_stat(time_t id_experiment,time_t id_detail_experiment,time_t id_experiment_dt,time_t id_detail_experiment_dt)
{
  _data.dtsvm_stat(id_experiment,id_detail_experiment,id_experiment_dt,id_detail_experiment_dt);
}

void Tdataframe::detail_dtsvm_stat(time_t id_experiment,time_t id_detail_experiment,time_t id_more_detail_experiment,time_t id_experiment_dt,time_t id_detail_experiment_dt,int no_svm)
{
  _data.detail_dtsvm_stat(id_experiment,id_detail_experiment,id_more_detail_experiment,id_experiment_dt,id_detail_experiment_dt,no_svm);
}

void Tdataframe::dtsvm_conf_metrix(time_t id_experiment,time_t id_detail_experiment,time_t id_experiment_dt,time_t id_detail_experiment_dt,Tconf_metrix &tmp_conf_metrix)
{
  _data.dtsvm_conf_metrix(id_experiment,id_detail_experiment,id_experiment_dt,id_detail_experiment_dt,tmp_conf_metrix); 
}


