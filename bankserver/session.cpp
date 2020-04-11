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
	session_timer(HI_timer::timer_type::deadline, expire_time_, (long long)0,
		[&]() {
			this->change_validation();
			cout << "session have expired" << endl;			
		}, []() {})
{		
	cout << "ctor done" << endl;
}

session* session::make_session(uint32_t s_number_, cspinfo* csp_, uint32_t secure_seed_, uint32_t expire_time_) {
	return new session(s_number_, csp_, secure_seed_, expire_time_);
}

void session::start_session_clock() {
	this->session_timer.start(&(this->session_timer));
}

void session::reset_session_clock() {
	
}

void session::add_session_time(int add_time_) {
	this->session_timer.add_to_left_time(add_time_);
}

void session::expire_session() {
	this->session_timer.expire_timer();
}

void session::change_validation() noexcept {
	mtx.lock();
	this->session_expr = !(this->session_expr);
	mtx.unlock();
}

bool session::check_session_validation() const {
	return session_expr;
}




