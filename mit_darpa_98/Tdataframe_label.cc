#include "Tdataframe_label.h"


bool Tdataframe_label::is_pass(vector<string> &data) 
{
	bool pass = true;
    if (_filter.size() > 0)
	{		
		vector<field_filter>::iterator it = _filter.begin();
		while ((it != _filter.end()) and pass)
		{
			switch (it->idx_col)
			{
			case 36 : {
				switch (it->idx_opt)
				{
				case 2 : {
					pass = data[36] == it->value;
					break;
				}
				case 3 : {
					pass = data[36] != it->value;
					break;
				}
				}

				break;
			}
			}
			++it;
		}
	}

	return pass;
}



