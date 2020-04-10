#pragma once

#include <boost/asio.hpp>

#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

using namespace boost;
using namespace std;

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
			asio::read_until(*sock.get(), req, '\n');
			/*
			parse req stream &
			add to session queue 
			*/
			string res = "server response";
			asio::write(*sock.get(), asio::buffer(res));
		}
		catch (system::system_error& e) {
			OutputDebugString(e.what());
		}
		delete this;
	}
};

class acceptor {
public:
	acceptor(asio::io_service& ios, unsigned short port_number) :
		m_ios(ios),
		m_acceptor(m_ios, asio::ip::tcp::endpoint(asio::ip::address_v4::any(), port_number))
	{
		m_acceptor.listen();
	}

	void accept_() {
		auto sock = make_shared<asio::ip::tcp::socket>(m_ios);
		m_acceptor.accept(*sock.get());

		(new service)->handler_function(sock);
	}

private:
	asio::io_service& m_ios;
	asio::ip::tcp::acceptor m_acceptor;
};

class listener {
public:
	listener() : m_stop(false) {}
	void listener_start(unsigned short port_number) {
		m_thread.reset(new std::thread([this, port_number]() {
			listener_run(port_number);
			}));
	}


private :
	void listener_run(unsigned short port_number) {
		acceptor acptr(m_ios, port_number);
		while (!m_stop.load()) {
			acptr.accept_();
		}
	}
	std::unique_ptr<std::thread> m_thread;
	std::atomic<bool> m_stop;
	asio::io_service m_ios;
};

