#include "Tdataframe_darpa.h"


Tdataframe_darpa::Tdataframe_darpa()
{

}

Tdataframe_darpa::~Tdataframe_darpa()
{

}

void Tdataframe_darpa::read_data(string nm_f)
{
	_nm_file = nm_f;
	_data.setnm_f(nm_f);
}
