#pragma once
#include "types.h"
#include "timer.h"
#include <thread>
#include <mutex>
#include "session_pool.h"

using namespace std;
using namespace bank_info_type;

class session_pool;

class session
{
public :	
	mutex mtx;
	session(uint32_t s_number_, cspinfo* csp_, uint32_t secure_seed_, uint32_t expire_time_);
	session(const session& cp);
	void change_validation() noexcept;
	bool check_session_validation() const;
	void start_session_clock();
	void reset_session_clock();
	void expire_session();
	void add_session_time(int at);
	~session() {}
private:
	bool session_expr;
	const uint32_t expire_time;
	uint32_t session_number;
	uint32_t session_seed;
	uint32_t secure_seed;
	cspinfo& current_customer;
	session_pool * current_pool;
	HI_timer session_timer;
	friend class session_pool;
};