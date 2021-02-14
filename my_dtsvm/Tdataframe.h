
#include "Tread_file.h"
#include "Tmy_dttype.h"
#include "Tlabel_stat.h"
#include "Tbelow_above.h"
#include "Tbase_dataframe.h"
#include <thread>
#include <unordered_map>
#include <map>
#include <cmath>
#include <string>

using namespace std;

#ifndef Included_Tdataframe_H

#define Included_Tdataframe_H

class Tdataframe : public Tbase_dataframe 
{
private:
    
  static void calculate_metric(int start, int end, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float & current_overall_metric, string & split_value,Tlabel_stat & stat_label);

  
public:
  Tdataframe();
  ~Tdataframe();
   
  void split_data(int split_column, string split_value, Tdataframe &data_below, Tdataframe &data_above);
  map<string, int> get_unique_value(int idx_col);
  
  
  float get_entropy();

  string get_nm_header(int idx_col);
  int get_opt(int idx_col, int is_below);
  void get_col_pot_split(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split);
  void calculate_overall_metric(int idx, map<Tmy_dttype, Tlabel_stat> &_col_pot_split, float &current_overall_metric, string &split_value);
  
};

#endif