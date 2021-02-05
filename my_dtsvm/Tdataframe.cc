
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

static void cetak_stdout(const char *s)
{
  fputs(s, stdout);
  fflush(stdout);
}

void Tdataframe::cetak ( const char * format, ... )
{
  char buffer[256];
  va_list args;
  va_start (args, format);
  vsprintf (buffer, format, args);
  //perror (buffer);
  va_end (args);
  cetak_stdout(buffer);
}



void Tdataframe::read_data(string nm_f)
{
  _nm_file = nm_f;
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
    int i = 0;
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
    cout << "Gagal buka " << nm_f << " !!!" << endl;
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
    cout << "get_unique_value, Gagal buka file !!!" << endl;
  }


  return tmp;
}

void Tdataframe::add_filter(field_filter filter)
{
  _data.index_off();
  is_index = true;
  _filter.push_back(filter);
  stat_tabel();
  is_index = false;
  _data.index_on();
}

void Tdataframe::ReFilter()
{
  _data.index_off();
  is_index = true;
  stat_tabel();
  is_index = false;
  _data.index_on();
}

vector<field_filter> Tdataframe::get_filter()
{
  return _filter;
}

void Tdataframe::to_lower(string &str)
{
  for (int i = 0; i < str.length(); ++i)
  {
    str[i] = toupper(str[i]);
  }
}

bool Tdataframe::is_pass(vector<string> &data)
{
  bool pass = true;
  if (_filter.size() > 0)
  {
    vector<field_filter>::iterator it = _filter.begin();
    while ((it != _filter.end()) and pass)
    {

      switch (it->idx_opt)
      {
      case 0 : {
        pass = stof(data[it->idx_col]) <= stof(it->value);
        break;
      }
      case 1 : {

        pass = stof(data[it->idx_col]) > stof(it->value);
        break;
      }
      case 2 : {
        string first_str = data[it->idx_col];
        string second_str = it->value;

        to_lower(first_str);
        to_lower(second_str);

        pass = first_str == second_str;

        break;
      }
      case 3 : {
        string first_str = data[it->idx_col];
        string second_str = it->value;

        to_lower(first_str);
        to_lower(second_str);

        pass = first_str != second_str;
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

    string first_str = value1;
    string second_str = value2;

    to_lower(first_str);
    to_lower(second_str);

    pass = first_str == second_str;
    break;
  }
  case 3 : {

    string first_str = value1;
    string second_str = value2;

    to_lower(first_str);
    to_lower(second_str);

    pass = first_str != second_str;
    break;
  }
  }

  return pass;
}

void Tdataframe::split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above)
{
  _data.clear_memory();
  //cout << "split_data"<<endl;
  if (_data_type[split_column] == "continuous.")
  {
    field_filter f;
    f.idx_col = split_column;
    f.idx_opt = 0;
    f.value = split_value;
    data_below.add_filter(f);
    f.idx_opt = 1;
    data_above.add_filter(f);

  } else {
    field_filter f;
    f.idx_col = split_column;
    f.idx_opt = 2;
    f.value = split_value;
    data_below.add_filter(f);
    f.idx_opt = 3;
    data_above.add_filter(f);
  }
}

void Tdataframe::stat_tabel()
{
  //cout << "stat_tabel"<<endl;
  vector<string> tmp_data;
  map<string, int>::iterator it;

  _stat_label.clear();

  _data.file_map();

  if (is_index) {
    _data.clear_index();
  }

  int i = 0;
  if (_data.open_file())
  {
    _data.read_file();
    while (!_data.is_eof())
    {
      tmp_data = _data.get_record();

      if (is_pass(tmp_data))
      {

        //cout << "is_pass" << endl;
        if (is_index) {
          _data.add_index();
        }

        if (_stat_label.size() > 0)
        {
          it = _stat_label.find(tmp_data[_jml_col - 1]);
          if (it == _stat_label.end())
          {
            //cout << _jml_col << " " << tmp_data[_jml_col-1] << endl;
            _stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
          } else {
            it->second += 1;
          }
        } else {
          //cout << "data baru" << _jml_col << tmp_data.size() << endl;
          //cout << _jml_col << " " << tmp_data[_jml_col-1] << endl;
          _stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
        }

        i++;
      }

      tmp_data.clear();
      tmp_data.shrink_to_fit();
      _data.next_record();
    }

    if (is_index) {
      _data.save_to_memory();
      _data.clear_index();
    }

    _data.close_file();
  } else {
    cout << "stat_tabel, Gagal buka file !!!" << endl;
  }
  _jml_row = i;
  //cout << _jml_row << endl ;
}

map<string, int> Tdataframe::get_stat_label()
{
  return _stat_label;
}



void Tdataframe::get_col_pot_split(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split)
{
  //cout << "get_col_pot_split " << idx <<endl;
  map<Tmy_dttype, Tlabel_stat>::iterator it;
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

        Tmy_dttype pot_split_holder(tmp_data[idx], _data_type[idx] == "continuous.");

        if (_col_pot_split.size() > 0)
        {
          it = _col_pot_split.find(pot_split_holder);
          if (it == _col_pot_split.end())
          {
            Tlabel_stat p;
            p.add(tmp_data[_jml_col - 1]);
            _col_pot_split.insert(pair<Tmy_dttype, Tlabel_stat>(pot_split_holder, p));
          } else {
            it->second.add(tmp_data[_jml_col - 1]);
          }

        } else {
          Tlabel_stat p;
          p.add(tmp_data[_jml_col - 1]);
          _col_pot_split.insert(pair<Tmy_dttype, Tlabel_stat>(pot_split_holder, p));
        }
      }
      tmp_data.clear();
      tmp_data.shrink_to_fit();
      _data.next_record();



    }
    _data.close_file();




  } else {
    cout << "get_col_pot_split,Gagal buka file !!!" << endl;
  }
}

