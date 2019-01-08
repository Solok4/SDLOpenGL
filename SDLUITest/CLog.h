#pragma once
#include<string>

class CLog
{
public:
	CLog();
	~CLog();

	static void MyLog(int key, std::string Message,...);
};

