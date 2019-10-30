#include "pch.h"
#include "CGameplay.h"


CGameplay::CGameplay()
{
}


CGameplay::~CGameplay()
{
}

void CGameplay::SetTimescale(float timescale)
{
	if (timescale < 0)
		timescale = 0;
	this->TimeScale = timescale;
}

float CGameplay::GetTimescale()
{
	return this->TimeScale;
}

void CGameplay::SetMouseLock(bool lock)
{
	this->MouseLock = lock;
}

void CGameplay::ToggleMouseLock()
{
	this->MouseLock = !this->MouseLock;
}

bool CGameplay::GetMouseLock()
{
	return this->MouseLock;
}
