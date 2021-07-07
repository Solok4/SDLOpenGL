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

	static void debug(const char* message, ...);
	static void info(const char* message, ...);
	static void error(const char* message, ...);
};
