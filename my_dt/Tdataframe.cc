
#include "Tdataframe.h"
#include <iterator>

Tdataframe::Tdataframe()
{

}

Tdataframe::~Tdataframe()
{
  _data_header.clear();
  _data_header.shrink_to_fit();
  _data_type.clear();
  _data_type.shrink_to_fit();
  _filter.clear();
  _stat_label.clear();

}

Tdataframe::Tdataframe(const Tdataframe &t)
{
  cout << "Copy constructor called " << endl;
}



void Tdataframe::read_data(string nm_f)
{
  _data.setnm_f(nm_f);
  _data.setseparator(",");
}

int Tdataframe::getjmlcol() {
  return _jml_col;
}

int Tdataframe::getjmlrow() {
  return _jml_row;
}


void Tdataframe::read_data_type(string nm_f)
{
  Tread_file tmp;
  vector<string> tmp_data;
  tmp.setnm_f(nm_f);
  tmp.setseparator(": ");

  if (tmp.open_file())
  {
    tmp.read_file();
    int i = 1;
    while (!tmp.is_eof())
    { 
      tmp_data = tmp.get_record();
      _data_header.push_back(tmp_data[0]);
      _data_type.push_back(tmp_data[1]);
      tmp.next_record();
      tmp_data.clear();
      tmp_data.shrink_to_fit();
      i++;
    }
    _jml_col = i;
    tmp.close_file();
  } else {
    cout << "Gagal buka "<< nm_f <<" !!!" << endl;
  }

  stat_tabel();
}

map<string, int> Tdataframe::get_unique_value(int idx_col)
{
  map<string, int> tmp;
  vector<string> tmp_data;
  map<string, int>::iterator it;

  if (_data.open_file())
  {
    _data.read_file();
    while (!_data.is_eof())
    {
      tmp_data = _data.get_record();
      if (is_pass(tmp_data))
      {
        if (tmp.size() > 0)
        {
          it = tmp.find(tmp_data[idx_col]);
          if (it == tmp.end())
          {
            tmp.insert(pair<string, int>(tmp_data[idx_col], 1));
          } else {
            //it->second+=1;
          }
        } else {
          tmp.insert(pair<string, int>(tmp_data[idx_col], 1));
        }
      }

      tmp_data.clear();
      tmp_data.shrink_to_fit();
      _data.next_record();
    }

    _data.close_file();
  } else {
    cout << "Gagal buka file !!!" << endl;
  }


  return tmp;
}

void Tdataframe::add_filter(int idx_col, field_filter filter)
{
  _filter.insert(pair<int, field_filter>(idx_col, filter));
  stat_tabel();
}

bool Tdataframe::is_pass(vector<string> &data)
{
  bool pass = true;
  if (_filter.size() > 0)
  {
    unordered_map<int, field_filter>::iterator it = _filter.begin();
    while ((it != _filter.end()) and pass)
    {
      switch (it->second.idx_opt)
      {
      case 0 : {        
        pass = stof(data[it->first]) <= stof(it->second.value);
        break;
      }
      case 1 : {
        
        pass = stof(data[it->first]) > stof(it->second.value);
        break;
      }
      case 2 : {

        pass = data[it->first] == it->second.value;

        break;
      }
      case 3 : {
        pass = data[it->first] != it->second.value;
        break;
      }
      }

      ++it;
    }
  }

  return pass;
}

bool Tdataframe::is_pass(int opt, string value1, string value2)
{
  bool pass = false;

  switch (opt)
  {
  case 0 : {
    //cout << value1 << " " << value2 << endl;
    pass = stof(value1) <= stof(value2);
    break;
  }
  case 1 : {
    //cout << value1 << " " << value2 << endl;
    pass = stof(value1) > stof(value2);
    break;
  }
  case 2 : {
    pass = value1 == value2;
    break;
  }
  case 3 : {
    pass = value1 != value2;
    break;
  }
  }

  return pass;
}

