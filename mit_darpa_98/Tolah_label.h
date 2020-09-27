#include "Tread_file.h"
#include <string>
#include <iostream>
#include <vector>
#include <regex>

using namespace std;

struct twaktu
{
  int jam, menit, detik;
};

struct thost
{
	string ip_addr;
	string host_name;
};

struct field_filter
{
	string name;
	string time;
	string src;
	string dst;
	string user;
	string where;
	string variant;
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
	map<string, map<string, vec_field_filter>> vec_map;

	vector<string> tokenizer(char *str, const char *separator);

	void baca_attack_file();
	void baca_host_file();

	string search_host(string token);

	bool isIP4(string token);
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

public:
	Tolah_label();
	~Tolah_label();

	void setnm_f(string nm_attack_f, string nm_host_f);
	void baca_file();
	string labeli(vector<string> row, string week, string day);


};