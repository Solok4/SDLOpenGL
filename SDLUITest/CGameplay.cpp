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

void CGameplay::AddFunctionToList(std::string name, std::function<void()> fun)
{
	if (this->GetFunctionFromList(name) == nullptr)
	{
		auto temp = std::make_shared<KeyFunction>(name, fun);
		this->KeyFunctionVector.push_back(temp);
		return;
	}
	CLog::MyLog(LogType::Debug, "Keyfunction named %s already exists", name.c_str());
}

std::shared_ptr<KeyFunction> CGameplay::GetFunctionFromList(std::string name)
{
	for (auto a : this->KeyFunctionVector)
	{
		if (strcmp(a->Name.c_str(), name.c_str()) == 0)
		{
			return a;
		}
	}
	return nullptr;
}

void CGameplay::BindKey(int key, std::string functionName)
{
	if (this->GetKeyBoundFunc(key) == nullptr)
	{
		auto function = this->GetFunctionFromList(functionName);
		if (function == nullptr)
		{
			CLog::MyLog(LogType::Debug, "Keyfunction named %s not found", functionName.c_str());
			return;
		}
		auto temp = std::make_shared<KeyMapping>(key, function);
		this->KeyMapVector.push_back(temp);
	}
}

void CGameplay::UnbindKey(int key)
{
	for (int i = 0; i < this->KeyMapVector.size(); i++)
	{
		if (this->KeyMapVector[i]->Key == key)
		{
			this->KeyMapVector.erase(this->KeyMapVector.begin() + i);
			return;
		}
	}
}

std::shared_ptr<KeyMapping> CGameplay::GetKeyBoundFunc(int key)
{
	for (auto a : this->KeyMapVector)
	{
		if (a->Key == key)
		{
			return a;
		}
	}
	return nullptr;
}

void CGameplay::AddKeyTrigger(int key)
{
	if (this->GetKeyTrigger(key) == -1)
	{
		this->KeyTriggerVector.push_back(key);
	}
}

int CGameplay::GetKeyTrigger(int key)
{
	for (auto a : this->KeyTriggerVector)
	{
		if (a == key)
		{
			return a;
		}
	}
	return -1;
}

void CGameplay::RemoveKeyTrigger(int key)
{
	for (int i = 0; i < this->KeyTriggerVector.size(); i++)
	{
		if (this->KeyTriggerVector[i] == key)
		{
			this->KeyTriggerVector.erase(this->KeyTriggerVector.begin() + i);
			return;
		}
	}
}

void CGameplay::Init()
{
	this->AddFunctionToList("MouseLock", [this]() 
		{
			this->MouseLock = !this->MouseLock;
		});

	this->BindKey(SDL_SCANCODE_1, "MouseLock");

	this->AddFunctionToList("Console", [this]()
		{
			if (strcmp(LayoutManager->GetCurrentLayout()->GetName(), "Console") == 0)
			{
				LayoutManager->PopActiveLayout();
			}
			else
			{
				LayoutManager->PushActiveLayout("Console");
			}
		});
	this->BindKey(SDL_SCANCODE_GRAVE,"Console");

	this->AddKeyTrigger(SDL_SCANCODE_1);
	this->AddKeyTrigger(SDL_SCANCODE_2);
	this->AddKeyTrigger(SDL_SCANCODE_F3);
	this->AddKeyTrigger(SDL_SCANCODE_GRAVE);
}

void CGameplay::OnGameplayChange()
{
	for (auto a : this->KeyTriggerVector)
	{
		KeyboardConf->SetKeyTriggerStatus(a, true);
	}

	//KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_1, true);
	//KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_2, true);
	//KeyboardConf->SetKeyTriggerStatus(SDL_SCANCODE_F3, true);
}

void CGameplay::Tick(double delta)
{
	this->KeyEvents(KeyboardConf->GetKeyButtons());
}

void CGameplay::KeyEvents(std::array<bool, 322> keys)
{
	if (!LayoutManager->GetCurrentLayout()->GetIsEditingLayout())
	{
		for (auto a : this->KeyMapVector)
		{
			if (keys[a->Key])
			{
				a->Function->Function();
			}
		}
		/*if (keys[SDL_SCANCODE_1])
		{
			this->ToggleMouseLock();
		}*/
		if (keys[SDL_SCANCODE_2])
		{
			if (this->TimeScale == 0.0f)
			{
				this->TimeScale = 1.0f;
			}
			else
			{
				this->TimeScale = 0.0f;
			}
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

