#pragma once

#include "ModelCommon/ModelData/MeshData3D/MeshData3D.h"
#include "ModelCommon/ModelData/MaterialData3D/MaterialData3D.h"

struct ModelData
{
	MeshData mesh;
	MaterialData material;
	std::string fileName;
	uint32_t textureHundle_;
};