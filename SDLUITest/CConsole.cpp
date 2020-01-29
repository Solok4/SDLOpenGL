#include "CConsole.h"

std::unique_ptr<CConsole> Console;


CConsole::CConsole()
{
}


CConsole::~CConsole()
{
}

std::shared_ptr<CVar> CConsole::GetCommandByName(std::string name)
{
	if (this->ConsoleCommands.size())
	{
		for (auto a : this->ConsoleCommands)
		{
			if (_strcmpi(a->GetName(), name.c_str()) == 0)
			{
				return a;
			}
		}
	}
	return {};
}

//void CConsole::ChangeValueOfGivenCommand(std::string name, std::variant<bool, int, float> value)
//{
//	auto Command = this->GetCommandByName(name);
//	if (Command != nullptr)
//	{
//		Command->SetCurrentValue(value);
//	}
//}

void CConsole::AddCommand(std::shared_ptr<CVar> Command)
{
	if (Command != nullptr)
	{
		bool found = false;
		for (auto a : this->ConsoleCommands)
		{
			if (_strcmpi(Command->GetName(), a->GetName()) == 0)
			{
				CLog::MyLog(LogType::Error, "Console command named %s already exists", a->GetName());
				found = true;
				return;
			}
		}
		this->ConsoleCommands.push_back(Command);
		return;
	}
}