void Tdataframe::split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above)
{
  //cout << "split_data"<<endl;
  if (_data_type[split_column] == "continuous.")
  {
    field_filter f;
    f.idx_opt = 0;
    f.value = split_value;
    data_below.add_filter(split_column, f);
    f.idx_opt = 1;
    data_above.add_filter(split_column, f);

  } else {
    field_filter f;
    f.idx_opt = 2;
    f.value = split_value;
    data_below.add_filter(split_column, f);
    f.idx_opt = 3;
    data_above.add_filter(split_column, f);
  }
}

void Tdataframe::stat_tabel()
{
  //cout << "stat_tabel"<<endl;
  vector<string> tmp_data;
  map<string, int>::iterator it;

  _stat_label.clear();

  int i = 0;
  if (_data.open_file())
  {
    _data.read_file();
    while (!_data.is_eof())
    {
      tmp_data = _data.get_record();
      if (is_pass(tmp_data))
      {
        if (_stat_label.size() > 0)
        {
          it = _stat_label.find(tmp_data[_jml_col]);
          if (it == _stat_label.end())
          {
            //cout << _jml_col << " " << tmp_data[_jml_col] << endl;
            _stat_label.insert(pair<string, int>(tmp_data[_jml_col], 1));
          } else {
            it->second += 1;
          }
        } else {
          //cout << _jml_col << " " << tmp_data[_jml_col] << endl;
          _stat_label.insert(pair<string, int>(tmp_data[_jml_col], 1));
        }

        i++;
      }

      tmp_data.clear();
      tmp_data.shrink_to_fit();
      _data.next_record();
    }

    _data.close_file();
  } else {
    cout << "Gagal buka file !!!" << endl;
  }
  _jml_row = i;
}

map<string, int> Tdataframe::get_stat_label()
{
  return _stat_label;
}



void Tdataframe::get_col_pot_split(int idx, map<string, pot_struct> &_col_pot_split)
{
  //cout << "get_col_pot_split " << idx <<endl;
  map<string, pot_struct>::iterator it;
  map<string, int>::iterator it_pot_struct;
  vector<string> tmp_data;
  //pot_struct p;

  if (_data.open_file())
  {
    _data.read_file();
    while (!_data.is_eof())
    {
      tmp_data = _data.get_record();
      if (is_pass(tmp_data))
      {
        if (_col_pot_split.size() > 0)
        {
          it = _col_pot_split.find(tmp_data[idx]);
          if (it == _col_pot_split.end())
          {
            pot_struct p;
            p.jml = 1;
            p.stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
            _col_pot_split.insert(pair<string, pot_struct>(tmp_data[idx], p));
          } else {
            it->second.jml += 1;

            it_pot_struct = it->second.stat_label.find(tmp_data[_jml_col - 1]);
            if (it_pot_struct == it->second.stat_label.end())
            {
              it->second.stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
            } else {
              it_pot_struct->second += 1;
            }
          }

        } else {
          pot_struct p;
          p.jml = 1;
          p.stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
          _col_pot_split.insert(pair<string, pot_struct>(tmp_data[idx], p));
        }
      }
      tmp_data.clear();
      tmp_data.shrink_to_fit();
      _data.next_record();
    }
    _data.close_file();




  } else {
    cout << "Gagal buka file !!!" << endl;
  }
}

