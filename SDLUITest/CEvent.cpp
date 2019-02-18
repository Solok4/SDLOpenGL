#include "pch.h"
#include "CEvent.h"


CEvent::CEvent()
{
	for (int i = 0; i < 322; i++)
	{
		this->Keys[i] = false;
	}
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
			MouseEvent = this->_Event.button;
		}

		if (this->_Event.type == SDL_MOUSEBUTTONDOWN)
		{
			MouseEvent = this->_Event.button;
		}

		if (this->_Event.type == SDL_MOUSEBUTTONUP)
		{
			MouseEvent = this->_Event.button;
		}

		if (this->_Event.type == SDL_QUIT)
		{
			_IsRunning = false;
		}
			
		switch(this->_Event.type)
		{
		case SDL_KEYDOWN:
			Keys[this->_Event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			Keys[this->_Event.key.keysym.sym] = false;
			break;
		default:
			break;
		}
	}
}

void CEvent::GetMouseMotion(int &Mx, int &My)
{
	Mx = this->_MouseX;
	My = this->_MouseY;

}

bool CEvent::GetIsRunning()
{
	return _IsRunning;
}

SDL_MouseButtonEvent CEvent::GetMouseData()
{
	return MouseEvent;
}

array<bool, 322> CEvent::GetKeyboardData()
{
	return this->Keys;
}

SDL_Event CEvent::GetEvent()
{
	return this->_Event;
}
