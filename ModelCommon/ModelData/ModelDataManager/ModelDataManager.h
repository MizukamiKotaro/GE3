#pragma once

#include "ModelCommon/ModelData/ModelData.h"
#include <vector>

class ModelDataManager 
{
public:

	static ModelDataManager* GetInstance();

	void Finalize();

	uint32_t LoadObj(const std::string& fileName);

	const uint32_t& GetTextureHundle(uint32_t hundle) { return modelDatas_[hundle].textureHundle_; }

	const ModelData* GetModelData(uint32_t hundle) { return &modelDatas_[hundle]; }

private:
	ModelDataManager() = default;
	~ModelDataManager() = default;
	ModelDataManager(const ModelDataManager&) = delete;
	ModelDataManager& operator=(const ModelDataManager&) = delete;


	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

	void LoadObjeFile(const std::string& directoryPath, const std::string& fileName);

private:

	std::vector<ModelData> modelDatas_;

	const std::string directoryPath_ = "Resources";

};