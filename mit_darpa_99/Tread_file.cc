#include "Tread_file.h"


Tread_file::Tread_file()
{

}

Tread_file::~Tread_file()
{
  clear_data();
  close_file();
}

Tread_file::Tread_file(const Tread_file &t)
{
  cout << "Copy constructor called " << endl;
}



void Tread_file::setnm_f(string nm_f)
{
  _nm_f = nm_f;
}

void Tread_file::setseparator(const char* separator)
{
  _separator = separator;
}

bool Tread_file::open_file(string mode)
{
  _file = fopen(_nm_f.c_str(), (char *) mode.c_str());
  return !(_file == NULL);
}

bool Tread_file::open_file()
{
  _file = fopen(_nm_f.c_str(), "r");
  return !(_file == NULL);
}

void Tread_file::close_file()
{
  if (!(_file == NULL))
  {
    fclose(_file);
    _file = NULL;
  }
}

vector<string> Tread_file::tokenizer(char * str, const char* separator)
{
  char *token;
  vector<string> vec;
  int len;

  len = strlen(str);
  if (str[len - 1] == '\n')
    str[len - 1] = 0;

  token = strtok(str, separator);
  while (token != 0) {
    vec.push_back(token);
    token = strtok(0, separator);
  }

  return vec;
}

void Tread_file::read_file()
{
  char str[1000];
  clear_data();

  if (fgets(str, 1000, _file) != NULL)
  {
    _data = tokenizer(str, _separator);
  }
}

void Tread_file::write_file(string row)
{
  fputs((char *) row.c_str(), _file);
  fputc('\n', _file);
}

void Tread_file::clear_data()
{
  _data.clear();
  _data.shrink_to_fit();
}


bool Tread_file::is_eof()
{
  return feof(_file);

}

vector<string> Tread_file::get_record()
{
  return _data;
}

void Tread_file::next_record()
{
  read_file();
}
