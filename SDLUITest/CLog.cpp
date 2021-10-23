#include "CLog.h"
#include "core/shared/Shared.h"
#include <string>

CLog::CLog()
{
}

CLog::~CLog()
{
}

void CLog::debug(const char* message, ...) {
	char VAContent[2048];

	va_list vl;
	va_start(vl, message);
	StdLibWrapper::Svsprintf(VAContent,message,vl);
	va_end(vl);
	std::string finalMessage = "[DEBUG]: ";
	finalMessage += VAContent;
	printf("%s\n", finalMessage.c_str());
}

void CLog::info(const char* message, ...) {
	char VAContent[2048];

	va_list vl;
	va_start(vl, message);
	StdLibWrapper::Svsprintf(VAContent,message,vl);
	va_end(vl);
	std::string finalMessage = "[INFO]: ";
	finalMessage += VAContent;
	printf("%s\n", finalMessage.c_str());
}

void CLog::error(const char* message, ...){
	char VAContent[2048];

	va_list vl;
	va_start(vl, message);
	StdLibWrapper::Svsprintf(VAContent,message,vl);
	va_end(vl);
	std::string finalMessage = "[ERROR]: ";
	finalMessage += VAContent;
	printf("%s\n", finalMessage.c_str());
}

