#include "ModelDataManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>
#include <fstream>
#include <sstream>
#include "TextureManager/TextureManager.h"

ModelDataManager* ModelDataManager::GetInstance()
{
	static ModelDataManager instance;
	return &instance;
}

void ModelDataManager::Finalize()
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {
		modelDatas_[modelNum].mesh.vertexResource_->Release();
	}
}

uint32_t ModelDataManager::LoadObj(const std::string& fileName)
{
	for (uint32_t modelNum = 0; modelNum < static_cast<uint32_t>(modelDatas_.size()); modelNum++) {

		if (modelDatas_[modelNum].fileName == fileName) {
			return modelNum;
		}
	}

	LoadObjeFile(directoryPath_, fileName);

	return static_cast<uint32_t>(modelDatas_.size()) - 1;
}

MaterialData ModelDataManager::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	MaterialData materialData_; // 構築するMaterialData
	std::string line; // ファイルから読んだ１行を格納するもの

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName); // ファイルを開く
	assert(file.is_open());

	// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFileName;
			s >> textureFileName;
			// 連結してファイルパスにする
			materialData_.textureFilePath = directoryPath + "/" + textureFileName;
		}
	}

	// 4. MaterialDataを返す
	return materialData_;
}

void ModelDataManager::LoadObjeFile(const std::string& directoryPath, const std::string& fileName)
{
	// 1. 中で必要となる変数の宣言
	modelDatas_.push_back(ModelData());; // 構築するModelData
	std::vector<Vector4> positions; // 位置
	std::vector<Vector3> normals; // 法線
	std::vector<Vector2> texcoords; // テクスチャ座標
	std::string line; // ファイルから読んだ1行を格納するもの

	modelDatas_.back().fileName = fileName;

	// 2. ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName + "/" + fileName + ".obj"); // ファイルを開く
	assert(file.is_open()); // とりあえず開けなかったら止める

	// 3. 実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier; // 先頭の識別子を読む

		// identifierに応じた処理

		if (identifier == "v") {
			Vector4 vertexPos = {};
			s >> vertexPos.x >> vertexPos.y >> vertexPos.z;
			vertexPos.x = -1.0f * vertexPos.x;
			vertexPos.w = 1.0f;
			positions.push_back(vertexPos);
		}
		else if (identifier == "vt") {
			Vector2 texcoord = {};
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal = {};
			s >> normal.x >> normal.y >> normal.z;
			normal.x = -1.0f * normal.x;
			normals.push_back(normal);
		}
		else if (identifier == "f") {

			VertexData triangle[3] = {};
			// 面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3] = {};
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // /区切りでIndexを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 vertexPos = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				triangle[faceVertex] = { vertexPos,texcoord,normal };

			}

			modelDatas_.back().mesh.verteces.push_back(triangle[2]);
			modelDatas_.back().mesh.verteces.push_back(triangle[1]);
			modelDatas_.back().mesh.verteces.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			// materialTemplateLibraryファイルの名前を取得する
			std::string materialFileName;
			s >> materialFileName;
			// 基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelDatas_.back().material = LoadMaterialTemplateFile(directoryPath + "/" + fileName + "/", materialFileName);
		}
	}

	modelDatas_.back().textureHundle_ = TextureManager::GetInstance()->LoadTexture(modelDatas_.back().material.textureFilePath);

	modelDatas_.back().mesh.vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * modelDatas_.back().mesh.verteces.size());


	//VertexBufferViewを作成する
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	modelDatas_.back().mesh.vertexBufferView_.BufferLocation = modelDatas_.back().mesh.vertexResource_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点3つ分のサイズ
	modelDatas_.back().mesh.vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelDatas_.back().mesh.verteces.size());
	//頂点当たりのサイズ
	modelDatas_.back().mesh.vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//Resourceにデータを書き込む
	//頂点リソースにデータを書き込む
	//書き込むためのアドレスを取得
	modelDatas_.back().mesh.vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&modelDatas_.back().mesh.vertexData_));
	std::memcpy(modelDatas_.back().mesh.vertexData_, modelDatas_.back().mesh.verteces.data(), sizeof(VertexData) * modelDatas_.back().mesh.verteces.size());
}
