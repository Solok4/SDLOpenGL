#pragma once
#include <array>
#include <memory>
#include "../shared/Shared.h"

struct KeyboardButtons
{
	bool IsPressed;
	bool IsTriggerType;
	bool LastStatus;
};

class CKeyboardConf
{
public:
	CKeyboardConf();
	~CKeyboardConf();

	void ProcessButtons(std::array<bool, 322> ButArray);
	void SetKeyToTriggerType(unsigned int Key, bool Status);
	void ResetKeyFromTriggerStatus();
	std::array<bool, 322> GetKeyButtons();

	std::array<bool, 322> KeyResults;
	std::array<KeyboardButtons, 322> KeyBtns;
};

extern std::unique_ptr<CKeyboardConf> KeyboardConf;
