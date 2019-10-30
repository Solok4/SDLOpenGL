#pragma once
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

	const char* Name;
	float TimeScale;
	bool MouseLock = false;
};

