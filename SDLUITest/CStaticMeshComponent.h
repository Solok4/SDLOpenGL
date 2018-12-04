#pragma once
#include "CBaseComponent.h"
#include <fstream>
#include <vector>
class CStaticMeshComponent :
	public CBaseComponent
{
public:
	CStaticMeshComponent();
	~CStaticMeshComponent();

	void CreateComponent() override;
	void FreeComponent() override;


private:

	fstream _File;
	const char* FileName;
	
	vector<vec3> _Vertexes;
	vector<vec2> _Texels;
	vector<vec3> _Normals;

};

