#include <string>
#include <vector>
#include <ctime>
#include "global.h"
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

	bool cari_tree(time_t id_detail_experiment);
	bool cari_tree(time_t id_experiment, time_t id_detail_experiment);

	void baca_tree(time_t id_detail_experiment);
	void baca_tree(time_t id_experiment, time_t id_detail_experiment);
	void write_tree(vector<string> &data); 

	void reset_file();
	void read_file();
	void close_file();

	bool is_eof();
	void next_record();
	vector<string> get_record();

};

#endif