//--------------------------------------------------------------------------
// Copyright (C) 2014-2020 Cisco and/or its affiliates. All rights reserved.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License Version 2 as published
// by the Free Software Foundation.  You may not use, modify or distribute
// this program under any other version of the GNU General Public License.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//--------------------------------------------------------------------------
// dpx.cc author Russ Combs <rcombs@sourcefire.com>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "detection/detection_engine.h"
#include "events/event_queue.h"
#include "framework/inspector.h"
#include "framework/module.h"
#include "log/messages.h"
#include "profiler/profiler.h"

#include "protocols/packet.h"
#include "protocols/tcp.h"
#include "protocols/udp.h"
#include "protocols/icmp4.h"

#include "log/log.h"
#include "log/log_text.h"
#include "log/text_log.h"

#include "my_dt/Tdec_tree.h"

#include "extractor/IpFragment.h"
#include "extractor/StatsEngine.h"
#include "extractor/ConversationReconstructor.h"
#include "extractor/Config.h"
#include "extractor/IpReassembler.h"
#include "extractor/FiveTuple.h"

#include "utils/util.h"

using namespace snort;
using namespace std;

#define DPX_GID 256
#define DPX_SID 1

static const char* s_name = "dpx";
static const char* s_help = "dynamic inspector example";

static THREAD_LOCAL ProfileStats dpxPerfStats;

static THREAD_LOCAL SimpleStats dpxstats;

#define LOG_BUFFER (4*K_BYTES)

static THREAD_LOCAL TextLog* csv_log;

class TPkey
{
private:
  uint32_t src;
  uint32_t dst;
  FeatureExtractor::ip_field_protocol_t proto;
  uint16_t id;
public:
  TPkey(const FeatureExtractor::IpFragment *fragment);
  ~TPkey();
  bool operator<(const TPkey& other) const;
};

TPkey::TPkey(const FeatureExtractor::IpFragment *fragment)
{
  this->src = fragment->get_src_ip();
  this->dst = fragment->get_dst_ip();
  this->proto = fragment->get_ip_proto();
  this->id = fragment->get_ip_id();
}

bool TPkey::operator<(const TPkey& other) const
{
  if (src < other.src)
    return true;
  if (src > other.src)
    return false;

  // src IPs are equal
  if (dst < other.dst)
    return true;
  if (dst > other.dst)
    return false;

  // dst IPs are equal
  if (id < other.id)
    return true;
  if (id > other.id)
    return false;

  // IDs are equal
  return (proto < other.proto);
}


//-------------------------------------------------------------------------
// class stuff
//-------------------------------------------------------------------------

class Dpx : public Inspector
{
public:
  Dpx(string f_model, string f_log);

  void show(const SnortConfig*) const override;
  void eval(Packet*) override;

  void tinit() override;
  void tterm() override;

private:
  /*uint16_t port;
  uint16_t max;*/

  string file_model;
  string file_log;

  Tdec_tree dec_tree;
  Tdataframe df_save;

  FeatureExtractor::Config *config;
  FeatureExtractor::IpReassembler *reasm;
  FeatureExtractor::ConversationReconstructor *conv_reconstructor;
  FeatureExtractor::StatsEngine *stats_engine;

  vector<string> get_attr(FeatureExtractor::ConversationFeatures *cf) ;
  void save_csv(FeatureExtractor::ConversationFeatures *cf, string label);

  FeatureExtractor::IpFragment *getIpFragment(Packet *);
};

Dpx::Dpx(string f_model, string f_log)
{
  /*port = p;
  max = m;*/

  file_model = f_model;
  file_log = f_log;

  string path = SnortConfig::get_conf()->log_dir;
  
  cout << file_model << endl;

  df_save.read_data(path + "/" + file_model); //"/dc_tree.csv"
  dec_tree.read_tree(df_save);

  config = new FeatureExtractor::Config();
  stats_engine = new FeatureExtractor::StatsEngine(config);
  reasm = new FeatureExtractor::IpReassembler();
  conv_reconstructor = new FeatureExtractor::ConversationReconstructor();
}

void Dpx::show(const SnortConfig*) const
{
  /*ConfigLogger::log_value("file_model", file_model);
  ConfigLogger::log_value("file_log", file_log);*/
}

