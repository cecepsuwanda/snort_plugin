#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
using namespace std;


struct twaktu
{
  int jam, menit, detik;
};

struct ttanggal
{
  int tanggal, bulan, tahun;
};

struct field_filter
{
  string ID;
  ttanggal Date;
  string Name;
  string Category;
  twaktu Start_Time;
  twaktu Duration;
  vector<string> Attacker;
  vector<string> Victim;
  string Username;
  vector<string> At_Attacker;
  vector<string> At_Victim;
};

struct tip_fragment
{
  string part1;
  string part2;
  string part3;
  string part4;
  int port;
};


class Tolah_label
{

private:
  string _nm_file;

  vector<field_filter *> vec;
  
  field_filter *field;

  vector<string> tokenizer(char* str, const char* separator);

  bool olah_id(char *str);
  bool olah_date(char *str);
  bool olah_name(char *str);
  bool olah_category(char *str);
  bool olah_start_time(char *str);
  bool olah_duration(char *str);
  bool olah_at_attacker(char *str);
  bool olah_at_victim(char *str);
  bool olah_username(char *str);
  bool olah_attacker(char *str);
  bool olah_victim(char *str);

  void olah_sisa_at_attacker(char *str);
  void olah_sisa_at_victim(char *str);

  bool is_subs(string stack, string needle);
  tip_fragment ip_frag(string ip_port);
  ttanggal date_frag(string date_str);
  twaktu waktu_frag(string waktu_str);
  int waktu_to_sec(twaktu waktu, int add12);
  bool compare_ip(tip_fragment ip1, tip_fragment ip2);
  bool compare_port(int port1, string port2);
  bool isNumber( std::string token );

  bool is_date_pass(vector<string> &row, field_filter *field);
  bool is_waktu_pass(vector<string> &row, field_filter *field);
  bool is_attacker_pass(string ip_port, vector<string> &ip_attacker, vector<string> &port_attacker);
  bool is_victim_pass(string ip_port, vector<string> &ip_victim, vector<string> &port_victim);

public:
  Tolah_label();
  ~Tolah_label();

  void setnm_f(string nm_f);
  void baca_file();
  string labeli(vector<string> row);
};