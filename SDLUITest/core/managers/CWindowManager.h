#pragma once

#include <memory>
#include <chrono>
#include "../../CLog.h"
#include "../shared/Shared.h"
#include "./CSettingsManager.h"

class CWindowManager
{
public:
	CWindowManager();
	~CWindowManager();

	// Initalizes window
	void Init(std::shared_ptr<WindowInfo> windowInfo);
	// Destroys Window
	void Destroy();
	// Change size of the window
	void Resize(int w, int h);
	void ResizeDemand();
	// Called on window move event
	void OnWindowMove();
	// Returns SDL_Window
	SDL_Window *GetWindow();
	// Returns window information struct
	std::shared_ptr<WindowInfo> GetWindowInfo() { return this->WInfo; };
	std::shared_ptr<WindowInfo> GetDemandWindowInfo() { return this->DemandWinfo; };

	void SetFrameLock(int frames);

private:
	SDL_Window *Window;
	std::shared_ptr<WindowInfo> WInfo;
	std::shared_ptr<WindowInfo> DemandWinfo;
};

extern std::unique_ptr<CWindowManager> WindowManager;
