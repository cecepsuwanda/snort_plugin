#include "Tolah_label.h"

Ttgl::Ttgl(int tgl, int bln, int thn)
{
  _tgl = tgl;
  _bln = bln;
  _thn = thn;
}

string Ttgl::cetak()
{
  return to_string(_bln) + "/" + to_string(_tgl) + "/" + to_string(_thn);
}

Ttgl::~Ttgl()
{
}

bool Ttgl::operator<(const Ttgl &tgl_jamObj) const
{
  if (this->_thn < tgl_jamObj._thn)
  {
    return true;
  }
  if (this->_thn > tgl_jamObj._thn)
  {
    return false;
  }

  if (this->_bln < tgl_jamObj._bln)
  {
    return true;
  }
  if (this->_bln > tgl_jamObj._bln)
  {
    return false;
  }

  if (this->_tgl < tgl_jamObj._tgl)
  {
    return true;
  }
  if (this->_tgl > tgl_jamObj._tgl)
  {
    return false;
  }

  return false;
}

Tolah_label::Tolah_label()
{
}

Tolah_label::~Tolah_label()
{
}

void Tolah_label::setnm_f(string nm_f)
{
  _nm_file = nm_f;
}

void Tolah_label::datetime_adjustment(ttanggal &tanggal, twaktu &waktu)
{
  waktu.jam = waktu.jam + 12;
  if (waktu.jam >= 24) {
    waktu.jam = waktu.jam % 24;
    tanggal.tanggal += 1;
    if (tanggal.tanggal > 31) {
      tanggal.tanggal = 1;
      tanggal.bulan += 1;
    }
  }
}

