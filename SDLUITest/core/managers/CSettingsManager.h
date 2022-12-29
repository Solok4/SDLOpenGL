#pragma once

#include <memory>
#include <chrono>

struct inputSettings
{
    inputSettings()
    {
        this->mouseSensitivity = 5;
    }

    double mouseSensitivity;
};

struct WindowInfo
{
	WindowInfo() : ScreenWidth(0), ScreenHeight(0), ScreenPosX(0), ScreenPosY(0), ScreenAspectRatio(0.0f), WindowFlags(0),
				   Delta(0), BeginingOfTheFrame(std::chrono::system_clock::now()), EndOfTheFrame(std::chrono::system_clock::now()), FPSLock(30){};
	int ScreenWidth;
	int ScreenHeight;
	int ScreenPosX;
	int ScreenPosY;
	float ScreenAspectRatio;
	uint32_t WindowFlags;

	std::chrono::duration<double> Delta;
	std::chrono::time_point<std::chrono::system_clock> BeginingOfTheFrame;
	std::chrono::time_point<std::chrono::system_clock> EndOfTheFrame;

	uint32_t FPSLock;
};

struct globalSettings
{
    globalSettings()
    {
        this->input = std::make_shared<inputSettings>();
        this->window = std::make_shared<WindowInfo>();
    };

    std::shared_ptr<inputSettings> input;
    std::shared_ptr<WindowInfo> window;
};

class CSettingsManager
{
public:
    CSettingsManager();
    ~CSettingsManager();

    std::shared_ptr<globalSettings> getSettings() { return this->settings; };

private:
    std::shared_ptr<globalSettings> settings;
};

extern std::unique_ptr<CSettingsManager> SettingsManager;