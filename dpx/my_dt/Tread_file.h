#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>

using namespace std;

class Tread_file {
private:
	string _nm_f;
	const char *_separator;
	FILE *_file = NULL;
	vector<string> _data;

	vector<string> tokenizer(char* str, const char* separator);


	void clear_data();
public:
	Tread_file();
	~Tread_file();
	Tread_file(const Tread_file &t);
	Tread_file& operator = (const Tread_file &t)
	{
		this->_nm_f = t._nm_f;
		this->_separator = t._separator;
		return *this;
	}

	void setnm_f(string nm_f);
	void setseparator(const char* separator);


	bool open_file();
	bool open_file(string mode);
	void read_file();
	void write_file(string row);
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

};