#include "Tread_file_darpa.h"
#include <vector>
#include <string>
using namespace std;


#ifndef Included_Tdataframe_darpa_H

#define Included_Tdataframe_darpa_H


class Tdataframe_darpa
{
private:
	Tread_file_darpa _data;
	string _nm_file;

public:
	Tdataframe_darpa();
	~Tdataframe_darpa();

	void read_data(string nm_f);

	bool open_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

};


#endif