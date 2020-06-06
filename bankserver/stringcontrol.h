#pragma once
#include "pch.h"

using namespace std;

class stringcontroler {
public:
	static string replace_string(string& s, const char* old_, const char* new_) {	
		if (s.empty() == true) return s;
		while (s.find(old_) != std::string::npos) {
			s.replace(s.find(old_), sizeof(old_) - 1, new_);
		}
		return s;
	}	
};

class stringconvertor {
public:
	static uint8_t* constchar_to_uint8array(const char* buf) {
		return (uint8_t*)buf;
	}
};