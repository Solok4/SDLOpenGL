#pragma once
#include <memory>
#include "CConsole.h"
class CInitialSetup
{
public:
	CInitialSetup()=delete;
	CInitialSetup(int argv, char* argc) :ArgumentCount(argv), ArgumentContent(argc)
	{
		//Console = std::make_unique<CConsole>();
	};
	~CInitialSetup();


private:
	int ArgumentCount;
	char* ArgumentContent;
};

extern std::unique_ptr<CInitialSetup> InitialSetup;

