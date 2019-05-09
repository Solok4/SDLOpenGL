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
#ifdef __EMSCRIPTEN__
		strncpy(Final, "[Warning]: ", 12);
#else
		strncpy_s(Final,"[Warning]: ",12);
#endif
		break;
	}

	default:
		//Log
#ifdef __EMSCRIPTEN__
		strncpy(Final, "[Log]: ", 8);
#else
		strncpy_s(Final,"[Log]: ",8);
#endif
		break;
	}
	va_list vl;
	va_start(vl, Message);
#ifdef __EMSCRIPTEN__
	vsprintf(VAContent, Message, vl);
	strcat(Final, VAContent);
#else
	vsprintf_s(VAContent, Message, vl);
	strcat_s(Final, VAContent);
#endif
	printf("%s\n", Final);
	va_end(vl);



	//std::cout << Prefix << Message << std::endl;
}