vector<string> Dpx::get_attr(FeatureExtractor::ConversationFeatures *cf) {

  vector<string> tmp_vec;

  tmp_vec.push_back(to_string(cf->get_conversation()->get_duration_ms() / 1000)) ;//<< "duration
  tmp_vec.push_back(cf->get_conversation()->get_protocol_type_str()) ;//<< "protocol_type  : "
  tmp_vec.push_back(cf->get_conversation()->get_service_str());//<< "service        : "
  tmp_vec.push_back(cf->get_conversation()->get_state_str());//<< "flag           : "
  tmp_vec.push_back(to_string(cf->get_conversation()->get_src_bytes()));//<< "src_bytes      : "
  tmp_vec.push_back(to_string(cf->get_conversation()->get_dst_bytes()));//<< "dst_bytes      : "
  tmp_vec.push_back(to_string(cf->get_conversation()->land()));//<< "land           : "
  tmp_vec.push_back(to_string(cf->get_conversation()->get_wrong_fragments()));//<< "wrong_fragment : "
  tmp_vec.push_back(to_string(cf->get_conversation()->get_urgent_packets()));//<< "urgent         : "
  tmp_vec.push_back(to_string(cf->get_count()));//<< "count             : "
  tmp_vec.push_back(to_string(cf->get_srv_count()));//<< "srv_count         : "
  tmp_vec.push_back(to_string(cf->get_serror_rate())); //<< "serror_rate       : "
  tmp_vec.push_back(to_string(cf->get_srv_serror_rate())); //<< "srv_serror_rate   : "
  tmp_vec.push_back(to_string(cf->get_rerror_rate())); //<< "rerror_rate       : "
  tmp_vec.push_back(to_string(cf->get_srv_rerror_rate()));//<< "srv_rerror_rate   : "
  tmp_vec.push_back(to_string(cf->get_same_srv_rate())); //<< "same_srv_rate     : "
  tmp_vec.push_back(to_string(cf->get_diff_srv_rate())); //<< "diff_srv_rate     : "
  tmp_vec.push_back(to_string(cf->get_srv_diff_host_rate())); //<< "srv_diff_host_rate : "
  tmp_vec.push_back(to_string(cf->get_dst_host_count())); //<< "dst_host_count     : "
  tmp_vec.push_back(to_string(cf->get_dst_host_srv_count())); //<< "dst_host_srv_count : "
  tmp_vec.push_back(to_string(cf->get_dst_host_same_srv_rate())); //<< "dst_host_same_srv_rate : "
  tmp_vec.push_back(to_string(cf->get_dst_host_diff_srv_rate())); //<< "dst_host_diff_srv_rate : "
  tmp_vec.push_back(to_string(cf->get_dst_host_same_src_port_rate())); //<< "dst_host_same_src_port_rate : "
  tmp_vec.push_back(to_string(cf->get_dst_host_srv_diff_host_rate())); //<< "dst_host_srv_diff_host_rate : "
  tmp_vec.push_back(to_string(cf->get_dst_host_serror_rate()));//<< "dst_host_serror_rate        : "
  tmp_vec.push_back(to_string(cf->get_dst_host_srv_serror_rate())); //<< "dst_host_srv_serror_rate    : "
  tmp_vec.push_back(to_string(cf->get_dst_host_rerror_rate()));//<< "dst_host_rerror_rate        : "
  tmp_vec.push_back(to_string(cf->get_dst_host_srv_rerror_rate())); //<< "dst_host_srv_rerror_rate    : "
  return tmp_vec;
}

