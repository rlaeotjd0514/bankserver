#include "listener.h"

class listener;

void service::handler_function(std::shared_ptr<asio::ip::tcp::socket> sock, session_pool* sp_) {
	std::thread th(([this, sock, sp_]() {
		uint8_t* req_byte = handle_client(sock, sp_);
		}));
	th.detach();
}

uint8_t* service::handle_client(std::shared_ptr<asio::ip::tcp::socket> sock, session_pool* sp_) {
	try {
		asio::streambuf req;
		asio::read_until(*sock.get(), req, SGMK_LE);
		uint8_t* req_byte = (uint8_t*)req.data().data();
		if (find_signature(&req_byte, req.data().size()) == nullptr) {
			asio::write(*sock.get(), asio::buffer("illegal connection"));
			sock->shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
			sock->close();
			return nullptr;
			//return or occur ERROR::signature not found.
		}
		else {
			/*
			parse req stream & add to session queue
			*/
			pinfo* inquery_customer = (pinfo*)req_byte;
			tcp::endpoint cli_ep = sock->remote_endpoint();
			/*
			parse ended
			*/
			bank_query* bq = new bank_query("dbname=CSPDB host=192.168.91.128 user=OKABE_RINTARO password=fhf10Rnfwoa");
			auto qres = bq->query_single_customer(*inquery_customer);
			cout << qres.what << endl;
			if (qres.result == action_status::success) {
				/*Create session and insert into session_pool cycle*/
				cspinfo tc_csp = cspinfo(inquery_customer->pid, string((char*)inquery_customer->ppass));
				session* clis = session::make_session(sp_->get_session_count() + 1, tc_csp, rand() % INT_MAX, 5, cli_ep);//heap allocation.
				/*clis will be only used as information trasporting instance (must create another class for this job, but not now)*/
				sp_->add_session(clis);
				asio::write(*sock.get(), asio::buffer("queued"));
				sock.get()->shutdown(asio::socket_base::shutdown_both);
				sock.get()->close();
				return req_byte;
			}
			else {
				asio::write(*sock.get(), asio::buffer("invalid client pass"));
				sock->shutdown(boost::asio::socket_base::shutdown_type::shutdown_both);
				sock->close();
				return nullptr;
			}
			delete bq;
		}
		/*std::string res((std::istreambuf_iterator<char>(&req)), std::istreambuf_iterator<char>());
		stringcontroler::replace_string(res, (const char *)0x4B4D4753, "");
		asio::write(*sock.get(), asio::buffer(string((char*)req_byte)));
		sock.get()->close();*/
	}
	catch (system::system_error& e) {
		OutputDebugString(e.what());
	}
	delete this;
}

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

void listener::listener_start(string ip_addr, unsigned short port_number) {
	m_thread.reset(new std::thread([this, ip_addr, port_number]() {
		listener_run(ip_addr, port_number);
		}));
}

void listener::listener_stop() {
	m_stop.store(true);
	m_thread->join();
}

void listener::assign_session_pool(session_pool* sp_) {
	pool_list.push_back(sp_);
}

void listener::listener_run(string ip_addr, unsigned short port_number) {
	acceptor_ acptr(m_ios, ip_addr, port_number, pool_list);
	while (!m_stop.load()) {
		acptr.accept_();
	}
}