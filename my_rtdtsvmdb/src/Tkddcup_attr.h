#include <string>
#include <map>

#include "global.h"
#include "Tmy_dttype.h"
#include "Tquery_builder.h"

using namespace std;

#ifndef Included_tkddcup_attr_H

#define Included_tkddcup_attr_H

class Tkddcup_attr
{

private:
  string _ip_src;
  string _ip_dst;
  int _port_src;
  int _port_dst;
  
  time_t _start;
  time_t _end;

  map<int, Tmy_dttype> _attr;

  string _label; 

public:
	Tkddcup_attr();
	~Tkddcup_attr();

  void add_continuous(int idx,double attr,int digit);
  void add_non_continuous(int idx,string attr);

  void add_extra(string ip_src,int port_src,string ip_dst,int port_dst,time_t start_t,time_t end_t);

  void save_to_db();
	
};


#endif