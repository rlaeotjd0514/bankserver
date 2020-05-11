#pragma once
#include "pch.h"

using namespace std;
using namespace std::chrono;
//abcdefghijklmnopqrstuvwxyz875770417
namespace bank_info_type {	
	///<summary>date class(not useing)</summary>
	struct date {		
		unsigned short year;
		unsigned short month;
		unsigned short day;
		unsigned short hour;
		unsigned short minute;
		unsigned short second;
	};

	#pragma pack(push, 1)
	struct pinfo {
		uint8_t ppass[26];
		unsigned long pid;
	};

	///<summary>client seed convert class::</summary>
	class cspinfo {
	private:
		bool random_authorized = false;
		unsigned long csid;
		string cspass;
		char rpool[63] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789";
		const int passsize = 26;
	public:
		bool operator == (cspinfo& cspinfo_p) {
			if (cspinfo_p.csid == this->csid && cspinfo_p.cspass == this->cspass) return true;
			else return false;
		}

		bool operator == (string& cspinfo_s) {
			stringstream strstream;
			strstream << this->csid;
			string cspstring;
			strstream >> cspstring;
			if (cspstring + this->cspass == cspinfo_s) return true;
			else return false;
		}

		cspinfo& operator = (const cspinfo& cspinfo_p) {
			this->csid = cspinfo_p.csid;
			this->cspass = cspinfo_p.cspass;
			return *this;
		}

		cspinfo() {
			string pass;
			srand((unsigned int)time(0));
			this->csid = rand() % LONG_MAX;
			for (int i = 0; i < passsize; i++) {
				pass += rpool[rand() % sizeof(rpool)];
			}
			this->cspass = pass;
			this->random_authorized = true;
		}

		cspinfo(unsigned long csid_, string cspass_) {
			csid = csid_;
			cspass = cspass_;
		}		

		string printcspinfo() {
			return cspass.c_str();
		}
	};

	///<summary><br>transaction information</br>
	///<br>if client creates a session and send a request, it will create corresponding transaction for that request</br>
	///</summary>
	class tspinfo {
	private:
		bool random_authorized = false;
		unsigned long tsid;
		string tspass;
		char rpool[63] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789";
		const int passsize = 26;
	public:
		bool operator == (tspinfo& tspinfo_p) {
			if (tspinfo_p.tsid == this->tsid && tspinfo_p.tspass == this->tspass) return true;
			else return false;
		}

		bool operator == (string& tspinfo_s) {
			stringstream strstream;
			strstream << this->tsid;
			string tspstring;
			strstream >> tspstring;
			if (tspstring + this->tspass == tspinfo_s) return true;
			else return false;
		}

		tspinfo& operator = (const tspinfo& tspinfo_p) noexcept {
			this->tsid = tspinfo_p.tsid;
			this->tspass = tspinfo_p.tspass;
			return *this;
		}

		tspinfo(const tspinfo& t_) = delete;

		tspinfo() {
			string pass;
			srand((unsigned int)time(0));
			this->tsid = rand() % ULONG_MAX;
			for (int i = 0; i < passsize; i++) {
				pass += rpool[rand() % sizeof(rpool)];
			}
			this->tspass = pass;
			this->random_authorized = true;
		}

		tspinfo(unsigned long tspid_, string tsppass_) {
			tsid = tspid_;
			tspass = tsppass_;
		}
	};
	
	///<summary>transaction creator class</summary>
	class transaction {	
	public:
		enum class t_type { send, receive, deposit, withdraw, change_status};
		transaction(transaction::t_type type, cspinfo s, cspinfo r, unsigned long long amount, boost::asio::ip::tcp::endpoint loc, std::chrono::time_point<system_clock> req_t) {
  			transaction_type = type;
			sender_ip = s;
			receive_ip = r;
			this->amount = amount;
			this->location = loc;	
			this->tsinfo = tspinfo();
			this->transaction_ctime = std::chrono::system_clock::now();
			this->transaction_rtime = req_t;			
		}
	private:
		tspinfo tsinfo;
		t_type transaction_type;
		cspinfo sender_ip;
		cspinfo receive_ip;
		std::chrono::time_point<system_clock> transaction_ctime;
		std::chrono::time_point<system_clock> transaction_rtime;
		boost::asio::ip::tcp::endpoint location;
		unsigned long long amount;
	};

	///<summary>transaction result class</summary>
	class t_result {
	private:
		uint8_t result_code;
		string error_msg;
	public:
		t_result(uint8_t result_code_, string emsg) : result_code(result_code_), error_msg(emsg) {}
	};

	///<summary>individual information</summary>
	struct individual_info {
		wchar_t name[10];
		date birth{ 0, 0, 0, 0, 0, 0 };
		enum class gender { male, female };
		gender gender;
		vector<string> contact_list;
	};
	
	///<summary>client accout class</summary>
	class account {
	private:
		cspinfo csinfo;
		individual_info ip_info;
		unsigned long long balance;
		enum class account_level { root, administrator, worker, customer };
		enum class account_state { active, banned };
		enum class account_credit { prime, secondary, tiertiary, bad_credit };
		account_level level;
		account_state status;
		account_credit credit;
	public:		
		account(unsigned long long balance_, individual_info private_info_, account::account_level level_, account::account_credit credit_) {		
			ip_info = private_info_;
			balance = balance_;
			level = level_;
			credit = credit_;
		}
	};		
}