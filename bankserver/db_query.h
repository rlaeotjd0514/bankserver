#pragma once
#include "pch.h"
#include "bank_protocol.h"

using namespace bank_network_methods;

namespace db_controller {
	///<summary>find csp number on db and returns account object</summary>
	class bank_query {
	public:
		bool query_single_customer(pinfo csp) {
			//query wether csp is existing and reliable, useable;
			return true;
		}
	};
}