
#include "Tkddcup_attr.h"

Tkddcup_attr::Tkddcup_attr()
{
   _ip_src = "0.0.0.0";
   _ip_dst = "0.0.0.0";
   _port_src = 0;
   _port_dst = 0;

   _label = "failed";
}


Tkddcup_attr::~Tkddcup_attr()
{
   _attr.clear();
}


void Tkddcup_attr::add_continuous(int idx, double attr, int digit)
{

   Tmy_dttype tmp(to_string(bulat_nol(attr, 0.001, digit)), true);
   _attr.insert({idx, tmp});
}

void Tkddcup_attr::add_non_continuous(int idx, string attr)
{
   Tmy_dttype tmp(attr, false);
   _attr.insert({idx, tmp});
}

void Tkddcup_attr::add_extra(string ip_src, int port_src, string ip_dst, int port_dst, time_t start_t, time_t end_t)
{
   _ip_src = ip_src;
   _ip_dst = ip_dst;
   _port_src = port_src;
   _port_dst = port_dst;

   _start = start_t;
   _end = end_t;
}

void Tkddcup_attr::set_label(string label)
{
   _label = label;
}


bool Tkddcup_attr::is_pass(int idx_attr, int idx_opt, string value)
{

   bool pass = false;

   field_filter tmp;

   tmp.idx_col = idx_attr;
   tmp.idx_opt = idx_opt;
   tmp.value.set_value(value,((idx_opt == 0) or (idx_opt == 1)));

   pass = tmp.is_pass(_attr[idx_attr]);

   // Tmy_dttype tmp(value, ((idx_opt == 0) or (idx_opt == 1)));

   // switch (idx_opt)
   // {
   // case 0 : {
   //    //cout << value1 << " " << value2 << endl;
   //    pass = _attr[idx_attr] <= tmp;
   //    break;
   // }
   // case 1 : {
   //    //cout << value1 << " " << value2 << endl;
   //    pass = tmp < _attr[idx_attr];
   //    break;
   // }
   // case 2 : {
   //    pass = tmp == _attr[idx_attr];
   //    break;
   // }
   // case 3 : {
   //    pass = tmp != _attr[idx_attr];
   //    break;
   // }
   // }

   return pass;

}

void Tkddcup_attr::set_file(string dataset_file, string svm_file)
{
   _dataset_file = dataset_file;
   _svm_file = svm_file;
}

void Tkddcup_attr::save_to_db()
{
   Tquery_builder my_query;

   my_query.open_connection();

   string values = "";

   for (auto i = _attr.begin(); i != _attr.end(); ++i)
   {
      if (i->second.is_continue())
      {
         values += i->second.get_value() + ",";
      } else {
         values += "'" + i->second.get_value() + "',";
      }



   }

   values += "'" + _label + "',";
   values += "'" + _ip_src + "',";
   values += to_string(_port_src) + ",";
   values += "'" + _ip_dst + "',";
   values += to_string(_port_dst) + ",";
   values += "FROM_UNIXTIME(" + to_string(_start) + "),";
   values += "FROM_UNIXTIME(" + to_string(_end) + "),";
   values += "'" + _dataset_file + "',";
   values += "'" + _svm_file + "',";
   values += "now()";



   string str_query = "insert into output(duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count, dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label,ip_src,port_src,ip_dst,port_dst,start_t,end_t,dataset_file,svm_file,create_at) values(" + values + ")";
   my_query.query(str_query);

   my_query.close_connection();
}

vector<string> Tkddcup_attr::get_svm_attr()
{
   vector<string> vec;

   for (size_t i = 0; i < _attr.size(); ++i)
   {
      switch (i) {
      case 1:
         vec.push_back((_attr[i] == "tcp" ? "1" : "0" ));
         vec.push_back((_attr[i] == "udp" ? "1" : "0" ));
         vec.push_back((_attr[i] == "icmp" ? "1" : "0" ));
         break;
      case 2:
         vec.push_back(((_attr[i] == "private") or (_attr[i] == "ecri") or (_attr[i] == "ecr_i") or (_attr[i] == "http")) ? "0" : "1");
         vec.push_back(((_attr[i] == "private") or (_attr[i] == "ecri") or (_attr[i] == "ecr_i") or  (_attr[i] == "http")) ? "1" : "0");
         break;
      case 3:
         vec.push_back((_attr[i] == "SF") ? "0" : "1");
         vec.push_back((_attr[i] == "SF") ? "1" : "0");
         break;
      default:
         vec.push_back(_attr[i].get_string());
         break;
      }

   }

   return vec;
}


double Tkddcup_attr::bulat_nol(double val, double tolerance, int digit)
{
  double tmp = val;

  if (abs(val) < tolerance)
  {
    tmp = 0.0;
  }
  else
  {
    const double multiplier = std::pow(10.0, digit);
    tmp = ceil(val * multiplier) / multiplier;
  }
  // double tmp = val;
  return tmp;
}

