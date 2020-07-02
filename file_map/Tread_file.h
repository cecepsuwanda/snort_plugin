#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class Tread_file {
private:
	string _nm_f;
	const char *_separator;
	FILE *_file = NULL;
	
	vector<string> _data;
	
	bool is_fmap = true;
	int  _posisi=0;
	int  _b_posisi=0;
    int _fd=-1;
	char * _file_in_memory= NULL;
	struct stat _sb;
    
    int _idx_posisi=0;
    bool is_index = false;
	vector<int> _index;

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
	void file_map();

	bool open_file();
	bool open_file(string mode);
	void read_file();
	void write_file(string row);
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

	void clear_index();
	void index_on();
	void index_off();
	void add_index();

};