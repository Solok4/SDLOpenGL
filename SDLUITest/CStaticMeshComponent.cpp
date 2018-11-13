#include "pch.h"
#include "CStaticMeshComponent.h"
#include <string>


CStaticMeshComponent::CStaticMeshComponent()
{
}


CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::CreateComponent()
{
	_File.open(FileName, std::ios::in);
	std::string FileNameAsStr(FileName);
	if (_File.good() == false)
	{
		CLog::MyLog(1, "Failed to load from file " + FileNameAsStr);
	}
	else
	{
		size_t dot = FileNameAsStr.find_last_of(".");
		std::string Extension(FileNameAsStr.substr(dot + 1, FileNameAsStr.length()-dot));
		if (Extension == "OBJ")
		{
			CLog::MyLog(0, "This file is an OBJ");
		}
	}
}

void CStaticMeshComponent::FreeComponent()
{
}
