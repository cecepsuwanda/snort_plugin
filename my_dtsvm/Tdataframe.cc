
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

  // cetak("{ split ");
  // cetak(to_string(split_column).c_str());
  // cetak(",");
  // cetak(split_value.c_str());
  // cetak("}");

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

        _stat_label.add(tmp_data[_jml_col - 1]);

        // if (_stat_label.size() > 0)
        // {
        //   it = _stat_label.find(tmp_data[_jml_col - 1]);
        //   if (it == _stat_label.end())
        //   {
        //     //cout << _jml_col << " " << tmp_data[_jml_col-1] << endl;
        //     _stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
        //   } else {
        //     it->second += 1;
        //   }
        // } else {
        //   //cout << "data baru" << _jml_col << tmp_data.size() << endl;
        //   //cout << _jml_col << " " << tmp_data[_jml_col-1] << endl;
        //   _stat_label.insert(pair<string, int>(tmp_data[_jml_col - 1], 1));
        // }

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
  return _stat_label.get_map();
}



void Tdataframe::get_col_pot_split(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split)
{
  //cetak(" get_col_pot_split ");
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



void Tdataframe::calculate_metric(bool is_continuous, int start, int end, vector<Tbelow_above> &_col_pot_split, float &current_metric, string &split_value, float &sum_entropy)
{


}

void Tdataframe::calculate_overall_metric(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value, float &sum_entropy)
{
  //cetak(" calculate_overall_metric ");
  //cout << "          calculate_overall_metric " << get_nm_header(idx) << endl;

  if (_col_pot_split.size() > 0)
  {

    vector<Tbelow_above> vec_col_pot_split;

    if (_col_pot_split.size() == 1)
    {
      //cetak("==1");
      auto itr = _col_pot_split.begin();
      
      Tbelow_above ba;
      ba.set_value((*itr).first);
      ba.add_below((*itr).second);

      vec_col_pot_split.push_back(ba);

    } else {

      //cetak(" >2 ");

      float jml = 0, is_pure = 0;

      for (auto itr = _col_pot_split.begin(); itr != _col_pot_split.end() ; itr++)
      {
        if ((*itr).second.get_jml_row_in_map() == 1)
        {
          is_pure++;
        }
        jml++;
      }

      if (is_pure == jml)
      {
        cetak(" >2 pure ");

        Tlabel_stat _stat_label_below;

        string prev_label;
        Tmy_dttype prev_key;
        Tlabel_stat prev_map;
        bool is_first = true;
        string tmp_string;

        for (auto itr = _col_pot_split.begin(); itr != _col_pot_split.end() ; itr++)
        {
          _stat_label_below = _stat_label_below + (*itr).second;

          if ((*itr).second.get_jml_row_in_map() == 1)
          {

            tmp_string = (*itr).second.get_first_value_in_map();


            if ((!is_first) and (prev_label != tmp_string))
            {
              Tmy_dttype tmp = (*itr).first;
              string tmp_str = to_string((stof(prev_key.get_string()) + stof(tmp.get_string())) / 2);
              Tmy_dttype mid_point(tmp_str, true);

              Tbelow_above ba;
              ba.set_value(mid_point);
              ba.add_below(_stat_label_below);
              Tlabel_stat tmp_stat = _stat_label - _stat_label_below;
              ba.add_above(tmp_stat);

              vec_col_pot_split.push_back(ba);
              //cetak(" ada 1 !!");
            }

            prev_label = tmp_string;
            prev_key = (*itr).first;
            prev_map = (*itr).second;
            is_first = false;

          }


        }

        _stat_label_below.clear();

      } else {



        Tlabel_stat _stat_label_below;
        Tmy_dttype prev_key, next_key;

        if (jml > 2)
        {
          //cetak("{>2 1}");

          int posisi_mid = ceil(jml / 2);

          int j = 0;
          auto itr =  _col_pot_split.begin();
          while ((j < posisi_mid) and (itr != _col_pot_split.end()))
          {
            _stat_label_below = _stat_label_below + (*itr).second;

            j++;
            itr++;
          }

          prev_key = (*itr).first;
          itr++;
          next_key = (*itr).first;
          

        } else {

          //cetak("{>2 2}");

          auto itr =  _col_pot_split.begin();
          prev_key = (*itr).first;
          _stat_label_below = _stat_label_below + (*itr).second;
          itr++;
          next_key = (*itr).first;
        }


        
        string tmp_str = to_string((stof(prev_key.get_string()) + stof(next_key.get_string())) / 2);
        Tmy_dttype mid_point(tmp_str, true);

        Tbelow_above ba;
        ba.set_value(mid_point);
        ba.add_below(_stat_label_below);
        ba.add_above(_stat_label - _stat_label_below);

        vec_col_pot_split.push_back(ba);

        _stat_label_below.clear();
        

      }
      
    }

    auto it = _col_pot_split.begin();
    while (it != _col_pot_split.end())
    {
      it->second.clear();
      it++;
    }

    _col_pot_split.clear();



    if (vec_col_pot_split.size() > 0)
    {


      float best_overall_metric = 999, overall_metric, tmp_sum_entropy = 0;
      string tmp_split_value;

      bool first_iteration = true;

      for (auto i = 0; i < vec_col_pot_split.size(); i++)
      {

        overall_metric = vec_col_pot_split[i].get_overall_metric();
        tmp_sum_entropy += overall_metric;

        if (first_iteration or (overall_metric <= best_overall_metric))
        {
          first_iteration = false;

          best_overall_metric = overall_metric;
          tmp_split_value = vec_col_pot_split[i].get_value().get_string();
        }

      }

      current_overall_metric = best_overall_metric;
      split_value = tmp_split_value;
      sum_entropy = tmp_sum_entropy;      

      for (int i = 0; i < vec_col_pot_split.size(); i++)
      {
        vec_col_pot_split[i].clear();
      }

      vec_col_pot_split.clear();
      vec_col_pot_split.shrink_to_fit();      

    }

    

    //cetak(">2 -hapus 2-");
  }



  //     thread t1(&Tdataframe::calculate_metric, _data_type[idx] == "continuous.", 1 + ((i - 1) * 5000), 5000 * i, ref(vec_col_pot_split), ref(best_overall_metric1), ref(tmp_split_value1), ref(tmp_sum_entropy1));
  //     thread t2(&Tdataframe::calculate_metric, _data_type[idx] == "continuous.", 1 + (i * 5000), 5000 * (i + 1), ref(vec_col_pot_split), ref(best_overall_metric2), ref(tmp_split_value2), ref(tmp_sum_entropy2));

  //     // cetak("t1");
  //     t1.join();
  //     // cetak("t2");
  //     // cetak("|");
  //     t2.join();



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

  cout << _stat_label << endl ;
}

float Tdataframe::get_entropy()
{
  return _stat_label.get_entropy();
}