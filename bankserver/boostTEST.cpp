#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include "debug.h"
#include <string>
#include "session.h"
#include "session_pool.h"

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio::chrono;

session_pool sp;

void print(const boost::system::error_code&) {
	std::cout << "Hello, world!" << endl;
}

void session_test() {
	cspinfo * private_key = new cspinfo(1, "abcdefghijkemnopqrstuvwxyz");
	session s1(0, private_key, 0, 5);
	cout << "Session Started" << endl;
	cout << "Session validation : " << (s1.check_session_validation() ? "true" : "false") << endl;		
	std::this_thread::sleep_for(std::chrono::seconds(10));
	auto ret = s1.check_session_validation();
	cout << "Session validation : " << (ret ? "true" : "false") << endl;
}

void session_pool_test() {
	cspinfo* private_key = new cspinfo(1, "abcdefghijkemnopqrstuvwxyz");
	session s1(0, private_key, 0, 5);
	sp.add_session(s1);
	sp.session_start();
}

void c_test() {
	/*boost::asio::io_service io;
	boost::asio::io_context io2;
	boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
	boost::asio::steady_timer t2(io, std::chrono::seconds(5));
	t2.async_wait([&t2](boost::system::error_code  e) {
		cout << "5 seconds passed" << endl;
	});
	io.run();
	io2.run();
	tcp::acceptor accepctor(io2, tcp::endpoint(tcp::v4(), 1313));
	for (;;) {
		tcp::socket socket(io2);
		cout << "waiting connection at " << accepctor.local_endpoint().address().to_string() << ":" << accepctor.local_endpoint().port() << endl;
		accepctor.listen(20);
		accepctor.accept(socket);
		cout << "connected from << " << socket.remote_endpoint().address().to_string() << endl;
		string message("hi world");
		boost::system::error_code igcode;
		boost::asio::write(socket, boost::asio::buffer(message), igcode);
		socket.shutdown(boost::asio::socket_base::shutdown_type::shutdown_both, igcode);
	}
	cout << sizeof(chrono::time_point<system_clock>) << endl;	*/
}

int main() {	
	session_pool_test();	
	return 0; 
}