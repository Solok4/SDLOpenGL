#include "CMaterialManager.h"
#include "../shared/TextureBaseData.h"
#include "../shared/Shared.h"

std::unique_ptr<CMaterialManager> MaterialManager;

Material::Material(std::string name)
{
	this->Name = name;
	this->Data = std::make_shared<MaterialData>();
	this->Data->AmbientColor = glm::vec3(1.0f);
	this->Data->DiffuseColor = glm::vec3(1.0f);
	this->Data->SpecularColor = glm::vec3(1.0f);
	this->Data->Shininess = 32.0f;
	this->Data->IluminationModel = 2;
	this->Data->RefractionIndex = 1.0f;
	this->Data->Transparency = 1.0f;
}

void Material::AddTextureToMaterial(std::string name, TextureTypes type)
{
	auto tex = TextureManager->GetTextureByName(name);
	if (!tex)
	{
		CLog::error("Texture named %s not found.", name.c_str());
		return;
	}
	auto exists = this->GetTextureByType(type);
	/*if (exists != -1)
	{
		CLog::MyLog(LogType::Warning, "There is already a texture of this type. Material: %s", this->Name.c_str());
		return;
	}*/
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
	auto material = std::make_shared<Material>("DefaultMaterial");

	GLuint diffuseTex;
	glGenTextures(1, &diffuseTex);
	glBindTexture(GL_TEXTURE_2D, diffuseTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, DiffuseTexBase);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint specularTex;
	glGenTextures(1, &specularTex);
	glBindTexture(GL_TEXTURE_2D, specularTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, SpecularTexBase);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	GLuint normalTex;
	glGenTextures(1, &normalTex);
	glBindTexture(GL_TEXTURE_2D, normalTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, NormalTexBase);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	material->GetData()->DiffuseTex = diffuseTex;
	material->GetData()->SpecularTex = specularTex;
	material->GetData()->NormalTex = normalTex;
	this->DefaultMaterial = material;
}

CMaterialManager::~CMaterialManager()
{
}

std::shared_ptr<Material> CMaterialManager::CreateNewMaterial(std::string name)
{
	auto Mat = std::make_shared<Material>(name);
	auto defaultMaterialData = this->DefaultMaterial->GetData();
	auto newMatData = Mat->GetData();
	newMatData->DiffuseTex = defaultMaterialData->DiffuseTex;
	newMatData->SpecularTex = defaultMaterialData->SpecularTex;
	newMatData->NormalTex = defaultMaterialData->NormalTex;
	this->MaterialVector.push_back(Mat);
	return Mat;
}

