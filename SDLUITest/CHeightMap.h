#pragma once
#include <vector>
#include "core/shared/Shared.h"
#include "glm/glm.hpp"
#include "core/managers/CMaterialManager.h"

#define HEIGHT_MAP_MAX_SIZE 1024

class CHeightMap
{
public:
	CHeightMap() = delete;
	CHeightMap(int size, float distance, int seed);
	~CHeightMap();
	void CreateMesh();
	void Draw();

	void SetPosition(glm::vec3 pos);
	//Sets rotation of the component. If the component is a child of another one the rotation will be relative to parent component.
	void SetRotation(glm::vec3 rot);
	//Sets scale of the component. If the component is a child of another one the scale will be relative to parent component.
	void SetScale(glm::vec3 scale);
	//Returns vec3 position of the component.
	glm::vec3 GetPosition();
	//Returns vec3 rotation of the component.
	glm::vec3 GetRotation();
	//Returns vec3 scale of the component.
	glm::vec3 GetScale();
	glm::mat4 GetModelMatrix();

private:
	void CalculateMatrix();
	
	int _Size;
	float _Distance;
	int _Seed;
	std::vector<std::vector<float>> _HeightMap;
	GLuint _HeightMapVAO;
	int _VertexCount;

	glm::mat4 _ModelMatrix;
	glm::vec3 _Position;
	glm::vec3 _Rotation;
	glm::vec3 _Scale;
	std::shared_ptr<Material> _HeightmapMaterial;
};
