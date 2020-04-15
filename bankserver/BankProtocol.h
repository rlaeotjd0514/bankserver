#pragma once
#include "pch.h"

using namespace std;
using namespace bank_info_type;
using namespace chrono;

namespace bank_network_methods {
	///object to parse bytes -> cspinfo/tspfino
	struct pinfo {
		uint8_t ppass[26];
		unsigned long pid;
	};

	const cspinfo csp_null = cspinfo(0, "");

	transaction* parse_data_buf_to_transaction(uint8_t* buffer, uint32_t len);

	uint8_t* find_signature(uint8_t** buffer, uint32_t len);

	uint8_t* decode_packet(uint8_t* buffer, uint32_t len);

	uint8_t* classify_packet(uint8_t* buffer, uint32_t len);
}


