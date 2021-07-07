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
#ifndef __EMSCRIPTEN__
			if (_strcmpi(a->GetName(), name.c_str()) == 0)
#else
			if (strcmp(a->GetName(), name.c_str()) == 0)
#endif
			{
				return a;
			}
		}
	}
	return {};
}

void CConsole::ChangeValueOfGivenCommand(std::string name, int value)
{
	auto command = CConsole::GetCommandByName(name);
	if (command != nullptr)
	{
		if (command->GetDataType() == CvarType::CVAR_INTEGER)
		{
			command->SetCurrentValue(value);
			return;
		}
	}
	CLog::error("Console command named %s not found", name.c_str());
}

void CConsole::ChangeValueOfGivenCommand(std::string name, double value)
{
	auto command = CConsole::GetCommandByName(name);
	if (command != nullptr)
	{
		if (command->GetDataType() == CvarType::CVAR_DOUBLE)
		{
			command->SetCurrentValue(value);
			return;
		}
	}
	CLog::error("Console command named %s not found", name.c_str());
}

void CConsole::ChangeValueOfGivenCommand(std::string name, std::string value)
{
	auto command = CConsole::GetCommandByName(name);
	if (command != nullptr)
	{
		if (command->GetDataType() == CvarType::CVAR_STRING)
		{
			command->SetCurrentValue(value);
			return;
		}
	}
	CLog::error("Console command named %s not found", name.c_str());
}

void CConsole::AddCommand(std::shared_ptr<CVar> Command)
{
	if (Command != nullptr)
	{
		bool found = false;
		for (auto a : this->ConsoleCommands)
		{
#ifndef __EMSCRIPTEN__
			if (_strcmpi(Command->GetName(), a->GetName()) == 0)
#else
			if (strcmp(Command->GetName(), a->GetName()) == 0)
#endif
			{
				CLog::error("Console command named %s already exists", a->GetName());
				found = true;
				return;
			}
		}
		this->ConsoleCommands.push_back(Command);
		return;
	}
}