#pragma once
#include "pch.h"
#include "bank_protocol.h"
#include <occi.h>

using namespace bank_network_methods;
using namespace oracle::occi;
using namespace std;

namespace db_controller {
	///<summary>find csp number on db and returns account object</summary>
	class bank_query {		
	public:
		bank_query() {}
		bank_query(string db_name, string address, unsigned short port, string SID, string user, string pass) {
			
		}
		bool query_single_customer(pinfo csp) {
			//query wether csp is existing and reliable, useable;
			return true;
		}
		
	};
}