void Tdataframe::calculate_overall_metric(int idx, map<string, pot_struct> &_col_pot_split, float &current_overall_metric, string &split_value)
{
  //cout << "calculate_overall_metric "<< idx <<endl;
  float best_overall_metric = 999;
  string tmp_split_value;
  map<string, pot_struct>::iterator it, it1;
  map<string, int>::iterator it_pot_struct, it_pot_struct1;
  bool is_below;
  bool first_iteration = true;
  float p_dt_above, p_dt_below, entropy_below, entropy_above, overall_metric;
  int jml;

  int _jml_row_above;
  int _jml_row_below;

  map<string, int> _stat_label_above;
  map<string, int> _stat_label_below;

  if (_col_pot_split.size() > 0)
  {
    for (it = _col_pot_split.begin(); it != _col_pot_split.end(); ++it)
    {
      _jml_row_below = 0;
      _jml_row_above = 0;

      _stat_label_below.clear();
      _stat_label_above.clear();


      for (it1 = _col_pot_split.begin(); it1 != _col_pot_split.end(); ++it1)
      {
        if (_data_type[idx] == "continuous.")
        {
          is_below =  stof(it1->first) <= stof(it->first) ;
        } else {
          is_below =  it1->first == it->first ;
        }

        if (is_below)
        {
          _jml_row_below += it1->second.jml;


          it_pot_struct = it1->second.stat_label.begin();
          while (it_pot_struct != it1->second.stat_label.end())
          {

            it_pot_struct1 = _stat_label_below.find(it_pot_struct->first);
            if (it_pot_struct1 == _stat_label_below.end())
            {
              _stat_label_below.insert(pair<string, int>(it_pot_struct->first, it_pot_struct->second));
            } else {
              it_pot_struct1->second += it_pot_struct->second;
            }

            it_pot_struct++;
          }

        } else {
          _jml_row_above += it1->second.jml;


          it_pot_struct = it1->second.stat_label.begin();
          while (it_pot_struct != it1->second.stat_label.end())
          {
            it_pot_struct1 = _stat_label_above.find(it_pot_struct->first);
            if (it_pot_struct1 == _stat_label_above.end())
            {
              _stat_label_above.insert(pair<string, int>(it_pot_struct->first, it_pot_struct->second));
            } else {
              it_pot_struct1->second += it_pot_struct->second;
            }

            it_pot_struct++;
          }

        }
      }

      jml = _jml_row_below + _jml_row_below;
      p_dt_below = (float) _jml_row_below / jml;
      p_dt_above = (float) _jml_row_above / jml;

      entropy_below = 0;
      it_pot_struct = _stat_label_below.begin();
      while (it_pot_struct != _stat_label_below.end())
      {
        entropy_below += ((float) it_pot_struct->second / _jml_row_below)  * (-1 * log2((float) it_pot_struct->second / _jml_row_below));
        it_pot_struct++;
      }

      entropy_above = 0;
      it_pot_struct = _stat_label_above.begin();
      while (it_pot_struct != _stat_label_above.end())
      {
        entropy_above += ((float) it_pot_struct->second / _jml_row_above)  * (-1 * log2((float) it_pot_struct->second / _jml_row_above));
        it_pot_struct++;
      }

      overall_metric = (p_dt_below * entropy_below) + (p_dt_above * entropy_above);

      if (first_iteration or (overall_metric <= best_overall_metric))
      {
        first_iteration = false;

        best_overall_metric = overall_metric;
        tmp_split_value = it->first;

        //cout << current_overall_metric << endl;
        //cout << split_column << endl;
        //cout << split_value << endl;
      }

    }

    current_overall_metric = best_overall_metric;
    split_value = tmp_split_value;

    if (_col_pot_split.size() > 0)
    {
      map<string, pot_struct>::iterator it = _col_pot_split.begin();
      while (it != _col_pot_split.end())
      {
        //pot_struct *p;
        it->second.stat_label.clear();
        //p=&it->second;
        //delete p;
        //*it->second = NULL;
        it++;
      }

      _col_pot_split.clear();
    }
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

void Tdataframe::write_data(vector<string> &data)
{
  if (data.size())
  {
    if (_data.open_file("w+"))
    {
      for (int i = 0; i < data.size(); ++i)
      {
        _data.write_file(data[i]);
      }

      _data.close_file();

    }
  }
}

bool Tdataframe::open_file()
{
  return _data.open_file();
}

void Tdataframe::read_file()
{
  _data.read_file();
}

void Tdataframe::close_file()
{
  _data.close_file();
}

bool Tdataframe::is_eof()
{
  return _data.is_eof();
}

void Tdataframe::next_record()
{
  _data.next_record();
}

vector<string> Tdataframe::get_record()
{
  return _data.get_record();
}