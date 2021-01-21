#include "Trange_holder.h"

Trange_holder::Trange_holder()
{
	
}

Trange_holder::Trange_holder(Tdatetime_holder start, Tdatetime_holder end, string label)
{
	_start = start;
	_end = end;
	_label = label;
}

Trange_holder::~Trange_holder()
{

}

ostream & operator << (ostream &out, const Trange_holder &tc)
{
	out << "Start : " << tc._start << " End : " << tc._end << " Label : " << tc._label ;
	return out;
}
