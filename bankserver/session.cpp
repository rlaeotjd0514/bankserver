#include "pch.h"
#include "session.h"
#include "bank_protocol.h"

using namespace std;
using namespace bank_network_methods;

///<summary>Session is used when actual communication is happening with client.
///<para>s_number : not used </para>
///<para>csp_ : information of client that has created this session(Client Secret Password)</para>
///<para>secure_seed_ : used when encrypting communication</para>
///<para>expire_time_ : session limit time.(session crashes after expire time)</para>
///<para>cli_socket_ : used for getting client's approximate location</para>
///</summary>
session::session(uint32_t s_number_, cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_, tcp::endpoint cli_socket_) :
	session_expr(true),
	session_number(s_number_),
	session_seed(rand() % ULLONG_MAX),
	secure_seed(secure_seed_),
	current_customer(csp_),
	expire_time(expire_time_),
	current_pool(nullptr),
	cli_socket(cli_socket_),
	session_timer(HI_timer::timer_type::deadline, expire_time_, (long long)0,
		[&]() {
			this->change_validation();
			cout << "session have expired" << endl;			
		}, []() {})	
{				
	cout << "session ctor done" << endl;
}

session::session(const session& cp)//Aim not to use copy con
{
	this->cli_socket = cp.cli_socket;
	this->current_customer = cp.current_customer;
	this->current_pool = cp.current_pool;
	this->expire_time = cp.expire_time;	
	this->secure_seed = cp.secure_seed;
	this->session_expr = cp.session_expr;
	this->session_timer = cp.session_timer;
	this->session_number = cp.session_number;
}

///<summary>Returns session object pointer</summary>
session* session::make_session(uint32_t s_number_, cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_, tcp::endpoint cli_socket_) {
	return new session(s_number_, csp_, secure_seed_, expire_time_, cli_socket_);
}

session* session::make_session(session && s_) {
	return new session(s_.session_number, s_.current_customer, s_.secure_seed, s_.expire_time, s_.cli_socket);
}

///<summary>Starts Session clock</summary>
void session::start_session_clock() {
	this->session_timer.start(&(this->session_timer));
}

///<summary>Reset session clock</summary>
void session::reset_session_clock() {
	
}
///<summary>Add to session clocks remaining time</summary>
void session::add_session_time(int add_time_) {
	this->session_timer.add_to_left_time(add_time_);	
}

void send_cmplt_handler(const boost::system::error_code& error, size_t s) {
	cout << "response complete";
}

///<summary>Accept client connection request.
///this function remains block until client sends connection request
///</summary>
tcp::socket session::accept_client(tcp::endpoint cli_ep)
{	
	boost::asio::io_service a_ios;
	boost::asio::io_service dc;
	boost::asio::ip::tcp::socket sock(a_ios);
	boost::asio::ip::tcp::acceptor gate = boost::asio::ip::tcp::acceptor(a_ios, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), cli_ep.port() - 1));
	boost::asio::deadline_timer accept_ternel = boost::asio::deadline_timer(dc);	
	accept_ternel.expires_from_now(boost::posix_time::seconds(5));
	accept_ternel.async_wait([&](const boost::system::error_code & ec) {
		gate.cancel();
		gate.close();
	});	
	thread th([&]() {
		try {
			dc.run();
		}
		catch (boost::system::error_code & ec) {}
	});
	th.detach();
	try {				
		gate.accept(sock);
		accept_ternel.cancel();
		if (!dc.stopped()) {
			dc.stop();			
		}

		uint8_t* packet_buf = new uint8_t[csp_packet_size];		
		sock.receive(boost::asio::buffer(packet_buf, csp_packet_size));
		csp_packet* packet_p = (csp_packet*)packet_buf;

		string server_response("Server's Response");
		sock.async_send(boost::asio::buffer(server_response, server_response.length()), send_cmplt_handler);
		cout << "client connection succeed::" << this->current_customer.printcspinfo() << endl;
		this->start_session_clock();

		//handle client request...

		

		/*this_thread::sleep_for(std::chrono::seconds(4));
		cout << (this->check_session_validation() ? "true" : "false") << endl;
		this_thread::sleep_for(std::chrono::seconds(2));
		cout << (this->check_session_validation() ? "true" : "false") << endl;*/
		
		gate.close();
		sock.shutdown(boost::asio::socket_base::shutdown_both);
		sock.close();
	}
	catch (boost::system::system_error& e) {
		OutputDebugString(e.what());
		cout << e.what() << endl;
	}
			
	return sock;//should be returning result.
}

///<summary>Expire session(sends exit signal to under threads)</summary>
void session::expire_session() {
	this->session_timer.expire_timer();
}

///<summary>Should not be used by lib user. Used on changing session validation.</summary>
void session::change_validation() noexcept {
	mtx.lock();
	session_expr = !session_expr;
	mtx.unlock();
}

///<summary>Returns session validation</summary>
bool session::check_session_validation() const {
	return session_expr;
}

session::~session() {
	this->session_timer.set_left_time(0);
	cout << "session dtor called" << endl;
}




