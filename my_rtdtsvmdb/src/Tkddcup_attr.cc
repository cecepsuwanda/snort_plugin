
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


void Tkddcup_attr::add_continuous(int idx, double attr,int digit)
{
   
   Tmy_dttype tmp(to_string(bulat_nol(attr,0.001,digit)), true);
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

void Tkddcup_attr::save_to_db()
{
   Tquery_builder my_query;

   my_query.open_connection();

   string values="";

   for (auto i = _attr.begin(); i != _attr.end(); ++i)
   {
      if(i->second.is_continue())
      {
       values += i->second.get_value() +",";  
    }else{
       values += "'"+i->second.get_value() +"',";
    }

       

   }

   values += "'"+_label+"',";
   values += "'"+_ip_src+"',";
   values += to_string(_port_src)+",";
   values += "'"+_ip_dst+"',";
   values += to_string(_port_dst)+",";
   values += "FROM_UNIXTIME("+to_string(_start) +"),";
   values += "FROM_UNIXTIME("+to_string(_end)+")";


   string str_query = "insert into output(duration,protocol_type,service,flag,src_bytes,dst_bytes,land,wrong_fragment,urgent,count,srv_count,serror_rate,srv_serror_rate,rerror_rate,srv_rerror_rate,same_srv_rate,diff_srv_rate,srv_diff_host_rate,dst_host_count, dst_host_srv_count,dst_host_same_srv_rate,dst_host_diff_srv_rate,dst_host_same_src_port_rate,dst_host_srv_diff_host_rate,dst_host_serror_rate,dst_host_srv_serror_rate,dst_host_rerror_rate,dst_host_srv_rerror_rate,label,ip_src,port_src,ip_dst,port_dst,start_t,end_t) values("+values+")";
   my_query.query(str_query);

   my_query.close_connection();
}

