#include "listener.h"

class listener;

acceptor_::acceptor_(asio::io_context& ios, unsigned short port_number) :
	m_ios(ios),
	m_acceptor(m_ios, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port_number))
{
	m_acceptor.listen();
}

void acceptor_::accept_() {
	std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(m_ios));
	m_acceptor.accept(*sock.get());
	(new service)->handler_function(sock);
}