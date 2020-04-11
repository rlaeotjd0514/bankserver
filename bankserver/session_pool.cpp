#include "session_pool.h"

session_pool::session_pool():
	pool_id_(rand() % UINT16_MAX),
	session_list(list<session*>())
{

}

void session_pool::add_session(session * s_) {
	session_list.push_back(s_);
	s_->current_pool = this;		
}

int session_pool::get_session_count() const {
	return session_list.size();
}

void session_pool::session_start() {
	for (session * s : session_list) {
		s->start_session_clock();
	}
}
