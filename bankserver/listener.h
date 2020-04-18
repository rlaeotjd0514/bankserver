#pragma once
#include "pch.h"
#include "BankProtocol.h"
#include "DBQuery.h"
#include "session_pool.h"
#include "stringcontrol.h"

using namespace boost;
using namespace db_controller;
using boost::asio::ip::tcp;

class listener;

class acceptor_ {
public:
	acceptor_(asio::io_context& ios, unsigned short port_number, session_pool * sp_);
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

	void handler_function(std::shared_ptr<asio::ip::tcp::socket> sock, session_pool* sp_) {
		std::thread th(([this, sock, sp_]() {
			uint8_t* req_byte = handle_client(sock);
			/*
			parse req stream & add to session queue
			*/
			pinfo* inquery_customer = (pinfo*)req_byte[0];
			tcp::endpoint cli_ep = sock->remote_endpoint();
			/*
			parse ended
			*/
			bank_query* bq = new bank_query();
			if (bq->query_single_customer(*inquery_customer)) {
				//세션을 만들고 / session_list 에 넣기
				cspinfo tc_csp = cspinfo(inquery_customer->pid, string((char*)inquery_customer->ppass));
				session* clis = session::make_session(sp_->get_session_count() + 1, tc_csp, rand() % INT_MAX, 5, cli_ep);
				sp_->add_session(*clis);
			}
		}));
		th.detach();
	}
private:	
	uint8_t* handle_client(std::shared_ptr<asio::ip::tcp::socket> sock) {
		try {
			asio::streambuf req;
			asio::read_until(*sock.get(), req, SGMK_LE);			
			uint8_t* req_byte = (uint8_t*)req.data().data();
			if (find_signature(&req_byte, req.data().size())) {
				return nullptr;
			}
			else {
				return req_byte;						
			}			
			/*std::string res((std::istreambuf_iterator<char>(&req)), std::istreambuf_iterator<char>());		
			stringcontroler::replace_string(res, (const char *)0x4B4D4753, "");
			asio::write(*sock.get(), asio::buffer(string((char*)req_byte)));
			sock.get()->close();*/
		}
		catch (system::system_error& e) {
			OutputDebugString(e.what());
		}
		delete this;
	}
};

class listener {
public:
	listener() : m_stop(false), pool_list(std::vector<session_pool*>()) {}
	void listener_start(unsigned short port_number) {
		m_thread.reset(new std::thread([this, port_number]() {
			listener_run(port_number);
			}));
	}
	void listener_stop() {
		m_stop.store(true);
		m_thread->join();
	}
	void assign_session_pool(session_pool* sp_) {
		pool_list.push_back(sp_);
	}
private:
	void listener_run(unsigned short port_number) {		
		acceptor_ acptr(m_ios, port_number, new session_pool());
		while (!m_stop.load()) {
			acptr.accept_();
		}
	}

	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_stop;
	std::vector<session_pool*> pool_list;
	asio::io_context m_ios;
	friend class acceptor_;
};




