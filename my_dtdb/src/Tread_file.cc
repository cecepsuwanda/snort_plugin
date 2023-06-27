#include "Tread_file.h"


Tread_file::Tread_file()
{
  _jml_row = -1;
  _jml_col = -1;
  _idx_label = -1;
  _is_con = false;
}

Tread_file::~Tread_file()
{
  clear_data();
  close_file();
}

vector<string> Tread_file::get_data_header()
{
  return _data_header;
}

vector<string> Tread_file::get_data_type()
{
  return _data_type;
}

int Tread_file::get_idx_label()
{
  return _idx_label;
}

void Tread_file::setnm_f(string nm_tb, int id_dt, int jns_dt, string partition)
{
  _nm_tb = nm_tb;
  _id_dt = id_dt;
  _jns_dt = jns_dt;
  _partition = partition;

  string tmp = "";

  tmp = "id_dt=" + to_string(_id_dt);

  if (_jns_dt == 3)
  {
    tmp = tmp + " and (jns_dt=2 or jns_dt=3)";
  } else {
    tmp = tmp + " and jns_dt=" + to_string(_jns_dt);
  }

  _head_where = tmp;
  
  _default_query = "select * from " + _nm_tb + " where " + _head_where + " order by id";
  if (_nm_tb == "dataset") {
    _default_query = "select * from " + _nm_tb + " partition(" + _partition + ") where " + _head_where + " order by id";
  }


  global_query_builder.open_connection();

  string tmp_query1 = "select count(*) as jml from " + _nm_tb + " where " + _head_where + " order by id";
  if (_nm_tb == "dataset") {
    tmp_query1 = "select count(*) as jml from " + _nm_tb + " partition(" + _partition + ") where " + _head_where + " order by id";
  }

  if (global_query_builder.query(tmp_query1))
  {
    if (global_query_builder.get_result())
    {
      vector<string> tmp = global_query_builder.fetch_row();
      _jml_row = stoi(tmp[0]);
    }
  }

  string tmp_query2 = "select * from " + _nm_tb + " where " + _head_where + " order by id limit 1";
  if (_nm_tb == "dataset") {
    tmp_query2 = "select * from " + _nm_tb + " partition(" + _partition + ") where " + _head_where + " order by id limit 1";
  }

  if (global_query_builder.query(tmp_query2))
  {
    if (global_query_builder.get_result())
    {
      _jml_col = global_query_builder.get_jml_col();

      _data_header.clear();
      _data_type.clear();

      if (_nm_tb == "dataset") {
        vector<string> tmp_header = global_query_builder.get_column_name();
        vector<string> tmp_type = global_query_builder.get_column_type();
        for (int i = 3; i < _jml_col; i++)
        {
          _data_header.push_back(tmp_header[i]);
          _data_type.push_back(tmp_type[i]);

          _idx_label = i - 3;
        }
      }
    }
  }

  global_query_builder.close_connection();
}

void Tread_file::filter(string sql, bool is_con)
{
  global_query_builder.open_connection();
  if (!is_con) {

    string tmp = "select count(*) as jml from " + _nm_tb + " where (" + _head_where + ")" + sql + " order by id";
    if (_nm_tb == "dataset") {
      tmp = "select count(*) as jml from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ")" + sql + " order by id";
    }

    if (global_query_builder.query(tmp))
    {
      if (global_query_builder.get_result())
      {
        vector<string> tmp = global_query_builder.fetch_row();
        _jml_row = stoi(tmp[0]);
      }
    }

    global_query_builder.close_connection();
  } else {
    string tmp = "select * from " + _nm_tb + " where (" + _head_where + ")" + sql + " order by id";
    if (_nm_tb == "dataset") {
      tmp = "select * from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ")" + sql + " order by id";
    }

    if (global_query_builder.query(tmp))
    {
      if (global_query_builder.get_result())
      {
        _jml_row = global_query_builder.get_jml_row();
        _jml_col = global_query_builder.get_jml_col();
      }
    }

  }

  _is_con = is_con;
}

void Tread_file::delete_data(string sql)
{

  string tmp  = "delete from " + _nm_tb + " where (" + _head_where + ") " + sql;

  global_query_builder.open_connection();
  global_query_builder.query(tmp);
  global_query_builder.close_connection();
}

Tlabel_stat Tread_file::hit_label_stat(string nm_kolom, string sql)
{
  Tlabel_stat label_stat;
  
  string tmp = "select id from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ")";
  if (sql != "")
  {
    tmp = "select id from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ") " + sql ;
  }

  global_query_builder.open_connection();

  string tmp_sql = "truncate tb_index";
  global_query_builder.query(tmp_sql);

  tmp_sql = "insert into tb_index(idx_row) " + tmp;
  global_query_builder.query(tmp_sql);

  tmp_sql = "select " + nm_kolom + ",count(" + nm_kolom + ") as jml from " + _nm_tb + " partition(" + _partition + ") where id in (select idx_row from tb_index) group by " + nm_kolom + " order by " + nm_kolom;

  if (global_query_builder.query(tmp_sql))
  {
    if (global_query_builder.get_result())
    {
      int jml_row = global_query_builder.get_jml_row();

      if (jml_row > 0) {

        while (jml_row > 0)
        {
          vector<string> data = global_query_builder.fetch_row();
          label_stat.add(data[0], stoi(data[1]));
          jml_row--;
        }
      }
    }
  }

  global_query_builder.close_connection();

  return label_stat;

}

