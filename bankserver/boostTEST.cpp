#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include "debug.h"
#include <string>
#include "session.h"

using namespace std;
using boost::asio::ip::tcp;
using namespace boost::asio::chrono;

void print(const boost::system::error_code&) {
	std::cout << "Hello, world!" << endl;
}

void debug_test(string ds) {
	debug_stream();
}

void session_test() {
	session(0, cspinfo(1, "abcdefghijkemnopqrstuvwxyz"), 0, 5);
	cout << "return to main" << endl;
}

int main() {
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
	session_test();
	io_service io;
	steady_timer st(io, std::chrono::seconds(10));
	st.async_wait([&st](boost::system::error_code e) {});
	io.run();
	return 0;
}