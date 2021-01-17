#include "Tread_file.h"
#include <map>
#include <iterator>
#include <string>
#include <ctime>

using namespace std;

#ifndef Included_Tdataframe_label_H

#define Included_Tdataframe_label_H

class Tdataframe_label
{
private:
	Tread_file _data;

	map<string, int> _stat_label;

	bool is_index = false;

public:
	Tdataframe_label();
	~Tdataframe_label();

	void read_data(string nm_f);
	void stat_tabel();
	map<string, int> get_stat_label();

	bool open_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

    void clear_memory();
    
};

#endif