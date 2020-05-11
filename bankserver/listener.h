#pragma once
#include "pch.h"
#include "bank_protocol.h"
#include "db_query.h"
#include "session_pool.h"
#include "stringcontrol.h"

using namespace boost;
using namespace db_controller;
using boost::asio::ip::tcp;

class listener;

class acceptor_ {
public:
	acceptor_(asio::io_context& ios, string ip_addr, unsigned short port_number, vector<session_pool*> sp_);
	void accept_();
private:
	asio::io_context& m_ios;
	tcp::acceptor m_acceptor;
	vector<session_pool*> sp_list;
	friend class listener;
};

///<summary><br>Service class is incharge of communicating with client.</br></summary>
class service {
public :
	service() {}
	void handler_function(std::shared_ptr<asio::ip::tcp::socket> sock, session_pool* sp_);
private:	
	uint8_t* handle_client(std::shared_ptr<asio::ip::tcp::socket> sock, session_pool* sp_);
};

class listener {
public:
	listener() : m_stop(false), pool_list(std::vector<session_pool*>()) {}
	void listener_start(string ip_addr, unsigned short port_number);
	void listener_stop();
	void assign_session_pool(session_pool* sp_);
private:
	void listener_run(string ip_addr, unsigned short port_number);

	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_stop;
	std::vector<session_pool*> pool_list;
	asio::io_context m_ios;
	friend class acceptor_;
};