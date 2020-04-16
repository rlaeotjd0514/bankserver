#include "listener.h"

class listener;

acceptor_::acceptor_(asio::io_context& ios, unsigned short port_number, session_pool * sp_) :
	m_ios(ios),
	m_acceptor(m_ios, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port_number))//127.0.0.1 to bind address
{
	sp_list.push_back(sp_);
	m_acceptor.listen();
}

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

	(new service)->handler_function(sock);
}