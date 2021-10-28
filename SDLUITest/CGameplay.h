#pragma once

#include <stdint.h>
#include <array>
#include <functional>
#include <vector>
#include <string>

#include "core/managers/CWindowManager.h"

struct KeyFunction
{
	KeyFunction(std::string name, std::function<void()> fun) :Name(name), Function(fun) {};
	std::string Name;
	std::function<void()> Function;
};

struct KeyMapping
{
	KeyMapping(int key, std::shared_ptr<KeyFunction> fun) :Key(key), Function(fun) {};
	int Key;
	std::shared_ptr<KeyFunction> Function;
};

class CGameplay
{
public:
	CGameplay();
	CGameplay(const char* name) :Name(std::string(name)) { this->TimeScale = 1.f; };
	~CGameplay();

	void SetTimescale(float timescale);
	float GetTimescale();
	void SetMouseLock(bool lock);
	void ToggleMouseLock();
	bool GetMouseLock();
	void AddFunctionToList(std::string name, std::function<void()> fun);
	std::shared_ptr<KeyFunction> GetFunctionFromList(std::string name);
	void BindKey(int key, std::string functionName);
	void UnbindKey(int key);
	std::shared_ptr<KeyMapping> GetKeyBoundFunc(int key);

	void AddKeyTrigger(int key);
	int GetKeyTrigger(int key);
	void RemoveKeyTrigger(int key);

	virtual void Init();
	virtual void OnGameplayChange();
	virtual void Tick(double delta);
	virtual void KeyEvents(std::array<bool, 322> keys);
	void SetFrameLimit(int frames)
	{
		this->FrameLimit = frames;
		WindowManager->SetFrameLock(frames);
	};
	int GetFrameLimit() { return this->FrameLimit; };

	std::string Name;
	float TimeScale;
	bool MouseLock = false;
	int FrameLimit;
	std::vector<std::shared_ptr<KeyFunction>> KeyFunctionVector;
	std::vector<std::shared_ptr<KeyMapping>> KeyMapVector;
	std::vector<int> KeyTriggerVector;
};