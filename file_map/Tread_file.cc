#include "Tread_file.h"


Tread_file::Tread_file()
{

}

Tread_file::~Tread_file()
{
  clear_data();
  close_file();
  clear_index();
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

void Tread_file::file_map()
{
  is_fmap = true;
}

bool Tread_file::open_file(string mode)
{
  _file = fopen(_nm_f.c_str(), (char *) mode.c_str());
  return !(_file == NULL);
}

bool Tread_file::open_file()
{
  if (is_fmap)
  {
    _fd = open(_nm_f.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);

    if (fstat(_fd, &_sb) == -1)
    {
      perror("couldn't get file size. \n");
    }

    //printf("file size is %ld\n",_sb.st_size);

    _file_in_memory = (char*) mmap(NULL, _sb.st_size, PROT_READ, MAP_PRIVATE, _fd, 0);
    _posisi = 0;
    _idx_posisi = 0;

    return !(_fd == -1);
  } else {
    _file = fopen(_nm_f.c_str(), "r");
    return !(_file == NULL);
  }
}

void Tread_file::close_file()
{
  if (is_fmap)
  {
    munmap(_file_in_memory, _sb.st_size);
    close(_fd);
  } else {

    if (!(_file == NULL))
    {
      fclose(_file);
      _file = NULL;
    }
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
  clear_data();
  char str[1000];
  if (is_fmap)
  {
    strcpy(str, "");

    if (is_index and (_index.size() > 0))
    {
      int p_awal = _index[_idx_posisi];

      while (( p_awal < _sb.st_size) and (_file_in_memory[p_awal] != '\n') )
      {
        char tmp = _file_in_memory[p_awal];
        strncat(str, &tmp, 1);
        p_awal++;
      }
      char tmp = _file_in_memory[p_awal];
      strncat(str, &tmp, 1);
      _idx_posisi++;

    } else {

      _b_posisi = _posisi;
      while (( _posisi < _sb.st_size) and (_file_in_memory[_posisi] != '\n') )
      {
        char tmp = _file_in_memory[_posisi];
        strncat(str, &tmp, 1);
        _posisi++;
      }
      char tmp = _file_in_memory[_posisi];
      strncat(str, &tmp, 1);
      _posisi++;
    }
    _data = tokenizer(str, _separator);
  } else {

    if (fgets(str, 1000, _file) != NULL)
    {
      //cout << str << endl;
      _data = tokenizer(str, _separator);
    }
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

void Tread_file::clear_index()
{
  _index.clear();
  _index.shrink_to_fit();
}


bool Tread_file::is_eof()
{
  if (is_fmap)
  {
     if(is_index)
     {
       return (_idx_posisi > _index.size());
     }
     else
     {
       return (_posisi == _sb.st_size);
     }

  } else {
    return feof(_file);
  }

}

vector<string> Tread_file::get_record()
{
  return _data;
}

void Tread_file::next_record()
{
  read_file();
}

void Tread_file::index_on()
{
  is_index=true;
}

void Tread_file::index_off()
{
  is_index=false;
}

void Tread_file::add_index()
{
  _index.push_back(_b_posisi);
}
