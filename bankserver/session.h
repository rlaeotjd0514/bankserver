#pragma once
#include "types.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <exception>
#include <thread>

using namespace boost::asio;
using namespace std;
using namespace bank_info_type;

class session
{
public :		
	session(uint32_t s_number_, cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_) : 
		session_number(s_number_),		
		expire_time(expire_time_),
		secure_seed(secure_seed_),
		current_customer(csp_),		
		session_seed(rand() % ULLONG_MAX)
	{
		steady_timer expire_timer(timer_service, std::chrono::seconds(expire_time));
		expire_timer.async_wait([this, &expire_timer](boost::system::error_code ec) {
			cout << "session no" << session_number << " have expired::due to time limit exceeded" << endl;
		});
		timer_service.dispatch([]() {});
	}

	~session() {}
private:
	const uint32_t expire_time;
	uint32_t session_number;
	uint32_t session_seed;
	uint32_t secure_seed;
	cspinfo current_customer;
	io_service timer_service;	
};