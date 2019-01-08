#include "pch.h"
#include "CLog.h"

#include <iostream>



CLog::CLog()
{
}


CLog::~CLog()
{
}

void CLog::MyLog(int key, std::string Message,...)
{
	std::string Prefix;
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
