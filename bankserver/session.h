#pragma once
#include "types.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <exception>
#include <thread>
#include <mutex>

using namespace boost::asio;
using namespace std;
using namespace bank_info_type;

mutex mtx;

class session
{
public :		
	session(uint32_t s_number_, cspinfo* csp_, uint32_t secure_seed_, uint32_t expire_time_) :
		session_number(s_number_),
		expire_time(expire_time_),
		secure_seed(secure_seed_),
		current_customer(*csp_),
		session_seed(rand() % ULLONG_MAX),
		session_expr(true)
	{		
		thread th([&](int et) {
			std::this_thread::sleep_for(std::chrono::seconds(et));
			this->change_validation();
			cout << "session have expired" << endl;		
		}, expire_time );		
		th.detach();	
		cout << "ctor done" << endl;
	}				

	session(const session& cp) :
		session_number(cp.session_number),
		expire_time(cp.expire_time),
		secure_seed(cp.secure_seed),
		current_customer(cp.current_customer),
		session_seed(rand() % ULLONG_MAX),
		session_expr(cp.session_expr)
	{

	}

	void change_validation() noexcept {
		this->session_expr = !(this->session_expr);
	}

	bool check_session_validation() const {
		return session_expr;
	}

	~session() {}
private:
	bool session_expr;
	const uint32_t expire_time;
	uint32_t session_number;
	uint32_t session_seed;
	uint32_t secure_seed;
	cspinfo& current_customer;
	io_service timer_service;	
};