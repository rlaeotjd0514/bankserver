#include "listener.h"

class listener;

///<summary><br>acceptor_ class acts as an middle gate between server and client</br>
///<br>acceptor_ class will set as listen mode when it's constructor has been called.</br></summary>
acceptor_::acceptor_(asio::io_context& ios, string ip_addr, unsigned short port_number, vector<session_pool*>sp_) :
	m_ios(ios),
	m_acceptor(m_ios, tcp::endpoint(boost::asio::ip::address::from_string(ip_addr), port_number))//127.0.0.1 to bind address
{
	sp_list = sp_;
	m_acceptor.listen();	
}

///<summary><br>accept_ method is called when client sends a request pakcet.</br>
///<br>Parses a packet and find for a available session_pool.</br>
///<br>Session will be automatically start when it is inserted in to session_pool.</br>
///<br>Two jobs will be done asynchronously.</br></summary>
void acceptor_::accept_() {
	std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(m_ios));
	m_acceptor.accept(*sock.get());

	session_pool * available_pool = nullptr;
	for (session_pool * sp : sp_list) {
		if (sp->get_session_count() < 100) {
			available_pool = sp;
			break;
		}
	}
	if (available_pool == nullptr) {
		throw new std::exception("listener::pool is not available");
	}

	(new service)->handler_function(sock, available_pool);
}