//--------------------------------------------------------------------------
// Copyright (C) 2014-2020 Cisco and/or its affiliates. All rights reserved.
// Copyright (C) 2013-2013 Sourcefire, Inc.
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

// alert_ex.cc author Russ Combs <rucombs@cisco.com>



#include <iostream>
#include <fstream>
#include <iomanip>

#include "detection/signature.h"
#include "events/event.h"
#include "main/snort_config.h"
#include "framework/logger.h"
#include "framework/module.h"

#include "protocols/packet.h"
#include "protocols/tcp.h"
#include "protocols/udp.h"
#include "protocols/icmp4.h"

#include "extractor/IpFragment.h"
#include "extractor/StatsEngine.h"
#include "extractor/ConversationReconstructor.h"
#include "extractor/Config.h"
#include "extractor/IpReassembler.h"
#include "extractor/FiveTuple.h"

#include "utils/util.h"

using namespace snort;
using namespace std;



static const char* s_name = "alert_ex";
static const char* s_help = "output gid:sid:rev for alerts";

//-------------------------------------------------------------------------
// module stuff
//-------------------------------------------------------------------------

static const Parameter s_params[] =
{
    { "upper", Parameter::PT_BOOL, nullptr, "false",
      "true/false -> convert to upper/lower case" },

    { nullptr, Parameter::PT_MAX, nullptr, nullptr, nullptr }
};

class ExModule : public Module
{
public:
    ExModule() : Module(s_name, s_help, s_params) { }

    bool set(const char*, Value&, SnortConfig*) override;
    bool begin(const char*, int, SnortConfig*) override;

    Usage get_usage() const override
    { return CONTEXT; }

public:
    bool upper;
};

bool ExModule::set(const char*, Value& v, SnortConfig*)
{
    if ( v.is("upper") )
        upper = v.get_bool();

    else
        return false;

    return true;
}

bool ExModule::begin(const char*, int, SnortConfig*)
{
    upper = true;
    return true;
}

//-------------------------------------------------------------------------
// logger stuff
//-------------------------------------------------------------------------

class ExLogger : public Logger
{
public:
    ExLogger(ExModule* m);

    void alert(Packet* p, const char* msg, const Event&) override;

private:
    bool upper;
    FeatureExtractor::Config *config;
    FeatureExtractor::IpReassembler *reasm;
    FeatureExtractor::ConversationReconstructor *conv_reconstructor;
    FeatureExtractor::StatsEngine *stats_engine;

    FeatureExtractor::IpFragment *getIpFragment(Packet *);
    
    void save_csv(FeatureExtractor::ConversationFeatures *cf);
};

ExLogger::ExLogger(ExModule* m)
{
   upper = m->upper;
   config = new FeatureExtractor::Config();
   stats_engine = new FeatureExtractor::StatsEngine(config);
   reasm = new FeatureExtractor::IpReassembler();
   conv_reconstructor = new FeatureExtractor::ConversationReconstructor();
}

