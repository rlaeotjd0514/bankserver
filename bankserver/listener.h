#pragma once
#include "pch.h"
#include "DBQuery.h"
#include "session_pool.h"
#include "BankProtocol.h"

using namespace boost;
using namespace bank_network_methods;
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

class service {
public :
	service() {}

	void handler_function(std::shared_ptr<asio::ip::tcp::socket> sock) {
		std::thread th(([this, sock]() {
			handle_client(sock);
			}));
		th.detach();
	}
private:
	///<summary> 핸들 클라이언트의 반환값을 특정한 코드(이넘)같은걸로 바꾸고 클라이언트가 연결했을 때 정보를 반환하여 실제 처리는 acceptor에서 처리하도록 변경. service 클래스는 순수하게 클라이언트와의 통신만을 담당함</summary>
	void handle_client(std::shared_ptr<asio::ip::tcp::socket> sock) {
		try {
			asio::streambuf req;
			asio::read_until(*sock.get(), req, SGMK_LE);
			/*
			parse req stream &
			add to session queue 
			*/

			uint8_t* req_byte = (uint8_t*)req.data().data();
			find_signature(&req_byte, req.data().size());
			pinfo* inquery_customer = (pinfo*)req_byte[0];
			tcp::endpoint cli_ep = sock->remote_endpoint();
			if (query_single_customer(*inquery_customer)) {
				
			}			

			/*
			parse ended
			*/
			std::string res((std::istreambuf_iterator<char>(&req)), std::istreambuf_iterator<char>());		
			stringcontroler::replace_string(res, (const char *)0x4B4D4753, "");
			asio::write(*sock.get(), asio::buffer(string((char*)req_byte)));
			sock.get()->close();
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




