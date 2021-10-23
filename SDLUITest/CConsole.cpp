#include "CConsole.h"
#include "CLog.h"
#include <string>
#include "core/shared/Shared.h"

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
			if (StdLibWrapper::Sstrcmpi(a->GetName(), name.c_str()) == 0)
			{
				return a;
			}
		}
	}
	CLog::error("Not found command: %s", name.c_str());
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
			if (StdLibWrapper::Sstrcmpi(Command->GetName(), a->GetName()) == 0)
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

void CConsole::ParseCommand(std::string command)
{
	std::vector<std::string> splittedBySpace = {};
	int beginIndex = 0;
	int currentIndex = 0;
	while((currentIndex = command.substr(beginIndex,command.length()).find(' ')) != std::string::npos) {
		splittedBySpace.push_back(command.substr(beginIndex, currentIndex));
		beginIndex = currentIndex+1;
	}
	splittedBySpace.push_back(command.substr(beginIndex, command.length()));

	if (splittedBySpace.size() == 0 || splittedBySpace[0] == "") {
		return;
	}

	auto cvar = this->GetCommandByName(splittedBySpace[0]);
	if (splittedBySpace.size() >= 2) {
		if (cvar->GetDataType() == CvarType::CVAR_INTEGER) {
			int intValue = 0;
			try {
				intValue = std::stoi(splittedBySpace[1]);
			}
			catch (std::exception) {
				CLog::error("Invalid integer value for command: %s", splittedBySpace[0].c_str());
				return;
			}
			cvar->SetCurrentValue(intValue);
		}
		else if (cvar->GetDataType() == CvarType::CVAR_DOUBLE) {
			double doubleValue = 0;
			try {
				doubleValue = std::stod(splittedBySpace[1]);
			}
			catch (std::exception) {
				CLog::error("Invalid double value for command: %s", splittedBySpace[0].c_str());
				return;
			}
			cvar->SetCurrentValue(doubleValue);
		}
		else if (cvar->GetDataType() == CvarType::CVAR_STRING) {
			cvar->SetCurrentValue(splittedBySpace[1]);
		}
		CLog::info("Value of %s changed to %s",splittedBySpace[0].c_str(),splittedBySpace[1].c_str());
	}
}
