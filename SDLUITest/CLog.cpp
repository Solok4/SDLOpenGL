#include "pch.h"
#include "CLog.h"

#include <iostream>
#include <stdio.h>
#include <stdarg.h>



CLog::CLog()
{
}


CLog::~CLog()
{
}

void CLog::MyLog(int key, const char* Message,...)
{
	char VAContent[2048];
	char Final[2048];
	switch (key)
	{
	case 1: {
		//Warning
		strncpy(Final,"[Warning]: ",12);
		break;
	}

	default:
		//Log
		strncpy(Final,"[Log]: ",8);
		//Final = "[Log]: ";
		break;
	}
	va_list vl;
	va_start(vl, Message);
	vsprintf(VAContent, Message, vl);
	strcat(Final, VAContent);
	printf("%s\n", Final);
	va_end(vl);



	//std::cout << Prefix << Message << std::endl;
}
