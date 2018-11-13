#include "pch.h"
#include "CLog.h"

#include<iostream>


CLog::CLog()
{
}


CLog::~CLog()
{
}

void CLog::MyLog(int key, const std::string& Message...)
{
	const char* Prefix;
	switch (key)
	{
	case 1: {
		//Warning
		Prefix = "[Warning]: ";
		break;
	}

	default:
		//Log
		Prefix = "[Log]: ";
		break;
	}

	std::cout << Prefix << Message << std::endl;
}