FeatureExtractor::IpFragment *ExLogger::getIpFragment(Packet *packet) {
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

void ExLogger::save_csv(FeatureExtractor::ConversationFeatures *cf) {

   ofstream myfile;
   myfile.open ("/home/cecep/cpp/mit_darpa_99/data/snort_alert_csv/alert_ex_data.csv", fstream::in | fstream::out | fstream::app);

   const FeatureExtractor::FiveTuple *ft = cf->get_conversation()->get_five_tuple_ptr();

   uint32_t src_ip = ft->get_src_ip();
   uint32_t dst_ip = ft->get_dst_ip();
   uint8_t *sip = (uint8_t *)&src_ip;
   uint8_t *dip = (uint8_t *)&dst_ip;

   int localzone = SnortConfig::get_conf()->thiszone;   
   FeatureExtractor::Timestamp ts = cf->get_conversation()->get_start_ts();
   struct timeval tvp = ts.get_timeval();
   
   //if (SnortConfig::output_use_utc())
   //    localzone = 0;

   int s = (tvp.tv_sec + localzone) % SECONDS_PER_DAY;
   time_t Time = (tvp.tv_sec + localzone) - s;

   struct tm ttm;
   struct tm* lt = gmtime_r(&Time, &ttm); 
   
   myfile << (lt->tm_mon+1) << "/" << lt->tm_mday << "-" << (s/3600) << ":" << ((s % 3600)/60) << ":" << (s % 60) << "." << ((u_int)tvp.tv_usec) << ','; 
   myfile << (int)sip[0] << "." << (int)sip[1] << "." << (int)sip[2] << "." << (int)sip[3] << ':';
   myfile << ft->get_src_port() << ',';
   myfile << (int)dip[0] << "." << (int)dip[1] << "." << (int)dip[2] << "." << (int)dip[3] << ':';
   myfile << ft->get_dst_port() << ',';
   
   myfile << std::setprecision(10) << (cf->get_conversation()->get_duration_ms() / 1000) << ",";//<< "duration       : "
   myfile << cf->get_conversation()->get_protocol_type_str() << ",";//<< "protocol_type  : "
   myfile << cf->get_conversation()->get_service_str() << ",";//<< "service        : "
   myfile << cf->get_conversation()->get_state_str() << ",";//<< "flag           : "
   myfile << std::setprecision(10) <<  cf->get_conversation()->get_src_bytes() << ",";//<< "src_bytes      : "
   myfile << std::setprecision(10) <<  cf->get_conversation()->get_dst_bytes() << ",";//<< "dst_bytes      : "
   myfile << cf->get_conversation()->land() << ",";//<< "land           : "
   myfile << std::setprecision(10) <<  cf->get_conversation()->get_wrong_fragments() << ",";//<< "wrong_fragment : "
   myfile << std::setprecision(10) <<  cf->get_conversation()->get_urgent_packets() << ",";//<< "urgent         : "
   //myfile << std::setprecision(3) <<  0 << ",";//<< "hot            : "
   //myfile << std::setprecision(3) <<  0 << ",";//<< "num_failed_logins : "
   //myfile << 0 << ",";//<< "logged_in      : "
   //myfile << 0 << ",";//<< "num_compromised: "
   //myfile << 0 << ",";//<< "root_shell     : "
   //myfile << 0 << ",";//<< "su_attempted   : "
   //myfile << 0 << ",";//<< "num_root       : "
   //myfile << 0 << ",";//<< "num_file_creations : "
   //myfile << 0 << ",";//<< "num_shells     : "
   //myfile << 0 << ",";//<< "num_access_files : "
   //myfile << 0 << ",";//<< "num_outbound_cmds : "
   //myfile << 0 << ",";//<< "is_host_login     : "
   //myfile << 0 << ",";//<< "is_guest_login    : "
   myfile << std::setprecision(10) << cf->get_count() << ",";//<< "count             : "
   myfile << std::setprecision(10) << cf->get_srv_count() << ",";//<< "srv_count         : "
   myfile << std::setprecision(10) << cf->get_serror_rate() << ","; //<< "serror_rate       : "
   myfile << std::setprecision(10) << cf->get_srv_serror_rate() << ","; //<< "srv_serror_rate   : "
   myfile << std::setprecision(10) << cf->get_rerror_rate() << ","; //<< "rerror_rate       : "
   myfile << std::setprecision(10) << cf->get_srv_rerror_rate() << ","; //<< "srv_rerror_rate   : "
   myfile << std::setprecision(10) << cf->get_same_srv_rate() << ","; //<< "same_srv_rate     : "
   myfile << std::setprecision(10) << cf->get_diff_srv_rate() << ","; //<< "diff_srv_rate     : "
   myfile << std::setprecision(10) << cf->get_srv_diff_host_rate() << ","; //<< "srv_diff_host_rate : "
   myfile << std::setprecision(10) << cf->get_dst_host_count() << ","; //<< "dst_host_count     : "
   myfile << std::setprecision(10) << cf->get_dst_host_srv_count() << ","; //<< "dst_host_srv_count : "
   myfile << std::setprecision(10) << cf->get_dst_host_same_srv_rate() << ","; //<< "dst_host_same_srv_rate : "
   myfile << std::setprecision(10) << cf->get_dst_host_diff_srv_rate() << ","; //<< "dst_host_diff_srv_rate : "
   myfile << std::setprecision(10) << cf->get_dst_host_same_src_port_rate() << ","; //<< "dst_host_same_src_port_rate : "
   myfile << std::setprecision(10) << cf->get_dst_host_srv_diff_host_rate() << ","; //<< "dst_host_srv_diff_host_rate : "
   myfile << std::setprecision(10) << cf->get_dst_host_serror_rate() << ",";//<< "dst_host_serror_rate        : "
   myfile << std::setprecision(10) << cf->get_dst_host_srv_serror_rate() << ","; //<< "dst_host_srv_serror_rate    : "
   myfile << std::setprecision(10) << cf->get_dst_host_rerror_rate() << ",";//<< "dst_host_rerror_rate        : "
   myfile << std::setprecision(10) << cf->get_dst_host_srv_rerror_rate() << "\n"; //<< "dst_host_srv_rerror_rate    : "
   myfile.close();
}

void ExLogger::alert(Packet* p, const char* msg, const Event& e)
{
    
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
        
	save_csv(cf);
        
        delete cf;
    }	
	
	
    //char timestamp[TIMEBUF_SIZE];
    //ts_print((const struct timeval*)&p->pkth->ts,timestamp);
    
	
    //string s = msg;

    //if ( upper )
    //    transform(s.begin(), s.end(), s.begin(), ::toupper);
    //else
    //    transform(s.begin(), s.end(), s.begin(), ::tolower);

    //cout << e.sig_info->gid << ":";
    //cout << e.sig_info->sid << ":";
    //cout << e.sig_info->rev << " ";
    //cout << s << endl;
}

//-------------------------------------------------------------------------
// api stuff
//-------------------------------------------------------------------------

static Module* mod_ctor()
{ return new ExModule; }

static void mod_dtor(Module* m)
{ delete m; }

static Logger* ex_ctor(Module* mod)
{
    return new ExLogger((ExModule*)mod);
}

static void ex_dtor(Logger* p)
{ delete p; }

static const LogApi ex_api =
{
    {
        PT_LOGGER,
        sizeof(LogApi),
        LOGAPI_VERSION,
        0,
        API_RESERVED,
        API_OPTIONS,
        s_name,
        s_help,
        mod_ctor,
        mod_dtor
    },
    OUTPUT_TYPE_FLAG__ALERT,
    ex_ctor,
    ex_dtor
};

SO_PUBLIC const BaseApi* snort_plugins[] =
{
    &ex_api.base,
    nullptr
};

