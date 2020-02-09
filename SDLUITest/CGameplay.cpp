#include "pch.h"
#include "CGameplay.h"
#include "CEvent.h"
#include "CSceneManager.h"
#include "CLayoutManager.h"
#include "CKeyboardConf.h"
#include "CRenderer.h"


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

void CGameplay::Tick(double delta)
{
	this->KeyEvents(KeyboardConf->GetKeyButtons());

}

void CGameplay::KeyEvents(std::array<bool, 322> keys)
{
	if (!LayoutManager->GetCurrentLayout()->GetIsEditingLayout())
	{
		if (keys[SDL_SCANCODE_1])
		{
			this->ToggleMouseLock();
		}
		if (keys[SDL_SCANCODE_T])
		{
			LayoutManager->ChangeCurrentLayout("Default");
		}
		if (keys[SDL_SCANCODE_Y])
		{
			LayoutManager->ChangeCurrentLayout("Blank");
		}
		if (keys[SDL_SCANCODE_F3])
		{
			auto renderingMethod = OpenGL->GetRenderMode();
			if (renderingMethod == RenderMode::RenderModeForward)
			{
				OpenGL->SetRenderMode(RenderMode::RenderModeDeferred);
			}
			else
			{
				OpenGL->SetRenderMode(RenderMode::RenderModeForward);
			}
		}

		if (keys[SDL_SCANCODE_W])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveForward(false);
		}
		if (keys[SDL_SCANCODE_S])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveBackwards(false);
		}
		if (keys[SDL_SCANCODE_A])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveLeft(false);
		}
		if (keys[SDL_SCANCODE_D])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveRight(false);
		}
		if (keys[SDL_SCANCODE_SPACE])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveUp(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveUp(false);
		}
		if (keys[SDL_SCANCODE_LSHIFT])
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveDown(true);
		}
		else
		{
			SceneManager->GetCurrentScene()->GetMovementObject()->MoveDown(false);
		}
	}
}

