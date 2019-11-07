#pragma once

#include <stdint.h>
#include <array>

class CGameplay
{
public:
	CGameplay();
	CGameplay(const char* name) :Name(name) { this->TimeScale = 1.f; };
	~CGameplay();
	
	void SetTimescale(float timescale);
	float GetTimescale();
	void SetMouseLock(bool lock);
	void ToggleMouseLock();
	bool GetMouseLock();
	virtual void Tick(double delta);
	virtual void KeyEvents(std::array<bool, 322> keys);

	const char* Name;
	float TimeScale;
	bool MouseLock = false;
};

