#pragma once
#include "pch.h"
#include "bank_protocol.h"
#include <libpq-fe.h>
#include "bank_error.h"

#pragma comment(lib, "libpq.lib")
#pragma comment(lib, "postgres.lib")

using namespace bank_network_methods;
using namespace std;

namespace db_controller {
	///<summary>find csp number on db and returns account object</summary>
	class bank_query {		
	public:
		bank_query() = delete;
		bank_query(const char * connection_string);
		static bank_result test_db_connection();
		static bank_result make_db_query(string connection_string);
		bank_result query_single_customer(pinfo csp);		
		bank_result connection_test();
		bank_result close();
		bank_result connect();
	private:
		PGconn* pconn;
		string connection_string_;
		ConnStatusType validation;
	};
}