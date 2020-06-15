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
			boost::asio::ip::tcp::endpoint * req_loc = (boost::asio::ip::tcp::endpoint*)(buffer+127);
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
			boost::asio::ip::tcp::endpoint* req_loc = (boost::asio::ip::tcp::endpoint*)(buffer+127);
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
			boost::asio::ip::tcp::endpoint* req_loc = (boost::asio::ip::tcp::endpoint*)(buffer+85);
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
			boost::asio::ip::tcp::endpoint* req_loc = (boost::asio::ip::tcp::endpoint*)(buffer+85);
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
		transaction::t_type res_type = get_header_info(buffer, len);
		if (res_type == transaction::t_type::none) throw new exception("Request Header is not valid!!");
		uint8_t * bdp = parse_body(buffer, len);
		if(bdp == nullptr) throw new exception("Body Signature is not valid!!"); //삭제 예정 (본문에는 시그니처같은거 필요 없음
		switch (res_type) {
			case transaction::t_type::send :
				//pinfo* s =
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

		//return transaction::make_transaction();
	}	
	
	transaction::t_type get_header_info(uint8_t* buffer, uint32_t len) {
		//write code to get info about header infomation
		return transaction::t_type::send;
	}

	uint8_t* parse_body(uint8_t* buffer, uint32_t len) {
		//return packet body start pointer;
		return nullptr;
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


