#include "pch.h"
#include "session.h"

using namespace std;

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

session::session(const session& cp)
{
	this->cli_socket = cp.cli_socket;
	this->current_customer = cp.current_customer;
	this->current_pool = cp.current_pool;
	this->expire_time = cp.expire_time;	
	this->secure_seed = cp.secure_seed;
	this->session_expr = this->session_expr;
	this->session_timer = this->session_timer;
	this->session_number = this->session_number;
}

///<summary>Returns session object pointer</summary>
session* session::make_session(uint32_t s_number_, cspinfo csp_, uint32_t secure_seed_, uint32_t expire_time_, tcp::endpoint cli_socket_) {
	return new session(s_number_, csp_, secure_seed_, expire_time_, cli_socket_);
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

///<summary>Accept client connection request.
///this function remains block until client sends connection request
///</summary>
tcp::socket session::accept_client(tcp::endpoint cli_ep)
{	
	boost::asio::io_service a_ios;
	boost::asio::ip::tcp::socket sock(a_ios, cli_ep.protocol());
	boost::asio::ip::tcp::acceptor gate = boost::asio::ip::tcp::acceptor(a_ios, tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), cli_ep.port()));
	try {
		gate.listen();
		gate.accept(sock);
	}
	catch (boost::system::system_error& e) {
		OutputDebugString(e.what());
	}

	this->start_session_clock();
	//handle client request...

	//handling request code;

	//////////////////////////	
	return sock;//should be returning result.
}

///<summary>Expire session(sends exit signal to under threads)</summary>
void session::expire_session() {
	this->session_timer.expire_timer();
}

///<summary>Should not be used by lib user. Used on changing session validation.</summary>
void session::change_validation() noexcept {
	mtx.lock();
	this->session_expr = !(this->session_expr);
	mtx.unlock();
}

///<summary>Returns session validation</summary>
bool session::check_session_validation() const {
	return session_expr;
}




