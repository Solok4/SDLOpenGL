#pragma once
#include <array>
#include <SDL.h>


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
	void SetKeyTriggerStatus(unsigned int Key,bool Status);
	std::array<bool, 322> GetKeyButtons();

	std::array<bool, 322> KeyResults;
	std::array<KeyboardButtons, 322> KeyBtns;



};