#include "db_query.h"

namespace db_controller {	
	bank_query::bank_query(const char * connection_string) {
		this->connection_string_ = connection_string;		
		this->pconn = PQconnectdb(connection_string);
		this->validation = PQstatus(pconn);
	}
	bank_result bank_query::test_db_connection()
	{
		PGconn* pconn = PQconnectdb("dbname=CSPDB host=192.168.91.128 user=OKABE_RINTARO password=fhf10Rnfwoa");
		if (PQstatus(pconn) == CONNECTION_BAD) {
			cout << "Connection Failed due to unknown reason";
			//return error code
			exit(0);
		}
		else {
			cout << "connection success" << endl;
		}
		PGresult* res = PQexec(pconn, "select * from csptable");
		cout << PQerrorMessage(pconn) << endl;
		int qcnt = PQntuples(res);
		int ccnt = PQnfields(res);
		for (int i = 0; i < qcnt; i++) {
			for (int j = 0; j < ccnt; j++) {
				cout << PQgetvalue(res, i, j) << " ";
			}
			cout << "\n";
		}
		return bank_result();
	}

	bank_result bank_query::make_db_query(string connection_string)
	{
		bank_query bq = bank_query(connection_string.c_str());
		return bank_result();
	}
	
	bank_result bank_query::query_single_customer(pinfo csp)
	{
		stringstream query_;
		uint8_t * csp_con = new uint8_t[27];
		memcpy(csp_con, csp.ppass, 26);
		csp_con[26] = '\0';
		query_ << "select * from csptable where CID = " << csp.pid << " and CPASS = '" << csp_con << "'";		
		string sres = query_.str();
		PGresult* qres = PQexec(this->pconn, sres.data());
		if (PQstatus(this->pconn) == CONNECTION_BAD) {
			return bank_result{ "DB connection failed", action_status::failed, result_type::db_query };
		}
		if (PQntuples(qres) > 0) {
			return bank_result{ PQgetvalue(qres, 0, 2), action_status::success, result_type::db_query };
		}
		else {
			return bank_result{ "query failed", action_status::failed, result_type::db_query };
		}
	}	

	bank_result bank_query::connection_test()
	{
		return bank_result();
	}
	bank_result bank_query::close()
	{
		return bank_result();
	}
	bank_result bank_query::connect()
	{
		return bank_result();
	}
}