#pragma once
#include <list>
#include "session.h"

using namespace std;

class session;

class session_pool {
public :
	session_pool();
	void add_session(session s_);
	int get_session_count() const;
	~session_pool() {}
	session_pool(const session_pool& rhs) = delete;
	void session_start();
private:
	uint16_t pool_id_;
	list<session> session_list;
};