void Tdataframe::continue_below(Tmy_dttype find, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, Tlabel_stat &stat_label)
{
  auto it_find = _col_pot_split.find(find);
  if (it_find != _col_pot_split.end())
  {
    it_find++;
    for (auto it = _col_pot_split.begin(); it != it_find; it++) {
      stat_label = stat_label + it->second;
    }
  }
}

void Tdataframe::continue_above(Tmy_dttype find, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, Tlabel_stat &stat_label)
{
  auto it_find = _col_pot_split.find(find);
  if (it_find != _col_pot_split.end())
  {
    it_find++;
    for (auto it = it_find; it != _col_pot_split.end(); it++) {
      stat_label = stat_label + it->second;
    }
  }
}

void Tdataframe::calculate_metric(bool is_continuous, int start, int end, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float &current_metric, string &split_value)
{
  float best_overall_metric = 999;
  string tmp_split_value;

  map<Tmy_dttype, Tlabel_stat>::iterator it, it1, it_start, it_end;

  bool is_below;
  bool first_iteration = true;
  float p_dt_above, p_dt_below, entropy_below, entropy_above, overall_metric;
  int jml;

  Tlabel_stat _stat_label_above;
  Tlabel_stat _stat_label_below;



  if (_col_pot_split.size() > 0)
  {

    if (start == 1)
    {
      it_start = _col_pot_split.begin();
    } else {
      int i = 1;
      it1 = _col_pot_split.begin();
      while ( (i <= start) and (it1 != _col_pot_split.end()))
      {
        it1++;
        i++;
      }
      it_start = it1;

    }

    if (end == _col_pot_split.size())
    {
      it_end = _col_pot_split.end();
    } else {
      int i = 1;
      it1 = _col_pot_split.begin();
      while ( (i <= end) and (it1 != _col_pot_split.end()))
      {
        it1++;
        i++;
      }
      it_end = it1;
    }

    for (it = it_start; it != it_end; ++it)
    {

      _stat_label_below.clear();
      _stat_label_above.clear();


      if (is_continuous)
      {
        //continue_below(it->first, _col_pot_split, _stat_label_below);
        //continue_above(it->first, _col_pot_split, _stat_label_above);
        thread t1(&Tdataframe::continue_below, it->first, ref(_col_pot_split), ref(_stat_label_below));
        thread t2(&Tdataframe::continue_above, it->first, ref(_col_pot_split), ref(_stat_label_above));

        t1.join();
        t2.join();

      } else {
        // for (it1 = _col_pot_split.begin(); it1 != _col_pot_split.end(); ++it1)
        // {
        //   is_below =  it1->first == it->first ;

        //   if (is_below)
        //   {
        //     _stat_label_below = _stat_label_below + it1->second;

        //   } else {
        //     _stat_label_above = _stat_label_above + it1->second;

        //   }

        // }
      }

      jml = _stat_label_below.get_jml_row() + _stat_label_above.get_jml_row();
      p_dt_below = (float) _stat_label_below.get_jml_row() / jml;
      p_dt_above = (float) _stat_label_above.get_jml_row() / jml;

      entropy_below = _stat_label_below.get_entropy();
      entropy_above = _stat_label_above.get_entropy();


      overall_metric = (p_dt_below * entropy_below) + (p_dt_above * entropy_above);


      if (first_iteration or (overall_metric <= best_overall_metric))
      {
        first_iteration = false;

        best_overall_metric = overall_metric;
        Tmy_dttype tmp_pot_split_holder = it->first;
        tmp_split_value = tmp_pot_split_holder.get_string();
      }
    }
    current_metric = best_overall_metric;
    split_value = tmp_split_value;
  }

}

