#pragma once
#include "pch.h"
#include "session.h"

using namespace std;

class session;

class session_pool {
public :
	session_pool();
	void add_session(session&& s_);
	int get_session_count() const;
	~session_pool() {}
	session_pool(const session_pool& rhs) = delete;
	void session_pool_start(future<void> &stop_ev_);
private:	
	mutex mtx;
	uint16_t pool_id_;
	list<session*> session_list;
	list<session> session_list_value;
};