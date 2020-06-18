#include "bank_protocol.h"
#include "types.h" 
#include <cstdio>
#include <exception>
#include <time.h>
#include <chrono>

using namespace std;
using namespace bank_info_type;
using namespace chrono;

namespace bank_network_methods {

	transaction::t_type get_header_info(uint8_t* buffer, uint32_t len) {
		//write code to get info about header infomation
		return transaction::t_type::send;
	}

	uint8_t* parse_body(uint8_t* buffer, uint32_t len) {
		//return packet body start pointer;
		return nullptr;
	}

	///<summary>object to parse bytes -> cspinfo/tspfino</summary>
	enum class jcode { empty };
	transaction* parse_data_buf_to_transaction(uint8_t* buffer, uint32_t len) {		
		uint16_t* version = (uint16_t*)buffer;
		if (*version != CURRENT_VERSION) return nullptr;
		if (buffer[2] == TRANSACTION_TYPE_SEND) {
			pinfo* spinfo_ = (pinfo*)(buffer+3);
			pinfo* rpinfo_ = (pinfo*)(buffer+45);
			uint32_t* amount = (uint32_t*)(buffer+87);
			time_point<system_clock>* req_t = (time_point<system_clock>*)(buffer+119);
			boost::asio::detail::addrinfo_type * req_loc = (boost::asio::detail::addrinfo_type*)(buffer+127);
			return new transaction(transaction::t_type::send,
				/*cspinfo(spinfo_->pid, string((char*)(spinfo_->ppass))),
				cspinfo(rpinfo_->pid, string((char*)(rpinfo_->ppass))),*/
				*spinfo_,
				*rpinfo_,
				*amount,
				*req_loc,
				*req_t
			);
		}
		if (buffer[2] == TRANSACTION_TYPE_RECIEVE) {
			pinfo* spinfo_ = (pinfo*)(buffer+3);
			pinfo* rpinfo_ = (pinfo*)(buffer+45);
			unsigned long long* amount = (unsigned long long*)(buffer+87);
			time_point<system_clock>* req_t = (time_point<system_clock>*)(buffer+119);
			boost::asio::detail::addrinfo_type* req_loc = (boost::asio::detail::addrinfo_type*)(buffer+127);
			return new transaction(transaction::t_type::receive,
				/*cspinfo(spinfo_->pid, string((char*)(spinfo_->ppass))),
				cspinfo(rpinfo_->pid, string((char*)(rpinfo_->ppass))),*/
				*spinfo_,
				*rpinfo_,
				*amount,
				*req_loc,
				*req_t
			);
		}
		if (buffer[2] == TRANSACTION_TYPE_DEPOSIT) {
			pinfo* rpinfo = (pinfo*)(buffer+3);
			uint32_t* amount = (uint32_t*)(buffer+45);
			time_point<system_clock>* req_t = (time_point<system_clock>*)(buffer+77);
			boost::asio::detail::addrinfo_type* req_loc = (boost::asio::detail::addrinfo_type*)(buffer+85);
			return new transaction(transaction::t_type::deposit,
				/*cspinfo(rpinfo->pid, string((char*)rpinfo->ppass)),*/				
				*rpinfo,
				p_null,
				*amount,
				*req_loc,
				*req_t
			);
		}
		if (buffer[2] == TRANSACTION_TYPE_WITHDRAW) {
			pinfo* rpinfo = (pinfo*)(buffer+3);
			uint32_t* amount = (uint32_t*)(buffer+45);
			time_point<system_clock>* req_t = (time_point<system_clock>*)(buffer+77);
			boost::asio::detail::addrinfo_type* req_loc = (boost::asio::detail::addrinfo_type*)(buffer+85);
			return new transaction(transaction::t_type::withdraw,
				/*cspinfo(rpinfo->pid, string((char*)rpinfo->ppass)),*/				
				*rpinfo,
				p_null,
				*amount,
				*req_loc,
				*req_t
			);
		}
	}

	shared_ptr<transaction> generate_transaction(uint8_t* buffer, uint32_t len) {
		csp_packet* cp_ = (csp_packet*)buffer;
		if (cp_->header.protocol_version == CURRENT_VERSION) throw new exception("Version Not Matched");		
		transaction::t_type res_type = (transaction::t_type)cp_->header.inquiry_type;
		if (res_type == transaction::t_type::none) throw new exception("Request Header is not valid!!");		
		switch (res_type) {
			case transaction::t_type::send :
				break;
			case transaction::t_type::receive :
				break;
			case transaction::t_type::deposit :
				break;
			case transaction::t_type::withdraw :
				break;
			default :
				throw new exception("Undefined Job Code");
		}

		pinfo sender = cp_->body.sender;
		pinfo receiver = cp_->body.receiver;
		uint64_t amount = cp_->body.amount;
		chrono::time_point<system_clock> req_timestamp = cp_->body.req_time;
		uint32_t req_loc = cp_->body.req_loc;
		//boost::asio::ip::tcp::endpoint::data_type req_ep(boost::asio::ip::address_v4(req_loc));
		addrinfo req_epinfo = { 0, };
		//parse cp_->body.req_loc to addrinfo

		return transaction::make_transaction(res_type, sender, receiver, amount, req_epinfo, req_timestamp);
	}			

	///<summary>find SGMK mark on recieved packed</summary>
	uint8_t* find_signature(uint8_t** buffer, uint32_t len) {
		if (*(uint32_t*)(*buffer) == SGMK_BE) {
			*buffer += 4;
			return *buffer;
		}
		else {
			return nullptr;
		}
	}

	jcode packet_classify(uint8_t buffer, uint32_t len) {		
		return jcode::empty;
	}

	///<summary>decode packet with signature</summary>
	uint8_t* decode_packet(uint8_t* buffer, uint32_t len) {
		uint8_t* res = (uint8_t*)malloc(len);
		unsigned short offset;
		for (offset = 0; offset < len; offset += 4) {
			res[offset] = *(uint32_t*)(buffer + offset) ^ SGMK_BE;
		}
		return res;
	}
}


