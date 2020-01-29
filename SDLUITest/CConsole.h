//#pragma once
//#include <vector>
#include <memory>
#include <deque>
#include <cstring>
#include <vector>

#include "CVar.h"
#include "CLog.h"

class CConsole
{
public:
	CConsole();
	~CConsole();
	std::shared_ptr<CVar> GetCommandByName(std::string name);
	//void ChangeValueOfGivenCommand(std::string name, std::variant<bool,int,float> value);
	void AddCommand(std::shared_ptr<CVar> Command);

private:
	std::vector<std::shared_ptr<CVar>> ConsoleCommands;
	std::deque<std::string> CommandBuffer;
	
};

extern std::unique_ptr<CConsole> Console;

