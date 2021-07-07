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

void CLog::debug(const char* message, ...) {
	char VAContent[2048];
	char Final[2048];

#ifdef __EMSCRIPTEN__
		strncpy(Final, "[Debug]: ", 10);
#else
		strncpy_s(Final, "[Debug]: ", 10);
#endif
	va_list vl;
	va_start(vl, message);
#ifdef __EMSCRIPTEN__
	vsprintf(VAContent, Message, vl);
	strcat(Final, VAContent);
#else
	vsprintf_s(VAContent, message, vl);
	strcat_s(Final, VAContent);
#endif
	printf("%s\n", Final);
	va_end(vl);
}

void CLog::info(const char* message, ...) {
	char VAContent[2048];
	char Final[2048];
#ifdef __EMSCRIPTEN__
		strncpy(Final, "[INFO]: ", 8);
#else
		strncpy_s(Final, "[INFO]: ", 8);
#endif
	va_list vl;
	va_start(vl, message);
#ifdef __EMSCRIPTEN__
	vsprintf(VAContent, Message, vl);
	strcat(Final, VAContent);
#else
	vsprintf_s(VAContent, message, vl);
	strcat_s(Final, VAContent);
#endif
	printf("%s\n", Final);
	va_end(vl);
}

void CLog::error(const char* message, ...){
	char VAContent[2048];
	char Final[2048];
#ifdef __EMSCRIPTEN__
	strncpy(Final, "[ERROR]: ", 9);
#else
	strncpy_s(Final, "[ERROR]: ", 9);
#endif
	va_list vl;
	va_start(vl, message);
#ifdef __EMSCRIPTEN__
	vsprintf(VAContent, Message, vl);
	strcat(Final, VAContent);
#else
	vsprintf_s(VAContent, message, vl);
	strcat_s(Final, VAContent);
#endif
	printf("%s\n", Final);
	va_end(vl);
}

