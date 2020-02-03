#include "CMaterialManager.h"

std::unique_ptr<CMaterialManager> MaterialManager;

void Material::AddTextureToMaterial(std::string name, TextureTypes type)
{
	auto tex = TextureManager->GetTextureByName(name);
	if (!tex)
	{
		CLog::MyLog(LogType::Warning, "Texture named %s not found.", name);
		return;
	}
	this->Textures.push_back(std::make_shared<MaterialTexHandle>(tex, type));
}

GLuint Material::GetTextureByType(TextureTypes type)
{
	for (auto a : this->Textures)
	{
		if (a->Type == type)
		{
			return a->TexHandle;
		}
	}
	return -1;
}




CMaterialManager::CMaterialManager()
{
}


CMaterialManager::~CMaterialManager()
{
}

std::shared_ptr<Material> CMaterialManager::CreateNewMaterial(std::string name)
{
	auto Mat = std::make_shared<Material>(name);
	this->MaterialVector.push_back(Mat);
	return Mat;
}

std::shared_ptr<Material> CMaterialManager::GetMaterialByName(std::string name)
{
	for (auto o : this->MaterialVector)
	{
		if (strcmp(name.c_str(), o->GetName().c_str()) == 0)
		{
			return o;
		}
	}
	return nullptr;
}

