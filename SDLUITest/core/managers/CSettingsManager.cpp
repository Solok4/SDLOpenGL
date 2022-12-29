#include "CSettingsManager.h"

std::unique_ptr<CSettingsManager> SettingsManager;

CSettingsManager::CSettingsManager()
{
    this->settings = std::make_shared<globalSettings>();
}

CSettingsManager::~CSettingsManager()
{
}