void Dpx::save_csv(FeatureExtractor::ConversationFeatures *cf, string label) {

  string sep = ",";
  //ofstream myfile;
  //string path = SnortConfig::get_conf()->log_dir;
  //myfile.open (path + "/alert_dpx.csv", fstream::in | fstream::out | fstream::app);

  const FeatureExtractor::FiveTuple *ft = cf->get_conversation()->get_five_tuple_ptr();

  uint32_t src_ip = ft->get_src_ip();
  uint32_t dst_ip = ft->get_dst_ip();
  uint8_t *sip = (uint8_t *)&src_ip;
  uint8_t *dip = (uint8_t *)&dst_ip;

  int localzone = SnortConfig::get_conf()->thiszone;
  FeatureExtractor::Timestamp ts = cf->get_conversation()->get_start_ts();
  struct timeval tvp = ts.get_timeval();

  int s = (tvp.tv_sec + localzone) % SECONDS_PER_DAY;
  time_t Time = (tvp.tv_sec + localzone) - s;

  struct tm ttm;
  struct tm* lt = gmtime_r(&Time, &ttm);

  string tmp_str = to_string(lt->tm_mon + 1) + "/" + to_string(lt->tm_mday) + "-" + to_string(s / 3600) + ":" + to_string((s % 3600) / 60) + ":" + to_string(s % 60) + "." + to_string((u_int)tvp.tv_usec);

  //myfile << tmp_str << ',';
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());

  tmp_str = to_string((int)sip[0]) + "." + to_string((int)sip[1]) + "." + to_string((int)sip[2]) + "." + to_string((int)sip[3]) + ":" + to_string(ft->get_src_port());
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());

  tmp_str = to_string((int)dip[0]) + "." + to_string((int)dip[1]) + "." + to_string((int)dip[2]) + "." + to_string((int)dip[3]) + ":" + to_string(ft->get_dst_port());
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << (int)sip[0] << "." << (int)sip[1] << "." << (int)sip[2] << "." << (int)sip[3] << ':';
  //myfile << ft->get_src_port() << ',';
  //myfile << (int)dip[0] << "." << (int)dip[1] << "." << (int)dip[2] << "." << (int)dip[3] << ':';
  //myfile << ft->get_dst_port() << ',';

  //myfile << std::setprecision(10) << (cf->get_conversation()->get_duration_ms() / 1000) << ",";//<< "duration       : "

  TextLog_Print(csv_log, "%.10f", (cf->get_conversation()->get_duration_ms() / 1000));
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << cf->get_conversation()->get_protocol_type_str() << ",";//<< "protocol_type  : "
  tmp_str = cf->get_conversation()->get_protocol_type_str();
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << cf->get_conversation()->get_service_str() << ",";//<< "service        : "
  tmp_str = cf->get_conversation()->get_service_str();
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << cf->get_conversation()->get_state_str() << ",";//<< "flag           : "
  tmp_str = cf->get_conversation()->get_state_str();
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << std::setprecision(10) <<  cf->get_conversation()->get_src_bytes() << ",";//<< "src_bytes      : "
  TextLog_Print(csv_log, "%.10f", cf->get_conversation()->get_src_bytes());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << std::setprecision(10) <<  cf->get_conversation()->get_dst_bytes() << ",";//<< "dst_bytes      : "
  TextLog_Print(csv_log, "%.10f", cf->get_conversation()->get_dst_bytes());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << cf->get_conversation()->land() << ",";//<< "land           : "
  tmp_str = cf->get_conversation()->land();
  TextLog_Puts(csv_log, tmp_str.c_str());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) <<  cf->get_conversation()->get_wrong_fragments() << ",";//<< "wrong_fragment : "
  TextLog_Print(csv_log, "%.10f", cf->get_conversation()->get_wrong_fragments());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) <<  cf->get_conversation()->get_urgent_packets() << ",";//<< "urgent         : "
  TextLog_Print(csv_log, "%.10f", cf->get_conversation()->get_urgent_packets());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_count() << ",";//<< "count             : "
  TextLog_Print(csv_log, "%.10f", cf->get_count());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_srv_count() << ",";//<< "srv_count         : "
  TextLog_Print(csv_log, "%.10f", cf->get_srv_count());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_serror_rate() << ","; //<< "serror_rate       : "
  TextLog_Print(csv_log, "%.10f", cf->get_serror_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_srv_serror_rate() << ","; //<< "srv_serror_rate   : "
  TextLog_Print(csv_log, "%.10f", cf->get_srv_serror_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_rerror_rate() << ","; //<< "rerror_rate       : "
  TextLog_Print(csv_log, "%.10f", cf->get_rerror_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_srv_rerror_rate() << ","; //<< "srv_rerror_rate   : "
  TextLog_Print(csv_log, "%.10f", cf->get_srv_rerror_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_same_srv_rate() << ","; //<< "same_srv_rate     : "
  TextLog_Print(csv_log, "%.10f", cf->get_same_srv_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_diff_srv_rate() << ","; //<< "diff_srv_rate     : "
  TextLog_Print(csv_log, "%.10f", cf->get_diff_srv_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_srv_diff_host_rate() << ","; //<< "srv_diff_host_rate : "
  TextLog_Print(csv_log, "%.10f", cf->get_srv_diff_host_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_count() << ","; //<< "dst_host_count     : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_count());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_srv_count() << ","; //<< "dst_host_srv_count : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_srv_count());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_same_srv_rate() << ","; //<< "dst_host_same_srv_rate : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_same_srv_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_diff_srv_rate() << ","; //<< "dst_host_diff_srv_rate : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_diff_srv_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_same_src_port_rate() << ","; //<< "dst_host_same_src_port_rate : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_same_src_port_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_srv_diff_host_rate() << ","; //<< "dst_host_srv_diff_host_rate : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_srv_diff_host_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_serror_rate() << ",";//<< "dst_host_serror_rate        : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_serror_rate());
  TextLog_Puts(csv_log, sep.c_str());


  //myfile << std::setprecision(10) << cf->get_dst_host_srv_serror_rate() << ","; //<< "dst_host_srv_serror_rate    : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_srv_serror_rate());
  TextLog_Puts(csv_log, sep.c_str());


// myfile << std::setprecision(10) << cf->get_dst_host_rerror_rate() << ",";//<< "dst_host_rerror_rate        : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_rerror_rate());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << std::setprecision(10) << cf->get_dst_host_srv_rerror_rate() << ","; //<< "dst_host_srv_rerror_rate    : "
  TextLog_Print(csv_log, "%.10f", cf->get_dst_host_srv_rerror_rate());
  TextLog_Puts(csv_log, sep.c_str());

  //myfile << label << "\n";
  tmp_str = label;
  TextLog_Puts(csv_log, tmp_str.c_str());
  //TextLog_Puts(csv_log, sep.c_str());
  //myfile.close();

  TextLog_NewLine(csv_log);
  TextLog_Flush(csv_log);
}

void Dpx::eval(Packet* p)
{
  vector<string> tmp;
  string tmp_str;
  if (p->is_ip4() && (p->is_tcp() || p->is_udp() || p->is_icmp())) {
    FeatureExtractor::IpFragment *frag = getIpFragment(p);
    FeatureExtractor::Packet *datagr = nullptr;
    //TPkey new_tpkey(frag);
    FeatureExtractor::Timestamp now = frag->get_end_ts();
    datagr = reasm->reassemble(frag);
    if (datagr) {
      conv_reconstructor->add_packet(datagr);
    } else {
      conv_reconstructor->report_time(now);
    }
    ++dpxstats.total_packets;
  }

  FeatureExtractor::Conversation *conv;
  while ((conv = conv_reconstructor->get_next_conversation()) != nullptr) {
    FeatureExtractor::ConversationFeatures *cf = stats_engine->calculate_features(conv);
    conv = nullptr;        // Should not be used anymore, object will commit suicide

    tmp = get_attr(cf);
    tmp_str = dec_tree.guess(df_save, tmp);
    if (tmp_str != "normal.") {
      for (int i = 0; i < tmp.size(); i++)
      {
        cout << tmp[i];
        if (i < (tmp.size() - 1))
        {
          cout << "  " ;
        }
      }

      cout << tmp_str << endl;
    }
    save_csv(cf, tmp_str);
    tmp.clear();
    tmp.shrink_to_fit();
    delete cf;
  }

}

FeatureExtractor::IpFragment *Dpx::getIpFragment(Packet *packet) {
  FeatureExtractor::IpFragment *f = new FeatureExtractor::IpFragment();
  FeatureExtractor::Timestamp ts(packet->pkth->ts);
  f->set_start_ts(ts);
  f->set_length(packet->pkth->pktlen);

  if (!packet->is_eth()) {
    return f;
  }
  f->set_eth2(true);
  if (!packet->is_ip4()) {
    return f;
  }

  f->set_src_ip(packet->ptrs.ip_api.get_src()->get_ip4_value());
  f->set_dst_ip(packet->ptrs.ip_api.get_dst()->get_ip4_value());
  f->set_ip_proto((FeatureExtractor::ip_field_protocol_t) packet->ptrs.ip_api.proto());
  f->set_ip_id((uint16_t) (packet->ptrs.ip_api.id()));
  f->set_ip_flag_mf(packet->is_fragment());
  f->set_ip_frag_offset(packet->ptrs.ip_api.off());
  f->set_ip_payload_length(packet->ptrs.ip_api.pay_len());

  if (f->get_ip_frag_offset() > 0)
    return f;

  switch (f->get_ip_proto()) {
  case FeatureExtractor::TCP:
    f->set_src_port(packet->ptrs.tcph->src_port());
    f->set_dst_port(packet->ptrs.tcph->dst_port());
    f->set_tcp_flags(packet->ptrs.tcph->th_flags);
    break;
  case FeatureExtractor::UDP:
    f->set_src_port(packet->ptrs.udph->src_port());
    f->set_dst_port(packet->ptrs.udph->dst_port());
    break;

  case FeatureExtractor::ICMP:
    f->set_icmp_type((FeatureExtractor::icmp_field_type_t) packet->ptrs.icmph->type);
    f->set_icmp_code(packet->ptrs.icmph->code);
    break;

  default:
    break;
  }
  return f;
}

void Dpx::tinit()
{
  size_t limit = 0;
  string file = file_log;//"alert_dpx.csv";
  csv_log = TextLog_Init(file.c_str(), LOG_BUFFER, limit);
}

void Dpx::tterm()
{
  if ( csv_log )
    TextLog_Term(csv_log);
}


//-------------------------------------------------------------------------
// module stuff
//-------------------------------------------------------------------------

static const Parameter dpx_params[] =
{
  {"file_model",  Parameter::PT_STRING, nullptr, nullptr,
                        "file model data mining"},
  {"file_log",  Parameter::PT_STRING, nullptr, nullptr,
                        "file log"},                      

  /*{ "port", Parameter::PT_PORT, nullptr, nullptr,
    "port to check"
  },

  { "max", Parameter::PT_INT, "0:65535", "0",
    "maximum payload before alert"
  },*/

  { nullptr, Parameter::PT_MAX, nullptr, nullptr, nullptr }
};

static const RuleMap dpx_rules[] =
{
  { DPX_SID, "too much data sent to port" },
  { 0, nullptr }
};

class DpxModule : public Module
{
public:
  DpxModule() : Module(s_name, s_help, dpx_params)
  { }

  unsigned get_gid() const override
  { return DPX_GID; }

  const RuleMap* get_rules() const override
  { return dpx_rules; }

  const PegInfo* get_pegs() const override
  { return simple_pegs; }

  PegCount* get_counts() const override
  { return (PegCount*)&dpxstats; }

  ProfileStats* get_profile() const override
  { return &dpxPerfStats; }

  bool set(const char*, Value& v, SnortConfig*) override;

  Usage get_usage() const override
  { return INSPECT; }

public:
  /*uint16_t port;
  uint16_t max;*/
  string file_model;
  string file_log;
};

bool DpxModule::set(const char*, Value& v, SnortConfig*)
{
  /*if ( v.is("port") )
    port = v.get_long();

  else if ( v.is("max") )
    max = v.get_long();*/
  if ( v.is("file_model") )
    file_model = v.get_string();

  else if ( v.is("file_log") )
    file_log = v.get_string();

  else
    return false;

  return true;
}

//-------------------------------------------------------------------------
// api stuff
//-------------------------------------------------------------------------

static Module* mod_ctor()
{ return new DpxModule; }

static void mod_dtor(Module* m)
{ delete m; }

static Inspector* dpx_ctor(Module* m)
{
  DpxModule* mod = (DpxModule*)m;
  return new Dpx(mod->file_model, mod->file_log);
}

static void dpx_dtor(Inspector* p)
{
  delete p;
}

static const InspectApi dpx_api
{
  {
    PT_INSPECTOR,
    sizeof(InspectApi),
    INSAPI_VERSION,
    0,
    API_RESERVED,
    API_OPTIONS,
    s_name,
    s_help,
    mod_ctor,
    mod_dtor
  },
  IT_PACKET,
  PROTO_BIT__ANY_IP,
  nullptr, // buffers
  nullptr, // service
  nullptr, // pinit
  nullptr, // pterm
  nullptr, // tinit
  nullptr, // tterm
  dpx_ctor,
  dpx_dtor,
  nullptr, // ssn
  nullptr  // reset
};

SO_PUBLIC const BaseApi* snort_plugins[] =
{
  &dpx_api.base,
  nullptr
};

