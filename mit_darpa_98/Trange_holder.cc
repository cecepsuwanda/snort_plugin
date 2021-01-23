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


void Trange_holder::setStart(Tdatetime_holder start)
{
	_start = start;
}

void Trange_holder::setEnd(Tdatetime_holder end)
{
	_end = end;
}

void Trange_holder::setLabel(string label)
{
	_label = label;
}

Tdatetime_holder Trange_holder::getStart()
{
	return _start;
}

Tdatetime_holder Trange_holder::getEnd()
{
	return _end;
}

string Trange_holder::getLabel()
{
	return _label;
}

bool Trange_holder::in_range(Tdatetime_holder start, Tdatetime_holder end)
{
	bool is_in_range = false;

	is_in_range = ((_start<=start) and (start<=_end)) or ((_start<=end) and (end<=_end));

	return is_in_range;
}

ostream & operator << (ostream &out, const Trange_holder &tc)
{
	out << "Start : " << tc._start << " End : " << tc._end << " Label : " << tc._label ;
	return out;
}
