#pragma once
#include <memory>
#include "CConsole.h"
class CInitialSetup
{
public:
	CInitialSetup()=delete;
	CInitialSetup(int argv, char** argc) :ArgumentCount(argv), ArgumentContent(argc)
	{
		Console = std::make_unique<CConsole>();
		Console->AddCommand(std::make_shared<CVar>("TestInteger", 10, "Testing integer cvar"));
		Console->ChangeValueOfGivenCommand("TestInteger", 11);
		Console->AddCommand(std::make_shared<CVar>("TestDouble", 10.2, "Testing double cvar"));
		Console->ChangeValueOfGivenCommand("TestDouble", 11.55);
		Console->AddCommand(std::make_shared<CVar>("TestString", std::string("Test string value"), "Testing string cvar"));
		Console->ChangeValueOfGivenCommand("TestString", std::string("Changed string value"));
	};
	~CInitialSetup();


private:
	int ArgumentCount;
	char** ArgumentContent;
};

extern std::unique_ptr<CInitialSetup> InitialSetup;

