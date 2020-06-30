#pragma once
#include "pch.h"
#include "types.h"

using namespace std;
using namespace bank_info_type;
using namespace chrono;

namespace bank_network_methods {
	constexpr uint8_t CURRENT_VERSION = 0x01;
	constexpr uint8_t TRANSACTION_TYPE_SEND = 0x01;
	constexpr uint8_t TRANSACTION_TYPE_RECIEVE = 0x02;
	constexpr uint8_t TRANSACTION_TYPE_DEPOSIT = 0x03;
	constexpr uint8_t TRANSACTION_TYPE_WITHDRAW = 0x04;
	constexpr uint32_t SGMK_LE = 0x4B4D4753;
	constexpr uint32_t SGMK_BE = 0x53474D4B;
	constexpr uint32_t P_headersig = 0x4944554A;
	constexpr uint32_t P_headerfoot = 0x53455952;	
	constexpr uint32_t MASK = 0xffffffff;
	constexpr uint32_t pinfo_size = 34;
	constexpr uint32_t csp_header_size = 18;
	constexpr uint32_t csp_body_size = pinfo_size * 2 + 4 + 8 + 4;
	constexpr uint32_t csp_packet_size = csp_header_size + csp_body_size;

	const cspinfo csp_null = cspinfo(0, "");
	const pinfo p_null = pinfo{ { "", }, 0 };	

	#pragma pack(push, 1)
	struct csp_header {//4 + 2 + 2 + 2 + 4 + 4
		uint8_t signature[4];
		uint16_t protocol_version;
		uint16_t inquiry_type;//*
		uint8_t body_length[2];
		uint8_t secret[4];
		uint8_t footer[4];
	};

	#pragma pack(push, 1)
	struct csp_body {
		pinfo sender;//*
		pinfo receiver;//*
		uint64_t amount;//*
		chrono::time_point<system_clock> req_time;//should be replace with explicit type
		uint32_t req_loc;//parse uint32_t to sockaddr 200618
	};
	
	#pragma pack(push, 1)
	struct csp_packet {
		csp_header header;
		csp_body body;
	};

	class csp_packetC {
	public:
		csp_packetC() = delete;
		csp_packetC(csp_header header, csp_body body) : header(header), body(body) {}

		csp_header get_header() {
			return header;
		}

		csp_body get_body() {
			return body;
		}

	private:
		csp_header header;
		csp_body body;
	};

	///<summary>parse raw buffer to request transaction</summary>
	transaction* parse_data_buf_to_transaction(uint8_t* buffer, uint32_t len);	

	std::shared_ptr<transaction> generate_transaction(uint8_t* buffer, uint32_t len);

	///<summary>find SGMK mark on recieved packed</summary>
	uint8_t* find_signature(uint8_t** buffer, uint32_t len);

	///<summary>find SGMK mark on recieved packed</summary>
	uint8_t* decode_packet(uint8_t* buffer, uint32_t len);
	
	///<summary>classify packet inquery</summary>	
	uint8_t* classify_packet(uint8_t* buffer, uint32_t len);

	///<summary>parse session requiring packet</summary>
	uint8_t* parse_require_packet(uint8_t* buffer, uint32_t len);
}


