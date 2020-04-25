#include "session_pool.h"

///<summary>
session_pool::session_pool():
	pool_id_(rand() % UINT16_MAX),
	session_list(list<session*>())
{

}

///<summary>add new sesion to session_list</summary>
void session_pool::add_session(session&& s_) {
	session_list_value.push_back(s_);
	s_.current_pool = this;
	session_list.push_back(&(session_list_value.back()));
	///this code is for context safeness::if we create session in client handling function context, the thread will be detached and the variable will be distroyed.
	///Therefore, we should create vector for storing the session value, and point the vector's tail. 
}

///<summary>get count of session_list</summary>
int session_pool::get_session_count() const {
	return session_list.size();
}

///<summary>session_pool starts dispose of session_lists.</summary>
void session_pool::session_pool_start(future<void>& stop_ev_) {	
	thread th([&stop_ev_, this]() {
		while (stop_ev_.wait_for(std::chrono::microseconds(100)) == std::future_status::timeout) {
			mtx.lock();
			size_t sl_sz = session_list.size();
			mtx.unlock();
			if (sl_sz > 0) {
				mtx.lock();
				session session_ptr_ = *(session_list.front());
				session_list.pop_front();
				auto cli_ep = session_ptr_.cli_socket;
				//unsigned short port_num = session_ptr_->session_seed % 31337;					
				mtx.unlock();
				thread client_thread([=](session s_ptr_) {
					auto com_session = session::make_session(std::move(s_ptr_));
					tcp::socket client_handle = com_session->accept_client(cli_ep);									
				}, session_ptr_);
				client_thread.detach();//session_ptr_ deleted... context range out
			}
		}
		cout << "session pool loop end" << endl;
	});
	th.detach();
}
