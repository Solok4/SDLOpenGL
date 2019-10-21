#pragma once
class CGameplay
{
public:
	CGameplay();
	CGameplay(const char* name) :Name(name) { this->TimeScale = 1.f; };
	~CGameplay();
	
	void SetTimescale(float timescale);
	float GetTimescale();

	const char* Name;
	float TimeScale;
};

