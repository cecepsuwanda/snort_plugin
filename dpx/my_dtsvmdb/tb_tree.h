#include <string>
#include <vector>
#include <ctime>
#include "Tquery_builder.h"

using namespace std;

#ifndef Included_tb_tree_H

#define Included_tb_tree_H

class tb_tree
{
private:
	Tquery_builder global_query_builder;
	vector<string> _data;

	int _jml_row = 0;
	int _jml_col = 0;

	void clear_data();	
public:
	tb_tree();
	~tb_tree();	

	void baca_tree();	

	void reset_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

};

#endif