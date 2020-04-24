 #pragma once
#include "pch.h"
#include "timer.h"
#include "session_pool.h"
#include "types.h"

using namespace std;
using namespace bank_info_type;
using namespace boost::asio::ip;

class session_pool;

class session
{
public :	
	mutex mtx;
	session(uint32_t s_number_, bank_info_type::cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_, tcp::endpoint cli_socket_);
	session(const session& cp);
	void change_validation() noexcept;
	bool check_session_validation() const;
	void start_session_clock();
	void reset_session_clock();
	void expire_session();
	void add_session_time(int at);
	tcp::socket accept_client(tcp::endpoint cli_ep);
	static session* make_session(uint32_t s_number_, bank_info_type::cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_, tcp::endpoint cli_socket_);
	~session();
private:
	bool session_expr;
	uint32_t expire_time;
	uint32_t session_number;
	uint32_t session_seed;
	uint32_t secure_seed;
	bank_info_type::cspinfo current_customer;
	session_pool * current_pool;
	HI_timer session_timer;
	tcp::endpoint cli_socket;
	friend class session_pool;
	friend class HI_timer;
};