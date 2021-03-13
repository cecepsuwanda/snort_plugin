#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ipc.h>     /* general SysV IPC structures          */
#include <sys/shm.h>	 /* shared memory functions and structs. */
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <bits/stdc++.h>

using namespace std;

#ifndef Included_Tread_file_H

#define Included_Tread_file_H

#define pagesize  4096

class Tread_file {
private:
	string _nm_f;
	const char *_separator;
	vector<string> _data;

	int  _posisi = 0;
	int  _b_posisi = 0;
	int _fd = -1;
	char * _file_in_memory = NULL;
	struct stat _sb;

	int _idx_posisi = 0;
	bool is_index = false;
	vector<int> _index;
	int *_idx_in_memory = NULL;
	int _jml_index = 0;
	int _ukuran_index = 0;

	vector<string> tokenizer(char* str, const char* separator);
	void clear_data();
	bool open_file();


public:
	Tread_file();
	~Tread_file();
	Tread_file(const Tread_file &t);
	Tread_file& operator = (const Tread_file &t)
	{
		this->_nm_f = t._nm_f;
		this->_separator = t._separator;

		this->_fd = t._fd;
		this->_file_in_memory = t._file_in_memory;
		this->_sb = t._sb;

		if (t._jml_index>0) {
			clear_index();
			for (int i = 0; i < t._jml_index; ++i)
			{
				_index.push_back(t._idx_in_memory[i]);
			}
			save_to_memory();
			clear_index();
		}
		return *this;
	}

	void setnm_f(string nm_f, const char* separator);
	void setseparator(const char* separator);

	bool open_file(string mode);
	void reset_file();
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
	void save_to_memory();
	void clear_memory();

};

#endif