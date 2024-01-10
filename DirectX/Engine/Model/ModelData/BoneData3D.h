#pragma once

#include <vector>
#include <string>
#include "Matrix4x4.h"

struct VertexWeight {
	uint32_t vertexID;
	float weight;
};

struct BoneData
{
	std::string name;
	Matrix4x4 offsetMatrix;
	std::vector<VertexWeight> vertexWeights;
};