

#include <string>

using namespace std;

#ifndef Included_Tread_file_darpa_H

#define Included_Tread_file_darpa_H

class Tread_file_darpa
{
private:
	string _nm_f;
    FILE *_file = NULL;

public:
	Tread_file_darpa();
	~Tread_file_darpa();

	void setnm_f(string nm_f);

	bool open_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();
};


#endif