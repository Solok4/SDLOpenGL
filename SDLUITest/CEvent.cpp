#include "pch.h"
#include "CEvent.h"
#include "CRenderer.h"

std::unique_ptr<CEvent> Event;

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
	this->IsEditing = false;
	this->RawKey = 0;
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
			Keys[this->_Event.key.keysym.scancode] = true;
			RawKey = this->_Event.key.keysym.sym;
			break;
		case SDL_KEYUP:
			Keys[this->_Event.key.keysym.scancode] = false;
			break;
		case SDL_TEXTINPUT:
			this->IsEditing = true;
			break;
		case SDL_TEXTEDITING:
			break;
		default:
			break;
		}
		if (this->_Event.type == SDL_WINDOWEVENT)
		{
			switch (this->_Event.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
			{
				CLog::MyLog(LogType::Debug, "Window shown");
				break;
			}
			case SDL_WINDOWEVENT_HIDDEN:
			{
				CLog::MyLog(LogType::Debug, "Window hidden");
				break;
			}
			case SDL_WINDOWEVENT_RESIZED:
			{
				CLog::MyLog(LogType::Debug, "Window resized");
				break;
			}
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			{
				CLog::MyLog(LogType::Debug, "Window size changed");
				Renderer->Resize(this->_Event.window.data1, this->_Event.window.data2);
				break;
			}
			case SDL_WINDOWEVENT_MOVED:
			{
				Renderer->OnWindowMove();
				break;
			}
			default:
				break;
			}
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
