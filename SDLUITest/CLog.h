#pragma once
#include<string>

#ifndef __EMSCRIPTEN__
#define HD4850
#endif


enum LogType
{
	Log = 0,
	Debug,
	Warning,
	Error,
};

class CLog
{
public:
	CLog();
	~CLog();

	static void MyLog(LogType type, const char* Message,...);
};

