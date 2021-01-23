#include "Tdatetime_holder.h"
#include <string>


using namespace std;

#ifndef Included_Trange_holder_H

#define Included_Trange_holder_H

class Trange_holder
{
private:
	Tdatetime_holder _start;
	Tdatetime_holder _end;
	string _label;

public:
	
    Trange_holder();
	Trange_holder(Tdatetime_holder start, Tdatetime_holder end, string label);
	~Trange_holder();

	void setStart(Tdatetime_holder start);
	void setEnd(Tdatetime_holder end);
	void setLabel(string label);

    Tdatetime_holder getStart();
	Tdatetime_holder getEnd();
	string getLabel();

	bool in_range(Tdatetime_holder start, Tdatetime_holder end);

	friend ostream & operator << (ostream &out, const Trange_holder &tc);

};


#endif