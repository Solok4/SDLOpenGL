#include "CFontManager.h"

std::unique_ptr<CFontManager> FontManager;

CFontManager::CFontManager()
{
}


CFontManager::~CFontManager()
{
	for (auto font : this->FontMap)
	{
		for (auto FontInstance : font.second)
		{
			TTF_CloseFont(FontInstance.FontHandle);
		}
	}
}

void CFontManager::LoadFont(std::string path, int size)
{
	char OnlyName[128];
	char* LastSlash;
	LastSlash = strrchr((char*)path.c_str(), '/');
#ifdef __EMSCRIPTEN__
	strcpy(OnlyName, LastSlash + 1);
#else
	strcpy_s(OnlyName, LastSlash+1);
#endif

	auto bExists = CFontManager::GetFontByName(OnlyName, size);
	if (bExists == nullptr)
	{
		auto tempFont = TTF_OpenFont(path.c_str(), size);
		if (tempFont != NULL)
		{
			MyFont fontStruct(tempFont, size);
			auto IsLoadedBefore = this->FontMap.find(OnlyName);
			if (IsLoadedBefore != this->FontMap.end())
			{
				IsLoadedBefore->second.push_back(fontStruct);
			}
			else
			{
				this->FontMap.emplace(std::string(OnlyName), std::vector<MyFont>(1,fontStruct));
			}
		}
		else
		{
			CLog::MyLog(LogType::Warning, "Font %s %d loading failed", OnlyName, size);
		}
		return;
	}
	CLog::MyLog(LogType::Warning, "Font %s %d has been loaded already", OnlyName, size);
}

TTF_Font* CFontManager::GetFontByName(std::string name, int size)
{
	for (auto a : this->FontMap)
	{
		if (strcmp(name.c_str(), a.first.c_str()) == 0)
		{
			for (auto Loadedsize : a.second)
			{
				if (Loadedsize.FontSize == size)
				{
					return Loadedsize.FontHandle;
				}
			}
		}
	}
	return nullptr;
}
