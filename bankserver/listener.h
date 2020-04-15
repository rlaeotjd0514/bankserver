#pragma once
#include "pch.h"

using namespace boost;
using boost::asio::ip::tcp;
using namespace bank_network_methods;

class listener;

class acceptor_ {
public:
	acceptor_(asio::io_context& ios, unsigned short port_number);
	void accept_();
private:
	asio::io_context& m_ios;
	tcp::acceptor m_acceptor;	
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
	void handle_client(std::shared_ptr<asio::ip::tcp::socket> sock) {
		try {
			asio::streambuf req;
			asio::read_until(*sock.get(), req, 0x4B4D4753);
			/*
			parse req stream &
			add to session queue 
			*/

			uint8_t* req_byte = (uint8_t*)req.data().data();

			find_signature(&req_byte, req.data().size());

			
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
	listener() : m_stop(false) {}
	void listener_start(unsigned short port_number) {
		m_thread.reset(new std::thread([this, port_number]() {
			listener_run(port_number);
			}));
	}
	void listener_stop() {
		m_stop.store(true);
		m_thread->join();
	}

private:
	void listener_run(unsigned short port_number) {
		acceptor_ acptr(m_ios, port_number);
		while (!m_stop.load()) {
			acptr.accept_();
		}
	}

	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_stop;
	asio::io_context m_ios;
	friend class acceptor_;
};




