#pragma once
#include <vector>
#include <sstream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include "types.h"

using namespace std;
using namespace std::chrono;

namespace bank_info_type {	
	///��¥
	class date {
		unsigned short year;
		unsigned short month;
		unsigned short day;
		unsigned short hour;
		unsigned short minute;
		unsigned short second;
	};

	///�������õ�
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
		}

		cspinfo() {
			string pass;
			srand(time(0));
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
	};

	///Ʈ����� �����õ�
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
		}

		tspinfo(const tspinfo& t_) {
			throw new exception("ERROR::CAN NOT COPY TSPINFO INSTANCE");
		}

		tspinfo() {
			string pass;
			srand(time(0));
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
	///�ŷ� Ʈ�����
	class transaction {	
	public:
		enum class t_type { send, receive, deposit, withdraw, change_status};
		transaction(transaction::t_type type, cspinfo s, cspinfo r, unsigned long long amount, uint32_t loc, std::chrono::time_point<system_clock> req_t) {
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
		uint32_t location;
		unsigned long long amount;
	};

	///Ʈ����� ���ǥ
	class t_result {
	private:
		uint8_t result_code;
		string error_msg;
	public:
		t_result(uint8_t result_code_, string emsg) : result_code(result_code_), error_msg(emsg) {}
	};

	///�������� :=>����
	class individual_info {
		wchar_t name[10];
		date birth;
		enum class gender { male, female };
		gender gender;
		vector<string> contact_list;
	};
	
	///����
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