void Tolah_label::baca_file()
{
  FILE *file = NULL;
  file = fopen(_nm_file.c_str(), "r");
  char str[1000];
  int len;

  string field_before = "";

  if (file != NULL)
  {

    while (!feof(file))
    {
      if (fgets(str, 1000, file) != NULL)
      {
        len = strlen(str);
        if (str[len - 1] == '\n')
          str[len - 1] = 0;

        //cout << str << endl;

        if (olah_id(str))
        {
        }
        else
        {
          if (olah_date(str))
          {
          }
          else
          {
            if (olah_name(str))
            {
            }
            else
            {
              if (olah_category(str))
              {
              }
              else
              {
                if (olah_start_time(str))
                {
                }
                else
                {
                  if (olah_duration(str))
                  {
                  }
                  else
                  {
                    if (olah_at_attacker(str))
                    {
                      field_before = "At_Attacker: ";
                    }
                    else
                    {
                      if (olah_at_victim(str))
                      {
                        field_before = "At_Victim: ";
                      }
                      else
                      {
                        if (olah_username(str))
                        {
                        }
                        else
                        {
                          if (olah_attacker(str))
                          {
                          }
                          else
                          {
                            if (olah_victim(str))
                            {
                            }
                            else
                            {

                              string tmp_str = str;

                              size_t posisi = tmp_str.find("Ports:");
                              if (posisi == string::npos)
                              {

                                if (field_before == "At_Attacker: ")
                                {
                                  olah_sisa_at_attacker(str);
                                  field_before = " ";
                                }

                                if (field_before == "At_Victim: ")
                                {
                                  olah_sisa_at_victim(str);
                                  field_before = " ";
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    fclose(file);
  }
  else
  {
    cout << "Gagal Buka File !!!" << endl;
  }

  string tmp_str;

  for (int i = 0; i < vec.size(); ++i)
  {


    for (int j = 0; j < vec[i]->At_Attacker.size(); ++j)
    {

      tmp_str = vec[i]->At_Attacker[j];
      size_t posisi = tmp_str.find("{");
      if (posisi != string::npos)
      {
        tmp_str = tmp_str.substr(0, posisi);
        vec[i]->At_Attacker[j] = tmp_str;
      }
    }

    for (int j = 0; j < vec[i]->At_Victim.size(); ++j)
    {
      tmp_str = vec[i]->At_Victim[j];
      size_t posisi = tmp_str.find("{");
      if (posisi != string::npos)
      {
        tmp_str = tmp_str.substr(0, posisi);
        vec[i]->At_Victim[j] = tmp_str;
      }
    }

    // if ( (vec[i]->Date.tanggal == 31 ) &&  (vec[i]->Date.bulan == 3 ) && (vec[i]->Date.tahun == 1999) )
    // {
    //   cout << vec[i]->Name << endl;
    // }

    datetime_adjustment(vec[i]->Date, vec[i]->Start_Time);

    int start_Time = waktu_to_sec(vec[i]->Start_Time);
    int duration = waktu_to_sec(vec[i]->Duration);

    if ((start_Time + duration) > (24 * 3600))
    {
      int new_duration = (start_Time + duration) - (24 * 3600);

      field_filter *tmp_field = new field_filter;
      tmp_field->ID = vec[i]->ID;
      tmp_field->Date.bulan = (vec[i]->Date.tanggal + 1) > 31 ? vec[i]->Date.bulan + 1 : vec[i]->Date.bulan;
      tmp_field->Date.tanggal = (vec[i]->Date.tanggal + 1) > 31 ? ((vec[i]->Date.tanggal + 1) % 31) : vec[i]->Date.tanggal + 1;
      tmp_field->Date.tahun = vec[i]->Date.tahun;
      tmp_field->Name = vec[i]->Name;
      tmp_field->Category = vec[i]->Category;

      tmp_field->Start_Time.jam = 0;
      tmp_field->Start_Time.menit = 0;
      tmp_field->Start_Time.detik = 0;

      tmp_field->Duration.jam = new_duration / 3600;
      tmp_field->Duration.menit = (new_duration % 3600) / 60;
      tmp_field->Duration.detik = (new_duration % 3600) % 60;

      tmp_field->Attacker = vec[i]->Attacker;
      tmp_field->Victim = vec[i]->Victim;
      tmp_field->Username = vec[i]->Username;
      tmp_field->At_Attacker = vec[i]->At_Attacker;
      tmp_field->At_Victim = vec[i]->At_Victim;
      insert_vec_map(tmp_field);
    }

    insert_vec_map(vec[i]);

  }
}

void Tolah_label::insert_vec_map(field_filter *field)
{
  Ttgl new_tgl(field->Date.tanggal, field->Date.bulan, field->Date.tahun);
  auto itr = vec_map.find(new_tgl);
  if (itr == vec_map.end())
  {
    vector<field_filter *> tmp_vec;
    tmp_vec.push_back(field);
    vec_map.insert({new_tgl, tmp_vec});
  }
  else
  {
    vec_field_filter *tmp_vec = &itr->second;
    tmp_vec->push_back(field);

  }
}

vector<string> Tolah_label::tokenizer(char *str, const char *separator)
{
  char *token;
  vector<string> vec;
  int len;

  len = strlen(str);
  if (str[len - 1] == '\n')
    str[len - 1] = 0;

  token = strtok(str, separator);
  while (token != 0)
  {
    //cout << token << endl;
    vec.push_back(token);
    token = strtok(0, separator);
  }

  return vec;
}

bool Tolah_label::olah_id(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("ID: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    //cout << str << endl;
    field = new field_filter;
    vec.push_back(field);

    tmp_str = tmp_str.substr(4);
    //cout << tmp_str << endl;

    field->ID = tmp_str;
  }

  return is_pass;
}

bool Tolah_label::olah_date(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Date: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    tmp_str = tmp_str.substr(6);
    //cout << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), "/");

    //cout << data[0] << "/" << data[1] << "/" << data[2] << endl;

    field->Date.bulan = stoi(data[0]);
    field->Date.tanggal = stoi(data[1]);
    field->Date.tahun = stoi(data[2]);
  }

  return is_pass;
}

bool Tolah_label::olah_name(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  if (is_subs(tmp_str, "Name: "))
  {
    is_pass = true;
    tmp_str = tmp_str.substr(6);

    if (is_subs(tmp_str, "("))
    {
      //cout << tmp_str << endl;
      size_t posisi1 = tmp_str.find("(");
      size_t posisi2 = tmp_str.find(")");
      tmp_str = tmp_str.substr(posisi1 + 1, (posisi2 - posisi1) - 1);
      //cout << tmp_str << endl;
    }

    field->Name = tmp_str;
  }

  return is_pass;
}

bool Tolah_label::olah_category(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Category: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    tmp_str = tmp_str.substr(10);
    //cout << tmp_str << endl;

    field->Category = tmp_str;
  }

  return is_pass;
}

bool Tolah_label::olah_start_time(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Start_Time: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    tmp_str = tmp_str.substr(12);

    //cout << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ":");

    field->Start_Time.jam = stoi(data[0]);
    field->Start_Time.menit = stoi(data[1]);
    field->Start_Time.detik = stoi(data[2]);

    //cout << field->Start_Time.jam << ":"<< field->Start_Time.menit << ":"<< field->Start_Time.detik <<endl;
  }
  return is_pass;
}

bool Tolah_label::olah_duration(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Duration: ");
  if (posisi != string::npos)
  {
    is_pass = true;

    tmp_str = tmp_str.substr(10);

    //cout << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ":");

    field->Duration.jam = stoi(data[0]);
    field->Duration.menit = stoi(data[1]);
    field->Duration.detik = stoi(data[2]);

    //cout << field->Duration.jam << ":"<< field->Duration.menit << ":"<< field->Duration.detik <<endl;
  }

  return is_pass;
}

bool Tolah_label::olah_at_attacker(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("At_Attacker: ");
  if (posisi != string::npos)
  {
    is_pass = true;

    //cout << "At_Attacker " << tmp_str << endl;
    tmp_str = tmp_str.substr(posisi + 13);
    //cout << "At_Attacker " << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      field->At_Attacker.push_back(tmp_str);
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if (tmp_str != "\0")
        {
          field->At_Attacker.push_back(tmp_str);
        }
      }
    }
  }

  return is_pass;
}

bool Tolah_label::olah_at_victim(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("At_Victim: ");
  if (posisi != string::npos)
  {
    is_pass = true;

    //cout << "At_Victim " << tmp_str << endl;
    tmp_str = tmp_str.substr(posisi + 11);
    //cout << "At_Victim " <<  tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      field->At_Victim.push_back(tmp_str);
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if (tmp_str != "\0")
        {
          field->At_Victim.push_back(tmp_str);
        }
      }
    }
  }

  return is_pass;
}

bool Tolah_label::olah_username(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Username: ");
  if (posisi != string::npos)
  {
    is_pass = true;

    //cout << str << endl;
    tmp_str = tmp_str.substr(10);
    //cout << tmp_str << endl;

    field->Username = tmp_str;
  }

  return is_pass;
}

bool Tolah_label::olah_attacker(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Attacker: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    //cout << tmp_str << endl;
    tmp_str = tmp_str.substr(10);

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      if ((tmp_str.length() > 0) and (tmp_str != "\0"))
      {
        field->Attacker.push_back(tmp_str);
      }
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if ((tmp_str.length() > 0) and (tmp_str != "\0"))
        {
          field->Attacker.push_back(tmp_str);
        }
      }
    }
  }

  return is_pass;
}

bool Tolah_label::olah_victim(char *str)
{
  bool is_pass = false;

  string tmp_str = str;

  size_t posisi = tmp_str.find("Victim: ");
  if (posisi != string::npos)
  {
    is_pass = true;
    //cout << str << endl;
    tmp_str = tmp_str.substr(8);

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      if ((tmp_str.length() > 0) and (tmp_str != "\0"))
      {
        field->Victim.push_back(tmp_str);
      }
    }
    else
    {

      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if ((tmp_str.length() > 0) and (tmp_str != "\0"))
        {
          field->Victim.push_back(tmp_str);
        }
      }
    }
  }

  return is_pass;
}

void Tolah_label::olah_sisa_at_attacker(char *str)
{

  string tmp_str = str;

  //cout <<"At_Attacker: "<< str << endl;
  int len = strlen(str);
  if (len > 1)
  {
    tmp_str = tmp_str.substr(1);
    //cout << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      if ((tmp_str.length() > 0) and (tmp_str != "\0"))
      {
        field->At_Attacker.push_back(tmp_str);
      }
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if ((tmp_str.length() > 0) and (tmp_str != "\0"))
        {
          field->At_Attacker.push_back(tmp_str);
        }
      }
    }
  }
}

void Tolah_label::olah_sisa_at_victim(char *str)
{

  string tmp_str = str;

  //cout << "At_Victim: " << str << endl;
  int len = strlen(str);
  if (len > 1)
  {
    tmp_str = tmp_str.substr(1);
    //cout << tmp_str << endl;

    vector<string> data = tokenizer((char *)tmp_str.c_str(), ", ");

    if (data.size() == 0)
    {
      if ((tmp_str.length() > 0) and (tmp_str != "\0"))
      {
        field->At_Victim.push_back(tmp_str);
      }
    }
    else
    {
      for (int i = 0; i < data.size(); ++i)
      {
        tmp_str = data[i];
        if ((tmp_str.length() > 0) and (tmp_str != "\0"))
        {
          field->At_Victim.push_back(tmp_str);
        }
      }
    }
  }
}

ttanggal Tolah_label::date_frag(string date_str)
{
  ttanggal tmp;

  vector<string> data = tokenizer((char *)date_str.c_str(), "/");

  tmp.bulan = stoi(data[0]);
  tmp.tanggal = stoi(data[1]);
  if (data.size() == 3)
  {
    tmp.tahun = stoi(data[2]);
  }

  return tmp;
}

twaktu Tolah_label::waktu_frag(string waktu_str)
{
  twaktu tmp;

  vector<string> data = tokenizer((char *)waktu_str.c_str(), ":");

  tmp.jam = stoi(data[0]);
  tmp.menit = stoi(data[1]);

  string tmp_detik = data[2];
  size_t posisi = tmp_detik.find(".");
  if (posisi == string::npos)
  {
    tmp.detik = stoi(tmp_detik);
  }
  else
  {
    data = tokenizer((char *)tmp_detik.c_str(), ".");
    tmp.detik = stoi(data[0]);
  }

  return tmp;
}

bool Tolah_label::is_subs(string stack, string needle)
{
  bool is_pass = false;

  size_t posisi = stack.find(needle);
  is_pass = posisi != string::npos;

  return is_pass;
}

bool Tolah_label::isNumber(std::string token)
{
  return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

tip_fragment Tolah_label::ip_frag(string ip_port)
{
  tip_fragment tmp;
  vector<string> data;

  if (is_subs(ip_port, ":"))
  {
    data = tokenizer((char *)ip_port.c_str(), ":");
    tmp.port = stoi(data[1]);
    string ip = data[0];
    data = tokenizer((char *)ip.c_str(), ".");
    tmp.part1 = isNumber(data[0]) ? data[0] : "0";
    tmp.part2 = isNumber(data[1]) ? data[1] : "0";
    tmp.part3 = isNumber(data[2]) ? data[2] : "0";
    tmp.part4 = isNumber(data[3]) ? data[3] : "0";
  }
  else
  {
    if (is_subs(ip_port, "."))
    {
      data = tokenizer((char *)ip_port.c_str(), ".");
      tmp.part1 = isNumber(data[0]) ? data[0] : "0";
      tmp.part2 = isNumber(data[1]) ? data[1] : "0";
      tmp.part3 = isNumber(data[2]) ? data[2] : "0";
      tmp.part4 = isNumber(data[3]) ? data[3] : "0";
    }
    else
    {
      tmp.part1 = "0";
      tmp.part2 = "0";
      tmp.part3 = "0";
      tmp.part4 = "0";
    }
  }

  return tmp;
}

bool Tolah_label::compare_ip(tip_fragment ip1, tip_fragment ip2)
{
  bool is_pass = false;
  vector<string> data;

  if (stoi(ip1.part1) == stoi(ip2.part1))
  {
    if (stoi(ip1.part2) == stoi(ip2.part2))
    {
      if (stoi(ip1.part3) == stoi(ip2.part3))
      {
        if (is_subs(ip2.part4, "*"))
        {
          is_pass = true;
        }
        else
        {
          if (is_subs(ip2.part4, "-"))
          {
            data = tokenizer((char *)ip2.part4.c_str(), "-");
            //cout << data[0] << "<=" << ip1.part4 << "<=" << data[1] << endl;
            is_pass = (stoi(ip1.part4) >= stoi(data[0])) and (stoi(ip1.part4) <= stoi(data[1]));
          }
          else
          {
            is_pass = stoi(ip1.part4) == stoi(ip2.part4);
          }
        }
      }
    }
  }

  return is_pass;
}

bool Tolah_label::compare_port(int port1, string protocol, string port2)
{
  bool is_pass = false;
  vector<string> data;

  if (is_subs(port2, "-"))
  {
    data = tokenizer((char *)port2.c_str(), "-");
    string tmp_port1 = data[0];
    string tmp_port2 = data[1];

    if (is_subs(tmp_port1, "/"))
    {
      data = tokenizer((char *)tmp_port1.c_str(), "/");
      string tmp_port = data[0];
      if (isNumber(tmp_port))
      {
        is_pass = port1 >= stoi(tmp_port);
        if (is_pass)
        {
          if (data[1] == "u")
          {
            is_pass = protocol == "udp";
            //cout<< port2 << " masuk u " << endl;
          }
        }
      }
    }
    else
    {
      if (isNumber(tmp_port1))
      {
        is_pass = port1 >= stoi(tmp_port1);
      }
    }

    if (is_subs(tmp_port2, "/"))
    {
      data = tokenizer((char *)tmp_port2.c_str(), "/");
      string tmp_port = data[0];
      if (isNumber(tmp_port))
      {
        is_pass = is_pass and (port1 <= stoi(tmp_port));
        if (is_pass)
        {
          if (data[1] == "u")
          {
            is_pass = protocol == "udp";
            //cout<< port2 << " masuk u " << endl;
          }
        }

      }
    }
    else
    {
      if (isNumber(tmp_port2))
      {
        is_pass = is_pass and (port1 <= stoi(tmp_port2));
      }

    }
  }
  else
  {
    if (is_subs(port2, "/"))
    {
      data = tokenizer((char *)port2.c_str(), "/");
      string tmp_port = data[0];
      if (isNumber(tmp_port))
      {
        is_pass = port1 == stoi(tmp_port);
        if (is_pass)
        {
          if (data[1] == "u")
          {
            is_pass = protocol == "udp";
            //cout<< port2 << " masuk u " << endl;
          }
        }
      }

    }
    else
    {
      if (isNumber(port2))
      {
        is_pass = port1 == stoi(port2);
      }
      else
      {
        if (port2 == "i")
        {
          is_pass = protocol == "icmp";
          if (is_pass)
          {
            //cout<< port2 << " masuk i " << endl;
          }
        }
      }
    }
  }

  //cout << port1 << " " << port2 << " " << is_pass << endl;

  return is_pass;
}

int Tolah_label::waktu_to_sec(twaktu waktu)
{
  return (waktu.jam * 3600) + (waktu.menit * 60) + waktu.detik;
}

bool Tolah_label::is_date_pass(vector<string> &row, field_filter *field)
{
  bool is_pass = false;

  string tmp_row = row[0];

  vector<string> data = tokenizer((char *)tmp_row.c_str(), "-");
  ttanggal tmp = date_frag(data[0]);

  //cout << tmp.tanggal << " " << tmp.bulan << endl;
  //cout << field->Date.tanggal << " " << field->Date.bulan << endl;

  is_pass = ((field->Date.tanggal == tmp.tanggal) and (field->Date.bulan == tmp.bulan));

  return is_pass;
}

bool Tolah_label::is_waktu_pass(vector<string> &row, field_filter *field)
{
  bool is_pass = false;

  string tmp_row = row[0];

  vector<string> data = tokenizer((char *)tmp_row.c_str(), "-");
  twaktu tmp = waktu_frag(data[1]);

  int tmp_sec = waktu_to_sec(tmp);
  int start_time_sec = waktu_to_sec(field->Start_Time);
  int duration_sec = waktu_to_sec(field->Duration);

  if (tmp_sec >= start_time_sec)
  {
    is_pass = (tmp_sec - start_time_sec) <= duration_sec;
  }

  return is_pass;
}

bool Tolah_label::is_ip_pass(string ip_src, string ip_dst, string protocol, vector<string> &ip_attacker, vector<string> &ip_victim, vector<string> &port_attacker, vector<string> &port_victim)
{
  bool is_pass = false;


  tip_fragment tmp_ip_src = ip_frag(ip_src);
  tip_fragment tmp_ip_dst = ip_frag(ip_dst);

  tmp_ip_src.protocol = protocol;
  tmp_ip_dst.protocol = protocol;

  bool is_ip_src_attacker = is_ip_attacker(tmp_ip_src, ip_attacker);
  bool is_ip_dst_attacker = is_ip_attacker(tmp_ip_dst, ip_attacker);;
  bool is_ip_src_victim = is_ip_victim(tmp_ip_src, ip_victim);
  bool is_ip_dst_victim = is_ip_victim(tmp_ip_dst, ip_victim);

  bool is_port_src_attacker = is_port_attacker(tmp_ip_src, port_attacker);
  bool is_port_dst_attacker = is_port_attacker(tmp_ip_dst, port_attacker);
  bool is_port_src_victim = is_port_victim(tmp_ip_src, port_victim);
  bool is_port_dst_victim = is_port_victim(tmp_ip_dst, port_victim);

  if ((port_attacker.size() > 0) && (port_victim.size() > 0)) {
    if ((is_ip_src_attacker && is_ip_dst_victim) || (is_ip_dst_attacker && is_ip_src_victim) )
    {
      if ((is_port_src_attacker && is_port_dst_victim) || (is_port_dst_attacker && is_port_src_victim))
      {
        is_pass = true;
      } else {
        if (is_port_src_attacker || is_port_dst_victim || is_port_dst_attacker || is_port_src_victim)
        {
          is_pass = true;
        }
      }
    } else {
      if ((is_ip_src_attacker && ( is_port_src_attacker || is_port_src_victim) ) || (is_ip_src_victim && ( is_port_src_attacker || is_port_src_victim) ))
      {
        is_pass = true;
      } else {
        if ((is_ip_dst_attacker && ( is_port_dst_attacker || is_port_dst_victim) ) || (is_ip_dst_victim && ( is_port_dst_attacker || is_port_dst_victim) ))
        {
          is_pass = true;
        }
      }
    }
  } else {
    if ((port_attacker.size() == 0) && (port_victim.size() == 0))
    {
      if (is_ip_src_attacker || is_ip_dst_victim || is_ip_dst_attacker || is_ip_src_victim)
      {
        is_pass = true;
      }
    } else {
      if (port_victim.size() > 0)
      {
        if (((is_ip_src_attacker || is_ip_src_victim ) && is_port_src_victim) || ((is_ip_dst_attacker || is_ip_dst_victim ) && is_port_dst_victim))
        {
          is_pass = true;
        }
      } else {
        if (port_attacker.size() > 0)
        {
          if (((is_ip_src_attacker || is_ip_src_victim ) && is_port_src_attacker) || ((is_ip_dst_attacker || is_ip_dst_victim ) && is_port_dst_attacker))
          {
            is_pass = true;
          }
        }
      }
    }
  }

  return is_pass;
}


bool Tolah_label::is_ip_attacker(tip_fragment ip, vector<string> &ip_attacker)
{
  bool is_pass = false;

  if (ip_attacker.size() > 0)
  {
    int j = 0;
    while ((!is_pass) and (j < ip_attacker.size()))
    {

      tip_fragment tmp = ip_frag(ip_attacker[j]);
      is_pass = compare_ip(ip, tmp);
      j++;
    }
  }

  return is_pass;

}

bool Tolah_label::is_ip_victim(tip_fragment ip, vector<string> &ip_victim)
{
  bool is_pass = false;

  if (ip_victim.size() > 0)
  {
    int j = 0;

    while ((!is_pass) and (j < ip_victim.size()))
    {
      tip_fragment tmp = ip_frag(ip_victim[j]);
      is_pass = compare_ip(ip, tmp);
      j++;
    }
  }

  return is_pass;
}

bool Tolah_label::is_port_attacker(tip_fragment ip, vector<string> &port_attacker)
{
  bool is_pass = false;

  if (port_attacker.size() > 0)
  {
    int j = 0;
    while ((!is_pass) and (j < port_attacker.size()))
    {
      string tmp = port_attacker[j];
      is_pass = compare_port(ip.port, ip.protocol, tmp);
      j++;
    }
  }

  return is_pass;
}

bool Tolah_label::is_port_victim(tip_fragment ip, vector<string> &port_victim)
{
  bool is_pass = false;

  if (port_victim.size() > 0)
  {
    int j = 0;
    while ((!is_pass) and (j < port_victim.size()))
    {
      string tmp = port_victim[j];
      is_pass = compare_port(ip.port, ip.protocol, tmp);
      j++;
    }
  }

  return is_pass;
}



string Tolah_label::labeli(vector<string> row)
{

  bool is_cetak = false;
  string label = "normal."; //"normal,Nan";

  string tmp_row = row[0];

  vector<string> data = tokenizer((char *)tmp_row.c_str(), "-");
  ttanggal tmp = date_frag(data[0]);

  Ttgl new_tgl(tmp.tanggal, tmp.bulan, 1999);
  auto itr = vec_map.find(new_tgl);
  if (itr != vec_map.end())
  {
    vec_field_filter *tmp_vec = &itr->second;
    auto itr1 = tmp_vec->begin();
    while ((!is_cetak) and (itr1 != tmp_vec->end()))
    {
      field_filter *tmp_field = *itr1;

      if (is_waktu_pass(row, tmp_field))
      {

        if (is_ip_pass(row[1], row[2], row[4], tmp_field->Attacker, tmp_field->Victim, tmp_field->At_Attacker, tmp_field->At_Victim))
        {
          is_cetak = true;
          label = tmp_field->Name + ".";
        }

      }

      itr1++;
    }
  }


  return label;
}
