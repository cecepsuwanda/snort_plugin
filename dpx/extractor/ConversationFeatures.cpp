#include <sstream>
#include <iostream>
#include <iomanip>
#include "ConversationFeatures.h"

namespace FeatureExtractor {
using namespace std;

ConversationFeatures::ConversationFeatures(Conversation *conv)
	: conv(conv)
{
	conv->register_reference();
}


ConversationFeatures::~ConversationFeatures()
{
	// Conversation object commits suicide when nobody needs it anymore
	conv->deregister_reference();
}


Conversation *ConversationFeatures::get_conversation()
{
	return conv;
}

/**
 * Getters, setters, inc & dec for derived feature values
 */
uint32_t ConversationFeatures::get_count() const {
	return count;
}
void ConversationFeatures::set_count(uint32_t count) {
	this->count = count;
}

uint32_t ConversationFeatures::get_srv_count() const {
	return srv_count;
}
void ConversationFeatures::set_srv_count(uint32_t srv_count) {
	this->srv_count = srv_count;
}

double ConversationFeatures::get_serror_rate() const {
	return serror_rate;
}
void ConversationFeatures::set_serror_rate(double serror_rate) {
	this->serror_rate = serror_rate;
}

double ConversationFeatures::get_srv_serror_rate() const {
	return srv_serror_rate;
}
void ConversationFeatures::set_srv_serror_rate(double srv_serror_rate) {
	this->srv_serror_rate = srv_serror_rate;
}

double ConversationFeatures::get_rerror_rate() const {
	return rerror_rate;
}
void ConversationFeatures::set_rerror_rate(double rerror_rate) {
	this->rerror_rate = rerror_rate;
}

double ConversationFeatures::get_srv_rerror_rate() const {
	return srv_rerror_rate;
}
void ConversationFeatures::set_srv_rerror_rate(double srv_rerror_rate) {
	this->srv_rerror_rate = srv_rerror_rate;
}

double ConversationFeatures::get_same_srv_rate() const {
	return same_srv_rate;
}
void ConversationFeatures::set_same_srv_rate(double same_srv_rate) {
	this->same_srv_rate = same_srv_rate;
}

double ConversationFeatures::get_diff_srv_rate() const {
	return diff_srv_rate;
}
void ConversationFeatures::set_diff_srv_rate(double diff_srv_rate) {
	this->diff_srv_rate = diff_srv_rate;
}

double ConversationFeatures::get_srv_diff_host_rate() const {
	return (srv_count == 0) ? 0.0 : ((srv_count - same_srv_count) / (double)srv_count);
}

uint32_t ConversationFeatures::get_same_srv_count() const {
	return same_srv_count;
}
void ConversationFeatures::set_same_srv_count(uint32_t same_srv_count) {
	this->same_srv_count = same_srv_count;
}

uint32_t ConversationFeatures::get_dst_host_count() const {
	return dst_host_count;
}
void ConversationFeatures::set_dst_host_count(uint32_t dst_host_count) {
	this->dst_host_count = dst_host_count;
}

uint32_t ConversationFeatures::get_dst_host_srv_count() const {
	return dst_host_srv_count;
}
void ConversationFeatures::set_dst_host_srv_count(uint32_t dst_host_srv_count)	{
	this->dst_host_srv_count = dst_host_srv_count;
}

double ConversationFeatures::get_dst_host_same_srv_rate() const {
	return dst_host_same_srv_rate;
}
void ConversationFeatures::set_dst_host_same_srv_rate(double dst_host_same_srv_rate) {
	this->dst_host_same_srv_rate = dst_host_same_srv_rate;
}

double ConversationFeatures::get_dst_host_diff_srv_rate() const {
	return dst_host_diff_srv_rate;
}
void ConversationFeatures::set_dst_host_diff_srv_rate(double dst_host_diff_srv_rate) {
	this->dst_host_diff_srv_rate = dst_host_diff_srv_rate;
}

double ConversationFeatures::get_dst_host_same_src_port_rate() const {
	return dst_host_same_src_port_rate;
}
void ConversationFeatures::set_dst_host_same_src_port_rate(double dst_host_same_src_port_rate) {
	this->dst_host_same_src_port_rate = dst_host_same_src_port_rate;
}

double ConversationFeatures::get_dst_host_serror_rate() const {
	return dst_host_serror_rate;
}
void ConversationFeatures::set_dst_host_serror_rate(double dst_host_serror_rate) {
	this->dst_host_serror_rate = dst_host_serror_rate;
}

double ConversationFeatures::get_dst_host_srv_serror_rate() const {
	return dst_host_srv_serror_rate;
}
void ConversationFeatures::set_dst_host_srv_serror_rate(double dst_host_srv_serror_rate) {
	this->dst_host_srv_serror_rate = dst_host_srv_serror_rate;
}

double ConversationFeatures::get_dst_host_rerror_rate() const {
	return dst_host_rerror_rate;
}
void ConversationFeatures::set_dst_host_rerror_rate(double dst_host_rerror_rate) {
	this->dst_host_rerror_rate = dst_host_rerror_rate;
}

double ConversationFeatures::get_dst_host_srv_rerror_rate() const {
	return dst_host_srv_rerror_rate;
}
void ConversationFeatures::set_dst_host_srv_rerror_rate(double dst_host_srv_rerror_rate) {
	this->dst_host_srv_rerror_rate = dst_host_srv_rerror_rate;
}

double ConversationFeatures::get_dst_host_srv_diff_host_rate() const {
	return (dst_host_srv_count == 0) ? 0.0 : ((dst_host_srv_count - dst_host_same_srv_count) / (double)dst_host_srv_count);
}

uint32_t ConversationFeatures::get_dst_host_same_srv_count() const {
	return dst_host_same_srv_count;
}
void ConversationFeatures::set_dst_host_same_srv_count(uint32_t dst_host_same_srv_count) {
	this->dst_host_same_srv_count = dst_host_same_srv_count;
}

// Allow using localtime instead of localtime_s
#pragma warning(disable : 4996)
void ConversationFeatures::print(bool print_extra_features) const
{
	stringstream ss;

	// Intrinsic features
	ss << noshowpoint << setprecision(0) << (conv->get_duration_ms() / 1000) << ','; // Cut fractional part
	ss << conv->get_protocol_type_str() << ',';
	ss << conv->get_service_str() << ',';
	ss << conv->get_state_str() << ',';
	ss << conv->get_src_bytes() << ',';
	ss << conv->get_dst_bytes() << ',';
	ss << conv->land() << ',';
	ss << conv->get_wrong_fragments() << ',';
	ss << conv->get_urgent_packets() << ',';

	// Derived time windows features
	ss << fixed << showpoint << setprecision(2);
	ss << count << ',';
	ss << srv_count << ',';
	ss << serror_rate << ',';
	ss << srv_serror_rate << ',';
	ss << rerror_rate << ',';
	ss << srv_rerror_rate << ',';
	ss << same_srv_rate << ',';
	ss << diff_srv_rate << ',';
	ss << get_srv_diff_host_rate() << ',';

	// Derived connection count window features
	ss << dst_host_count << ',';
	ss << dst_host_srv_count << ',';
	ss << dst_host_same_srv_rate << ',';
	ss << dst_host_diff_srv_rate << ',';
	ss << dst_host_same_src_port_rate << ',';
	ss << get_dst_host_srv_diff_host_rate() << ',';
	ss << dst_host_serror_rate << ',';
	ss << dst_host_srv_serror_rate << ',';
	ss << dst_host_rerror_rate << ',';
	ss << dst_host_srv_rerror_rate;

	if (print_extra_features) {
		const FiveTuple *ft = conv->get_five_tuple_ptr();

		// TODO: ugly wtf, but working
		uint32_t src_ip = ft->get_src_ip();
		uint32_t dst_ip = ft->get_dst_ip();
		uint8_t *sip = (uint8_t *)&src_ip;
		uint8_t *dip = (uint8_t *)&dst_ip;
		ss << ',';
		ss << (int)sip[0] << "." << (int)sip[1] << "." << (int)sip[2] << "." << (int)sip[3] << ',';
		ss << ft->get_src_port() << ',';
		ss << (int)dip[0] << "." << (int)dip[1] << "." << (int)dip[2] << "." << (int)dip[3] << ',';
		ss << ft->get_dst_port() << ',';

		// Time (e.g.: 2010-06-14T00:11:23)
		struct tm *ltime;
		//struct tm timeinfo;
		char timestr[20];
		time_t local_tv_sec;
		local_tv_sec = conv->get_last_ts().get_secs();
		ltime = localtime(&local_tv_sec);
		//localtime_s(&timeinfo, &local_tv_sec);
		strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", ltime);
		//strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", &timeinfo);
		ss << timestr;
	}

	cout << ss.str() << endl;

}

#pragma warning(disable : 4996)
void ConversationFeatures::kddcup_attr(Tmy_dtsvm &my_dtsvm) const
{
	Tkddcup_attr kddcup_attr;

	// Intrinsic features
	kddcup_attr.add_continuous(0, (conv->get_duration_ms() / 1000),1);
	kddcup_attr.add_non_continuous(1, conv->get_protocol_type_str());
	kddcup_attr.add_non_continuous(2, conv->get_service_str());
	kddcup_attr.add_non_continuous(3, conv->get_state_str());
	kddcup_attr.add_continuous(4, conv->get_src_bytes(),1);
	kddcup_attr.add_continuous(5, conv->get_dst_bytes(),1);
	kddcup_attr.add_non_continuous(6, to_string(conv->land() == 0 ? 0 : 1));
	kddcup_attr.add_continuous(7, conv->get_wrong_fragments(),1);
	kddcup_attr.add_continuous(8, conv->get_urgent_packets(),1);

	// Derived time windows features
	kddcup_attr.add_continuous(9, count,2);
	kddcup_attr.add_continuous(10, srv_count,2);
	kddcup_attr.add_continuous(11, serror_rate,2);
	kddcup_attr.add_continuous(12, srv_serror_rate,2);
	kddcup_attr.add_continuous(13, rerror_rate,2);
	kddcup_attr.add_continuous(14, srv_rerror_rate,2);
	kddcup_attr.add_continuous(15, same_srv_rate,2);
	kddcup_attr.add_continuous(16, diff_srv_rate,2);
	kddcup_attr.add_continuous(17, get_srv_diff_host_rate(),2);

	// Derived connection count window features
	kddcup_attr.add_continuous(18, dst_host_count,2);
	kddcup_attr.add_continuous(19, dst_host_srv_count,2);
	kddcup_attr.add_continuous(20, dst_host_same_srv_rate,2);
	kddcup_attr.add_continuous(21, dst_host_diff_srv_rate,2);
	kddcup_attr.add_continuous(22, dst_host_same_src_port_rate,2);
	kddcup_attr.add_continuous(23, get_dst_host_srv_diff_host_rate(),2);
	kddcup_attr.add_continuous(24, dst_host_serror_rate,2);
	kddcup_attr.add_continuous(25, dst_host_srv_serror_rate,2);
	kddcup_attr.add_continuous(26, dst_host_rerror_rate,2);
	kddcup_attr.add_continuous(27, dst_host_srv_rerror_rate,2);

	const FiveTuple *ft = conv->get_five_tuple_ptr();

	// TODO: ugly wtf, but working
	uint32_t src_ip = ft->get_src_ip();
	uint32_t dst_ip = ft->get_dst_ip();
	uint8_t *sip = (uint8_t *)&src_ip;
	uint8_t *dip = (uint8_t *)&dst_ip;

	string ip_src = to_string((int)sip[0]) + "." + to_string((int)sip[1]) + "." + to_string((int)sip[2]) + "." + to_string((int)sip[3]);
	int port_src = ft->get_src_port();
	string ip_dst = to_string((int)dip[0]) + "." + to_string((int)dip[1]) + "." + to_string((int)dip[2]) + "." + to_string((int)dip[3]);
	int port_dst = ft->get_dst_port();


	// Time (e.g.: 2010-06-14T00:11:23)
	time_t local_tv_sec_start;
	local_tv_sec_start = conv->get_start_ts().get_secs();

	time_t local_tv_sec_last;
	local_tv_sec_last = conv->get_last_ts().get_secs();

	kddcup_attr.add_extra(ip_src, port_src, ip_dst, port_dst, local_tv_sec_start, local_tv_sec_last);
    	
	kddcup_attr.set_label(my_dtsvm.guess(kddcup_attr));

	kddcup_attr.save_to_db();
}


void ConversationFeatures::print_human() const
{
	conv->print_human();

	stringstream ss;
	ss << fixed << setprecision(2);
	ss << "count = " << count << endl;
	ss << "srv_count = " << srv_count << endl;
	ss << "serror_rate = " << serror_rate << endl;
	ss << "srv_serror_rate = " << srv_serror_rate << endl;
	ss << "rerror_rate = " << rerror_rate << endl;
	ss << "srv_rerror_rate = " << srv_rerror_rate << endl;
	ss << "same_srv_rate = " << same_srv_rate << endl;
	ss << "diff_srv_rate = " << diff_srv_rate << endl;
	ss << "get_srv_diff_host_rate = " << get_srv_diff_host_rate() << endl;
	ss << "dst_host_count = " << dst_host_count << endl;
	ss << "dst_host_srv_count = " << dst_host_srv_count << endl;
	ss << "dst_host_same_srv_rate = " << dst_host_same_srv_rate << endl;
	ss << "dst_host_diff_srv_rate = " << dst_host_diff_srv_rate << endl;
	ss << "dst_host_same_src_port_rate = " << dst_host_same_src_port_rate << endl;
	ss << "get_dst_host_srv_diff_host_rate = " << get_dst_host_srv_diff_host_rate() << endl;
	ss << "dst_host_serror_rate = " << dst_host_serror_rate << endl;
	ss << "dst_host_srv_serror_rate = " << dst_host_srv_serror_rate << endl;
	ss << "dst_host_rerror_rate = " << dst_host_rerror_rate << endl;
	ss << "dst_host_srv_rerror_rate = " << dst_host_srv_rerror_rate << endl;
	cout << ss.str() << endl;
}

}