void Tdataframe::calculate_overall_metric(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value)
{
  //cout << "          calculate_overall_metric " << get_nm_header(idx) << endl;

  if (_col_pot_split.size() > 0)
  {


    std::vector<Tbelow_above> vec_col_pot_split;

    int i = 0;
    for (auto it = _col_pot_split.begin(); it != _col_pot_split.end(); it++)
    {
      Tbelow_above ba;
      ba.set_value(it->first);

      if (i>0)
      {
        ba.add_below(it->second+vec_col_pot_split[i-1].get_below());
      }
      else {
        ba.add_below(it->second);
      }

      vec_col_pot_split.push_back(ba);
      i++;
    }
    
    i-=2;
    int j=0;
    for (auto it = _col_pot_split.rbegin(); it !=_col_pot_split.rend();it++)
    {
      if((j>0) and (i>=0))
      {
        vec_col_pot_split[i].add_above(vec_col_pot_split[i+1].get_above()+it->second);
      }else
      {
        if(i>0){   
         vec_col_pot_split[i].add_above(it->second); 
         }
      }
      j++;
      i--;
    }



    if (_col_pot_split.size() < 10000)
    {
      float best_overall_metric;
      string tmp_split_value;
      thread t1(&Tdataframe::calculate_metric, _data_type[idx] == "continuous.", 1, _col_pot_split.size(), ref(_col_pot_split), ref(best_overall_metric), ref(tmp_split_value));
      t1.join();
      current_overall_metric = best_overall_metric;
      split_value = tmp_split_value;
    } else {
      float best_overall_metric = 999, best_overall_metric1, best_overall_metric2;
      string tmp_split_value, tmp_split_value1, tmp_split_value2;

      int jml_loop = _col_pot_split.size() / 5000;

      if ((5000 * jml_loop) < _col_pot_split.size()) {
        jml_loop += 1;
      }

      cetak(to_string(jml_loop).c_str());
      cetak("|");

      for (int i = 1; i <= jml_loop; i += 2)
      {
        cetak(to_string(i).c_str());

        thread t1(&Tdataframe::calculate_metric, _data_type[idx] == "continuous.", 1 + ((i - 1) * 5000), 5000 * i, ref(_col_pot_split), ref(best_overall_metric1), ref(tmp_split_value1));
        thread t2(&Tdataframe::calculate_metric, _data_type[idx] == "continuous.", 1 + (i * 5000), 5000 * (i + 1), ref(_col_pot_split), ref(best_overall_metric2), ref(tmp_split_value2));

        cetak("t1");
        t1.join();
        cetak("t2");
        cetak("|");
        t2.join();



        if (best_overall_metric1 < best_overall_metric)
        {
          best_overall_metric = best_overall_metric1;
          tmp_split_value = tmp_split_value1;
        }

        if (best_overall_metric2 < best_overall_metric)
        {
          best_overall_metric = best_overall_metric2;
          tmp_split_value = tmp_split_value2;
        }
      }

      current_overall_metric = best_overall_metric;
      split_value = tmp_split_value;
    }



    if (_col_pot_split.size() > 0)
    {
      map<Tmy_dttype, Tlabel_stat>::iterator it = _col_pot_split.begin();
      while (it != _col_pot_split.end())
      {
        it->second.clear();
        it++;
      }

      _col_pot_split.clear();

      for(int i=0;i<vec_col_pot_split.size();i++)
      {
         vec_col_pot_split[i].clear();
      }

      vec_col_pot_split.clear();

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

    } else {
      cout << "Gagal Buka/Buat File !!!" << endl;
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

void Tdataframe::clear_memory()
{
  _data.clear_memory();
}

void Tdataframe::info()
{
  cout << " Info" << endl;
  cout << " Nama File   : " << _nm_file << endl;
  cout << " Jumlah Data : " << _jml_row << endl;
  cout << " Statistik   : " << endl;
  if (_stat_label.size() > 0)
  {
    cout << setw(30) << "label" << setw(10) << "Jumlah" << endl;
    for (auto it = _stat_label.begin(); it != _stat_label.end(); ++it) {
      cout << setw(30) << (*it).first << setw(10) << (*it).second << endl;
    }
  }
}