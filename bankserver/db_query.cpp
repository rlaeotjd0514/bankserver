#include "db_query.h"

namespace db_controller {
	bank_query::bank_query() {
		PGconn* pconn = PQconnectdb("dbname=bankserver host=192.168.74.134 user=OKABE_RINTARO password=fhf10Rnfwoa");
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
	}
	bool bank_query::query_single_customer(pinfo csp)
	{
		return true;
	}
}