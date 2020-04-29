#pragma once
#include "pch.h"
#include "bank_protocol.h"
#include <libpq-fe.h>

#pragma comment(lib, "libpq.lib")
#pragma comment(lib, "postgres.lib")

using namespace bank_network_methods;
using namespace std;

namespace db_controller {
	///<summary>find csp number on db and returns account object</summary>
	class bank_query {		
	public:
		bank_query();
		bool query_single_customer(pinfo csp);
	};
}