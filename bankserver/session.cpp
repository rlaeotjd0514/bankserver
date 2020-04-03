#include "session.h"
#include <iostream>

using namespace std;

session::session(uint32_t s_number_, cspinfo* csp_, uint32_t secure_seed_, uint32_t expire_time_) :
	session_expr(true),
	session_number(s_number_),
	session_seed(rand() % ULLONG_MAX),
	secure_seed(secure_seed_),
	current_customer(*csp_),
	expire_time(expire_time_),
	current_pool(nullptr),
	session_timer(HI_timer::deadline, 5, (long long)0,
		[&]() {
			this->change_validation();
			cout << "session have expired" << endl;
		}, []() {})
{		
	cout << "ctor done" << endl;
}

void session::start_session_clock() {
	this->session_timer.start();
}

session::session(const session& cp):
	session_number(cp.session_number),
	expire_time(cp.expire_time),
	secure_seed(cp.secure_seed),
	current_customer(cp.current_customer),
	session_seed(rand() % ULLONG_MAX),
	session_expr(cp.session_expr),
	current_pool(cp.current_pool),
	session_timer(cp.session_timer)
{

}

void session::change_validation() noexcept {
	mtx.lock();
	this->session_expr = !(this->session_expr);
	mtx.unlock();
}

bool session::check_session_validation() const {
	return session_expr;
}




