#pragma once

#include <string>
#include <cstring>
#include <regex>

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