#pragma once
#include <string>

class CLog
{
public:
	CLog();
	~CLog();

	static void MyLog(int key, const std::string& Message...);
};

