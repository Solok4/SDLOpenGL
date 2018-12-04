#include "pch.h"
#include "CEvent.h"


CEvent::CEvent()
{
}


CEvent::~CEvent()
{
}


void CEvent::PollEvents()
{
	while (SDL_PollEvent(&this->_Event))
	{
		if (this->_Event.type == SDL_MOUSEMOTION)
		{
			_MouseX = this->_Event.motion.x;
			_MouseY = this->_Event.motion.y;
		}

		if (this->_Event.type == SDL_QUIT)
		{
			_IsRunning = false;
		}

		MouseEvent = this->_Event.button;
	}
}

int CEvent::GetMouseMotion(bool getX)
{
	if (getX)
	{
		return _MouseX;
	}
	else
	{
		return _MouseY;
	}
}

bool CEvent::GetIsRunning()
{
	return _IsRunning;
}

SDL_MouseButtonEvent CEvent::GetMouseData()
{
	return MouseEvent;
}

SDL_Event CEvent::GetEvent()
{
	return this->_Event;
}
