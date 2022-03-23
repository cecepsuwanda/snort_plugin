#include <string>
#include <map>
#include <iterator>
#include "Tmy_double.h"

using namespace std;

#ifndef Included_Tmy_cache_H

#define Included_Tmy_cache_H

class Tmy_cache
{
private:
   int _jml_data;
   int _size;

   map<int, vector<Tmy_double>> _head;

public:
	Tmy_cache(int jml_data,int size);
	~Tmy_cache();

	bool is_in_head(int idx);
	vector<Tmy_double> get_head(int idx);
	void isi_head(int idx_map,int idx_vec,Tmy_double val);
	
};

#endif