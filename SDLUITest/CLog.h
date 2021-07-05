#pragma once
#include<string>

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

	static void MyLog(LogType type, const char* Message, ...);
};