// map<Tmy_dttype, Tlabel_stat> Tread_file::hit_col_split(string group_kolom , string count_kolom, string sql)
// {
//   map<Tmy_dttype, Tlabel_stat> col_split;
  
//   size_t i = 0;
//   bool ketemu = false;
//   while ((i < _data_header.size()) and (not ketemu))
//   {
//     if (_data_header[i] == group_kolom)
//     {
//       ketemu = true;
//     } else {
//       i++;
//     }
//   }

//   if (ketemu)
//   {
//     global_query_builder.open_connection();

//     string tmp = "truncate attr" + to_string(i);

//     global_query_builder.query(tmp);

//     if (_data_type[i] == "continuous.")
//     {

//       // tmp = "select round(" + group_kolom + ",7) as hsl_round," + count_kolom + " from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ") ";
//       // if (sql != "")
//       // {
//       //   tmp = tmp + sql;
//       // }

//       tmp = "select round(" + group_kolom + ",2) as hsl_round," + count_kolom + " from " + _nm_tb + " partition(" + _partition + ") where id in (select idx_row from tb_index)";

//       tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select hsl_round," + count_kolom + ",count(" + count_kolom + ") as jml from (" + tmp + ") tb group by hsl_round," + count_kolom +  " order by hsl_round," + count_kolom;

//       global_query_builder.query(tmp);

//     } else {
//       // tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select " + group_kolom + "," + count_kolom + ",count(" + count_kolom + ") as jml from " + _nm_tb + " partition(" + _partition + ") where (" + _head_where + ") ";
//       // if (sql != "")
//       // {
//       //   tmp = tmp + sql;
//       // }

//       tmp = "insert into attr" + to_string(i) + "(" + group_kolom + ",label,jml) select " + group_kolom + "," + count_kolom + ",count(" + count_kolom + ") as jml from " + _nm_tb + " partition(" + _partition + ") where id in (select idx_row from tb_index)";

//       tmp = tmp + " group by " + group_kolom + "," + count_kolom +  " order by " + group_kolom + "," + count_kolom;
//       global_query_builder.query(tmp);
//     }

//     global_query_builder.close_connection();

//   }


//   return col_split;
// }

void Tread_file::update_attr_stat(int idx)
{
  global_query_builder.open_connection();
  string tmp = "delete from attr_stat where id=" + to_string(idx);
  global_query_builder.query(tmp);

  tmp = "select count(*) as jml from ( select distinct " + _data_header[idx] + " from attr" + to_string(idx) + ") tb";
  if (global_query_builder.query(tmp))
  {
    if (global_query_builder.get_result())
    {
      vector<string> data = global_query_builder.fetch_row();
      tmp = "insert into attr_stat values(" + to_string(idx) + ",'" + _data_header[idx] + "'," + (_data_type[idx] == "continuous." ? "1" : "0") + "," + data[0] + ")";
      global_query_builder.query(tmp);
    }
  }
  global_query_builder.close_connection();
}


int Tread_file::get_jml_row()
{
  return _jml_row;
}

int Tread_file::get_jml_col()
{
  return _jml_col;
}

void Tread_file::close_file()
{
  _is_con = false;
  global_query_builder.close_connection();
}

void Tread_file::reset_file()
{
  global_query_builder.go_to_no_record(0);
  read_file();
}

void Tread_file::read_file()
{
  clear_data();
  _idx_col = 0;

  vector<string> data = global_query_builder.fetch_row();

  if (data.size() > 0) {
    int awal = (_nm_tb == "dataset") ? 3 : (_nm_tb == "tree") ? 7 : 0;

    for (int i = awal; i < _jml_col; i++)
    {
      _data.push_back(data[i]);
    }

  }

}

void Tread_file::clear_data()
{
  _data.clear();
  _data.shrink_to_fit();
  _data_header.clear();
  _data_header.shrink_to_fit();
  _data_type.clear();
  _data_type.shrink_to_fit();
}

bool Tread_file::is_eof()
{
  return _data.size() == 0;
}

vector<string> Tread_file::get_record()
{
  return _data;
}

void Tread_file::next_col()
{
  _idx_col = _idx_col + 1;
}

bool Tread_file::is_end_col()
{
  return  !(_idx_col < _data.size());
}

string Tread_file::get_col_val()
{

  return _data[_idx_col];
}

int Tread_file::get_idx_col()
{

  return _idx_col;
}

string Tread_file::get_col_val(int idx_col)
{
  return _data[idx_col];
}

void Tread_file::next_record()
{
  read_file();
}




