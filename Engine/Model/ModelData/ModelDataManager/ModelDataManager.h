#pragma once

#include "Model/ModelData/ModelData.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelDataManager 
{
public:

	static ModelDataManager* GetInstance();

	void Finalize();

	//uint32_t LoadObj(const std::string& fileName);
	// 例)
	// Cubeフォルダ
	//   Cube.obj
	//   Cube.mtl
	//   hoge.png
	// の状態

	/// <summary>
	/// objファイルの読み込み
	/// </summary>
	/// <param name="fileName">.objが入っているフォルダの名前( "Cube" 等)</param>
	/// <returns>モデルデータのハンドル</returns>
	uint32_t LoadObj(const std::string& fileName);

	/// <summary>
	/// glTFファイルの読み込み
	/// </summary>
	/// <param name="fileName">.glTFが入っているフォルダの名前( "Cube" 等)</param>
	/// <returns>モデルデータのハンドル</returns>
	uint32_t LoadGLTF(const std::string& fileName);

	/// <summary>
	/// テクスチャーハンドルの取得
	/// </summary>
	/// <param name="hundle">モデルハンドル( 0 等)</param>
	/// <returns>テクスチャーハンドル</returns>
	const uint32_t& GetTextureHundle(uint32_t hundle) { return modelDatas_[hundle].textureHundle_; }

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	/// <param name="hundle">モデルハンドル( 0 等)</param>
	/// <returns>モデルデータ</returns>
	const ModelData* GetModelData(uint32_t hundle) { return &modelDatas_[hundle]; }

private:
	ModelDataManager() = default;
	~ModelDataManager() = default;
	ModelDataManager(const ModelDataManager&) = delete;
	ModelDataManager& operator=(const ModelDataManager&) = delete;

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

	void LoadObjFile(const std::string& directoryPath, const std::string& fileName);

	NodeData ReadNode(aiNode* node);

	void LoadGLTFFile(const std::string& directoryPath, const std::string& fileName);

private:

	std::vector<ModelData> modelDatas_;

	const std::string directoryPath_ = "Resources";

};