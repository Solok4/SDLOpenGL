#include "StdLibWrapper.h"
#include <cstring>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include "../../CLog.h"

StdLibWrapper::StdLibWrapper()
{

}

StdLibWrapper::~StdLibWrapper()
{
    
}

int StdLibWrapper::Sstrcmp(const char* a, const char* b)
{
    #ifdef __WIN32
        return strcmp(a,b);
    #else
        return strcmp(a,b);
    #endif
}

void StdLibWrapper::Ssprintf(char* buffer, const char* format, ...)
{
    va_list vl;
	va_start(vl, format);
    StdLibWrapper::Svsprintf(buffer,format,vl);
    va_end(vl);
}

int StdLibWrapper::Svsprintf(char* buffer, const char* format, va_list list)
{
    #ifdef __WIN32
        return vsprintf_s(buffer, format, list);
    #else
        return vsprintf(buffer, format, list);
    #endif
}

char* StdLibWrapper::Sstrcat(char* a, char* b)
{
    #ifdef __WIN32
        return strcat_s(a,b)
    #else
        return strcat(a,b);
    #endif
}

char* StdLibWrapper::Sstrncpy(char* buffer, const char* string, int lenght)
{
    #ifdef __WIN32
        return strcat_s(a, b)
    #else
        return strncpy(buffer, string, lenght);
    #endif
}

int StdLibWrapper::Sstrcmpi(const char* a, const char* b)
{
    #ifdef __WIN32
        return _strcmpi(a, b);
    #else
        return strcmp(a, b);
    #endif
}

char* StdLibWrapper::Sstrcpy(char* buffer, const char* string)
{
    #ifdef __WIN32
        return strcpy_s(a, b);
    #else
        return strcpy(buffer, string);
    #endif
}

FILE* StdLibWrapper::Sfopen(FILE* file, const char* path, const char* mode)
{
    #ifdef __WIN32
        fopen_s(&file,path,mode);
    #else
        return fopen(path, mode);
    #endif
}

int StdLibWrapper::Sfscanf(FILE* file, unsigned long length, const char* format, ...)
{
    va_list vl;
	va_start(vl, format);
    int result = 0;
    #ifdef __WIN32
        result = vfscanf_s(file,format, vl,length);
    #else
        result = vfscanf(file, format, vl);
    #endif
    va_end(vl);
    return result;
}