
#include <vector>
#include <string>
using namespace std;

#ifndef Included_Twrite_file_H

#define Included_Twrite_file_H

class Twrite_file
{
private:
   string _nm_f;
   FILE *_file = NULL;
   void open_file();

public:
	Twrite_file();
	~Twrite_file();

	void setnm_f(string nm_f);
	void close_file();

    void write_file(vector<string> &data);
	void write_file(string data);

};

#endif