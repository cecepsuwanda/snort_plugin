#include "Tread_file.h"
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <ctime>

using namespace std;

struct twaktu
{
  int jam, menit, detik;
};

struct ttanggal
{
  int tanggal, bulan, tahun;
};


struct thost
{
	string ip_addr;
	string host_name;
	string tag;
};

struct field_filter
{
	string week;
	string day;
	string name;
	string time;
	string src;
	vector<string> vec_src;
	string dst;
	vector<string> vec_dst;
	string user;
	string where;
	string variant;
	long tgl_timestamp;
	long waktu_timestamp;
};

struct tip_fragment
{
  string part1;
  string part2;
  string part3;
  string part4;
  int port;
  string protocol;    
};

typedef vector<field_filter *> vec_field_filter;

class Tolah_label
{

private:
	string _attack_file, _host_file;
	vector<thost *> vec_host;
	vec_field_filter vec_attack;
	map<long, map<long, vec_field_filter>> vec_map;

	vector<string> tokenizer(char *str, const char *separator);

	void baca_attack_file();
	void baca_host_file();

	string search_host(string token);

	bool isIP4(string token);
	bool isIP4star(string token);
	bool isIP43seg(string token);
	bool isQuote(string token);
	bool isString(string token);
	bool is_subs(string stack, string needle);
	bool isNumber(std::string token);

	twaktu waktu_frag(string waktu_str);
	tip_fragment ip_frag(string ip_port);
	int waktu_to_sec(twaktu waktu);
	bool compare_ip(tip_fragment ip1, tip_fragment ip2);

	bool is_waktu_pass(vector<string> &row, field_filter *field);
	bool is_ip_pass(vector<string> &row, field_filter *field);

	long ttanggal_to_timestamp(ttanggal tgl);
	long datetime_to_timestamp(ttanggal tgl,twaktu waktu);

public:
	Tolah_label();
	~Tolah_label();

	void setnm_f(string nm_attack_f, string nm_host_f);
	void baca_file();
	string labeli(vector<string> row, string week, string day);


};