std::vector<std::shared_ptr<Material>> CMaterialManager::CreateNewMaterialFromFile(std::string path)
{
	const char* name(path.c_str());
	const char* slash = strrchr(name, '.');
	const char* justExtension(slash + 1);
	if (StdLibWrapper::Sstrcmp(justExtension, "mtl") != 0)
	{
		CLog::error("Failed to load a material from %s. Wrong file extension.", path.c_str());
		return std::vector<std::shared_ptr<Material>>();
	}

	FILE* file = StdLibWrapper::Sfopen(path.c_str(), "r");
	if (file == NULL)
	{
		CLog::error("Failed to load a material from %s. No such file.", path.c_str());
		return std::vector<std::shared_ptr<Material>>();
	}
	std::vector<std::shared_ptr<Material>> materialsToReturn;
	std::shared_ptr<MaterialData> currentMaterialData = nullptr;
	while (1)
	{
		char LineHeader[128];
		int res = StdLibWrapper::Sfscanf(file, sizeof(LineHeader) / sizeof(LineHeader[0]), "%s", LineHeader);
		if (res == EOF)
			break;
		if (StdLibWrapper::Sstrcmp(LineHeader, "newmtl") == 0)
		{
			char name[128];
			StdLibWrapper::Sfscanf(file, sizeof(name) / sizeof(name[0]), "%s", &name);

			auto newMaterial = std::make_shared<Material>(name);
			this->MaterialVector.push_back(newMaterial);
			materialsToReturn.push_back(newMaterial);
			currentMaterialData = newMaterial->GetData();
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "Ns") == 0)
		{
			float Shininess;
			StdLibWrapper::Sfscanf(file, sizeof(float), "%f", &Shininess);

			currentMaterialData->Shininess = Shininess;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "Ka") == 0)
		{
			glm::vec3 ambientColor;
			StdLibWrapper::Sfscanf(file,sizeof(glm::vec3), "%f %f %f", &ambientColor.x, &ambientColor.y, &ambientColor.z);

			currentMaterialData->AmbientColor = ambientColor;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "Kd") == 0)
		{
			glm::vec3 diffuseColour;
			StdLibWrapper::Sfscanf(file, sizeof(glm::vec3), "%f %f %f", &diffuseColour.x, &diffuseColour.y, &diffuseColour.z);

			currentMaterialData->DiffuseColor = diffuseColour;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "Ks") == 0)
		{
			glm::vec3 specularColor;
			StdLibWrapper::Sfscanf(file, sizeof(glm::vec3), "%f %f %f", &specularColor.x, &specularColor.y, &specularColor.z);

			currentMaterialData->SpecularColor = specularColor;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "Ni") == 0)
		{
			float opticalDensity;
			StdLibWrapper::Sfscanf(file,  sizeof(float), "%f", &opticalDensity);

			currentMaterialData->RefractionIndex = opticalDensity;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "d") == 0)
		{
			float transparency;
			StdLibWrapper::Sfscanf(file,  sizeof(float), "%f", &transparency);

			currentMaterialData->Transparency = transparency;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "illum") == 0)
		{
			int iluminationModel;
			StdLibWrapper::Sfscanf(file, sizeof(int), "%d", &iluminationModel);

			currentMaterialData->IluminationModel = iluminationModel;
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "map_Ka") == 0)
		{
			char ambientMapPath[128];
			StdLibWrapper::Sfscanf(file, sizeof(ambientMapPath) / sizeof(ambientMapPath[0]), "%s", &ambientMapPath);

			const char* name(path.c_str());
			const char* slash = strrchr(name, '/');
			std::string basePath(name, slash + 1);
			auto ambientTexture = TextureManager->LoadTexture(basePath + ambientMapPath);
			if (ambientTexture != nullptr) {
				currentMaterialData->DiffuseTex = ambientTexture->texHandle;
			}
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "map_Kd") == 0)
		{
			char diffuseMapPath[128];
			StdLibWrapper::Sfscanf(file, sizeof(diffuseMapPath) / sizeof(diffuseMapPath[0]), "%s", &diffuseMapPath);

			const char* name(path.c_str());
			const char* slash = strrchr(name, '/');
			std::string basePath(name, slash + 1);
			auto diffuseTexture = TextureManager->LoadTexture(basePath + diffuseMapPath);
			if (diffuseTexture != nullptr) {
				currentMaterialData->DiffuseTex = diffuseTexture->texHandle;
			}
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "map_Ks") == 0)
		{
			char specularMapPath[128];
			StdLibWrapper::Sfscanf(file,  sizeof(specularMapPath) / sizeof(specularMapPath[0]), "%s", &specularMapPath);

			const char* name(path.c_str());
			const char* slash = strrchr(name, '/');
			std::string basePath(name, slash + 1);
			auto specluarTexture = TextureManager->LoadTexture(basePath + specularMapPath);
			if (specluarTexture != nullptr) {
				currentMaterialData->SpecularTex = specluarTexture->texHandle;
			}
		}
		else if (StdLibWrapper::Sstrcmp(LineHeader, "map_bump") == 0 || strcmp(LineHeader, "bump") == 0)
		{
			char normalMapPath[128];
			StdLibWrapper::Sfscanf(file, sizeof(normalMapPath) / sizeof(normalMapPath[0]), "%s", &normalMapPath);

			const char* name(path.c_str());
			const char* slash = strrchr(name, '/');
			std::string basePath(name, slash + 1);
			auto normalTexture = TextureManager->LoadTexture(basePath + normalMapPath);
			if (normalTexture != nullptr) {
				currentMaterialData->NormalTex = normalTexture->texHandle;
			}
		}
	}
	fclose(file);
	return materialsToReturn;
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

std::shared_ptr<Material> CMaterialManager::GetDefaultMaterial()
{
	return this->DefaultMaterial;
}