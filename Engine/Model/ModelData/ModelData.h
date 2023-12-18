#pragma once

#include "Model/ModelData/MeshData3D/MeshData3D.h"
#include "Model/ModelData/MaterialData3D/MaterialData3D.h"

struct ModelData
{
	MeshData mesh;
	MaterialData material;
	std::string fileName;
	uint32_t textureHundle_;
};