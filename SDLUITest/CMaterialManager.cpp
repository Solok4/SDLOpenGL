#include "CMaterialManager.h"

std::unique_ptr<CMaterialManager> MaterialManager;

void Material::AddTextureToMaterial(std::string name, TextureTypes type)
{
	auto tex = TextureManager->GetTextureByName(name);
	if (!tex)
	{
		CLog::MyLog(LogType::Warning, "Texture named %s not found.", name.c_str());
		return;
	}
	auto exists = this->GetTextureByType(type);
	if (exists != -1)
	{
		CLog::MyLog(LogType::Warning, "There is already a texture of this type. Material: %s", this->Name.c_str());
		return;
	}
	switch (type)
	{
	case TextureTypes::BaseTex: {
		this->Data->DiffuseTex = tex;
		break;
	}
	case TextureTypes::SpecularMap: {
		this->Data->SpecularTex = tex;
		break;
	}
	case TextureTypes::NormalMap: {
		this->Data->NormalTex = tex;
		break;
	}
	}
}

GLuint Material::GetTextureByType(TextureTypes type)
{
	switch (type)
	{
	case TextureTypes::BaseTex: {
		return this->Data->DiffuseTex;
	}
	case TextureTypes::SpecularMap: {
		return this->Data->SpecularTex;
	}
	case TextureTypes::NormalMap: {
		return this->Data->NormalTex;
	}
	default: {
		return 0;
	}
	}
}

std::shared_ptr<MaterialData> Material::GetData()
{
	return this->Data;
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

std::shared_ptr<Material> CMaterialManager::CreateNewMaterialFromFile(std::string path)
{
	return std::shared_ptr<Material>();
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

