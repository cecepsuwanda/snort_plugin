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

#include "my_dt/Tdec_tree.h"

#include "extractor/IpFragment.h"
#include "extractor/StatsEngine.h"
#include "extractor/ConversationReconstructor.h"
#include "extractor/Config.h"
#include "extractor/IpReassembler.h"
#include "extractor/FiveTuple.h"


using namespace snort;

#define DPX_GID 256
#define DPX_SID 1

static const char* s_name = "dpx";
static const char* s_help = "dynamic inspector example";

static THREAD_LOCAL ProfileStats dpxPerfStats;

static THREAD_LOCAL SimpleStats dpxstats;

//-------------------------------------------------------------------------
// class stuff
//-------------------------------------------------------------------------

class Dpx : public Inspector
{
public:
  Dpx(uint16_t port, uint16_t max);

  void show(const SnortConfig*) const override;
  void eval(Packet*) override;

private:
  uint16_t port;
  uint16_t max;

  Tdec_tree dec_tree;
  Tdataframe df_save;

  FeatureExtractor::Config *config;
  FeatureExtractor::IpReassembler *reasm;
  FeatureExtractor::ConversationReconstructor *conv_reconstructor;
  FeatureExtractor::StatsEngine *stats_engine;

  vector<string> get_attr(FeatureExtractor::ConversationFeatures *cf) ;

  FeatureExtractor::IpFragment *getIpFragment(Packet *);
};

Dpx::Dpx(uint16_t p, uint16_t m)
{
  port = p;
  max = m;

  df_save.read_data("/home/cecep/cpp/my_dt/data/alert_ex_dc_tree.csv");
  dec_tree.read_tree(df_save);

  config = new FeatureExtractor::Config();
  stats_engine = new FeatureExtractor::StatsEngine(config);
  reasm = new FeatureExtractor::IpReassembler();
  conv_reconstructor = new FeatureExtractor::ConversationReconstructor();
}

void Dpx::show(const SnortConfig*) const
{
  ConfigLogger::log_value("port", port);
  ConfigLogger::log_value("max", max);
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

void Dpx::eval(Packet* p)
{
  vector<string> tmp;
  string tmp_str;
  if (p->is_ip4() && (p->is_tcp() || p->is_udp() || p->is_icmp())) {
    FeatureExtractor::IpFragment *frag = getIpFragment(p);
    FeatureExtractor::Packet *datagr = nullptr;

    FeatureExtractor::Timestamp now = frag->get_end_ts();
    datagr = reasm->reassemble(frag);
    if (datagr) {
      conv_reconstructor->add_packet(datagr);
    } else {
      conv_reconstructor->report_time(now);
    }

  }

  FeatureExtractor::Conversation *conv;
  while ((conv = conv_reconstructor->get_next_conversation()) != nullptr) {
    FeatureExtractor::ConversationFeatures *cf = stats_engine->calculate_features(conv);
    conv = nullptr;        // Should not be used anymore, object will commit suicide

    tmp = get_attr(cf);
    tmp_str = dec_tree.guess(df_save, tmp);
    if (tmp_str != "normal") {
      cout << tmp_str << endl;
    }
    tmp.clear();
    tmp.shrink_to_fit();
    delete cf;
  }

  // precondition - what we registered for
  //assert(p->is_udp());

  //if ( p->ptrs.dp == port && p->dsize > max )
  //DetectionEngine::queue_event(DPX_GID, DPX_SID);

  //++dpxstats.total_packets;
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


//-------------------------------------------------------------------------
// module stuff
//-------------------------------------------------------------------------

static const Parameter dpx_params[] =
{
  { "port", Parameter::PT_PORT, nullptr, nullptr,
    "port to check"
  },

  { "max", Parameter::PT_INT, "0:65535", "0",
    "maximum payload before alert"
  },

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
  uint16_t port;
  uint16_t max;
};

bool DpxModule::set(const char*, Value& v, SnortConfig*)
{
  if ( v.is("port") )
    port = v.get_long();

  else if ( v.is("max") )
    max = v.get_long();

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
  return new Dpx(mod->port, mod->max);
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

