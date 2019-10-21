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
