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

			case 28 : {
				
                
                Tip_port_holder ip_port_label(data[28], data[29]);

				std::vector<string> v;
				string tmp_str = it->value;
				v = global_func::tokenizer((char *) tmp_str.c_str(), ":");
				Tip_port_holder ip_port_search(v[0], v[1]);

				

				switch (it->idx_opt)
				{
				case 2 : {
					pass = ip_port_label == ip_port_search;
					break;
				}
				case 3 : {
					
					break;
				}
				}

				break;
			}

			case 30 : {
				Tip_port_holder ip_port_label(data[30], data[31]);

                std::vector<string> v;
				string tmp_str = it->value;
				v = global_func::tokenizer((char *) tmp_str.c_str(), ":");
				Tip_port_holder ip_port_search(v[0], v[1]);

				switch (it->idx_opt)
				{
				case 2 : {
					pass = ip_port_label == ip_port_search;
					break;
				}
				case 3 : {
					
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



