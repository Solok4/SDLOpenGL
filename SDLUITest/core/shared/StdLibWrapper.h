#pragma once

#include <cstdarg>
#include <cstdio>

class StdLibWrapper {
public:
    StdLibWrapper();
    ~StdLibWrapper();

    static int Sstrcmp(const char* , const char* );
    static void Ssprintf(char*, const char*, ...);
    static int Svsprintf(char*, const char*, va_list);
    static char* Sstrcat(char*, char*);
    static char* Sstrncpy(char*, const char*, int);
    static int Sstrcmpi(const char*, const char*);
    static char* Sstrcpy(char*, const char*);
    static FILE* Sfopen(const char*,const char*);
    static int Sfscanf(FILE*, unsigned long, const char*, ...);
};