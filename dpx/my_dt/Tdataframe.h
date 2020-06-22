
#include "Tread_file.h"
#include <unordered_map>
#include <map>
#include <cmath>

struct field_filter
{
  int idx_opt;
  string value;
};

struct pot_struct
{
  int jml;
  map<string, int> stat_label;
};

class Tdataframe
{
private:
  Tread_file _data;
  vector<string> _data_header;
  vector<string> _data_type;
  unordered_map<int, field_filter> _filter;
  map<string, int> _stat_label;
  int _jml_col = 0;
  int _jml_row = 0;

public:
  Tdataframe();
  ~Tdataframe();
  Tdataframe(const Tdataframe &t);
  Tdataframe& operator = (const Tdataframe &t)
  {
    this->_data = t._data;
    this->_data_header = t._data_header;
    this->_data_type = t._data_type;
    this->_filter = t._filter;
    //this->_stat_label = t._stat_label;
    this->_jml_col = t._jml_col;
    //this->_jml_row = t._jml_row;
    return *this;
  }

  void read_data(string nm_f);
  void write_data(vector<string> &data);
  void read_data_type(string nm_f);
  void add_filter(int idx_col, field_filter filter);
  void split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above);
  map<string, int> get_unique_value(int idx_col);
  int getjmlcol();
  int getjmlrow();
  bool is_pass(vector<string> &data);
  bool is_pass(int opt, string value1, string value2);
  void stat_tabel();
  map<string, int> get_stat_label();

  string get_nm_header(int idx_col);
  int get_opt(int idx_col, int is_below);
  void get_col_pot_split(int idx, map<string, pot_struct> &_col_pot_split);
  void calculate_overall_metric(int idx, map<string, pot_struct> &_col_pot_split, float &current_overall_metric, string &split_value);

  bool open_file();
  void read_file();
  void close_file();

  bool is_eof();
  void next_record();
  vector<string> get_record();


};