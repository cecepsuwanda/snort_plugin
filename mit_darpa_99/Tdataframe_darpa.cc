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

	stat_tabel();
}

void Tdataframe_darpa::stat_tabel()
{

	if (open_file())
	{

		vector<string> tmp_data;
		read_file();
		while (!is_eof())
		{
			tmp_data = get_record();

			for (int i = 0; i < tmp_data.size(); ++i)
			{
				
			}

			tmp_data.clear();
			tmp_data.shrink_to_fit();

			next_record();
		}
		close_file();
	} else {
		cout << "stat_tabel, Gagal buka file !!!" << endl;
	}
}


bool Tdataframe_darpa::open_file()
{
	return _data.open_file();
}

void Tdataframe_darpa::read_file()
{
	_data.read_file();
}

void Tdataframe_darpa::close_file()
{
	_data.close_file();
}

bool Tdataframe_darpa::is_eof()
{
	return _data.is_eof();
}

void Tdataframe_darpa::next_record()
{
	_data.next_record();

}

vector<string> Tdataframe_darpa::get_record()
{
	vector<string> tmp_data = _data.get_record();
    string tmp_str;
    int idx;

	for (size_t i = 0; i < tmp_data.size(); ++i)
	{
		size_t find = tmp_data[i].find(":");
        if(find==string::npos)
        {          
          if(tmp_str[tmp_str.size() - 1] == '\n')
          {
          	tmp_str.erase(tmp_str.size() - 1);
          }

          tmp_data[i]=tmp_data[i].substr(1,tmp_data[i].size());         

          tmp_str += tmp_data[i];
          tmp_data[idx]=tmp_str;         
        }else{
        	tmp_str = tmp_data[i];
        	idx = i; 
        }  
		
	}


	return tmp_data;
}
