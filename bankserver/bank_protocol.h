#pragma once
#include "pch.h"
#include "types.h"

using namespace std;
using namespace bank_info_type;
using namespace chrono;

namespace bank_network_methods {	
	constexpr auto CURRENT_VERSION = 0x01;
	constexpr auto TRANSACTION_TYPE_SEND = 0x01;
	constexpr auto TRANSACTION_TYPE_RECIEVE = 0x02;
	constexpr auto TRANSACTION_TYPE_DEPOSIT = 0x03;
	constexpr auto TRANSACTION_TYPE_WITHDRAW = 0x04;
	constexpr uint32_t SGMK_LE = 0x4B4D4753;
	constexpr uint32_t SGMK_BE = 0x53474D4B;
	constexpr uint32_t MASK = 0xffffffff;

	#pragma pack(push, 1)
	struct pinfo {		
		uint8_t ppass[26];
		unsigned long pid;
	};

	const cspinfo csp_null = cspinfo(0, "");

	///<summary>parse raw buffer to request transaction</summary>
	transaction* parse_data_buf_to_transaction(uint8_t* buffer, uint32_t len);

	///<summary>find SGMK mark on recieved packed</summary>
	uint8_t* find_signature(uint8_t** buffer, uint32_t len);

	///<summary>find SGMK mark on recieved packed</summary>
	uint8_t* decode_packet(uint8_t* buffer, uint32_t len);
	
	///<summary>classify packet inquery</summary>	
	uint8_t* classify_packet(uint8_t* buffer, uint32_t len);

	///<summary>parse session requiring packet</summary>
	uint8_t* parse_require_packet(uint8_t* buffer, uint32_t len);
}


