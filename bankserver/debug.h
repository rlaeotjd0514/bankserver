//#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//#include <windows.h>
//#include <fstream>
//#include <io.h>
//#include <fcntl.h>
//#include <stdio.h>
//
//using namespace std;
//
//class debug_stream {
//private:		
//	void dstream_init() {			
//		PROCESS_INFORMATION pi;
//		STARTUPINFO si;		
//		ZeroMemory(&pi, sizeof(pi));		
//		ZeroMemory(&si, sizeof(si));
//		si.dwFlags = STARTF_USESIZE | STARTF_USEPOSITION;
//		si.dwX = 100;
//		si.dwY = 50;
//		si.dwXSize = 640;
//		si.dwYSize = 480;
//		si.cb = sizeof(STARTUPINFO);
//		auto res = CreateProcess((LPSTR)"C:\\\\Windows\\System32\\cmd.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);	
//		FreeConsole();
//		AttachConsole(pi.dwProcessId);
//		cout << GetLastError() << endl;
//		//RedirectIOToConsole();		
//		//OutputDebugString(TEXT("debug test output string"));
//	}
//
//	static const WORD MAX_CONSOLE_LINES = 500;
//
//	void RedirectIOToConsole()
//	{
//		int hConHandle;
//		long lStdHandle;
//		CONSOLE_SCREEN_BUFFER_INFO coninfo;
//		FILE* fp;
//
//		FreeConsole();
//		// allocate a console for this app
//		if (AllocConsole() == 0) {
//			cout << GetLastError() << endl;
//		}		
//		else {			
//			cout << "succeed" << endl;
//		}
//
//		// set the screen buffer to be big enough to let us scroll text
//		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
//		coninfo.dwSize.Y = MAX_CONSOLE_LINES;
//		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);
//
//		// redirect unbuffered STDOUT to the console
//		lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
//		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//		fp = _fdopen(hConHandle, "w");
//
//		*stdout = *fp;
//
//		setvbuf(stdout, NULL, _IONBF, 0);
//
//		// redirect unbuffered STDIN to the console
//
//		lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
//		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//		fp = _fdopen(hConHandle, "r");
//		*stdin = *fp;
//		setvbuf(stdin, NULL, _IONBF, 0);
//
//		// redirect unbuffered STDERR to the console
//		lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
//		hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
//
//		fp = _fdopen(hConHandle, "w");
//
//		*stderr = *fp;
//
//		setvbuf(stderr, NULL, _IONBF, 0);
//
//		// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
//		// point to console as well
//		ios::sync_with_stdio();
//		cout << "hi im new console" << endl;
//	}
//	vector<string> d_log;
//public:
//	string print_debug_log() {
//		string dl;		
//		for (string log : d_log) {
//			dl += log;
//		}
//		d_log.clear();
//		return dl;
//	}
//		
//	debug_stream& operator<<(string s) {
//		d_log.push_back(s + "\n");
//		cout << s;
//		return *this;
//	}	
//
//	debug_stream() {
//		dstream_init();
//	}
//};