#include "BlockManager.h"
#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "Model/ModelData/ModelDataManager/ModelDataManager.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "Utils/RandomGenerator/RandomGenerator.h"
#include "Camera.h"
#include <numbers>
#include <algorithm>
#include "TextureManager.h"

Blocks::Blocks(const std::string& fileName)
{

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj(fileName);

	textureHundle_ = modelManager->GetTextureHundle(meshHundle_);

	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(ParticleForGPU) * kNumInstance);
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].WVP = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].World = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].color = { 1.0f,1.0f,1.0f,0.0f };
	}

	CreateSRV();

	//平行光源用のリソースを作る。
	directionalLightResource_ = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

Blocks::Blocks(uint32_t modelHundle)
{
	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelHundle;

	textureHundle_ = modelManager->GetTextureHundle(meshHundle_);

	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(ParticleForGPU) * kNumInstance);
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].WVP = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].World = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].color = { 1.0f,1.0f,1.0f,0.0f };
	}

	CreateSRV();

	//平行光源用のリソースを作る。
	directionalLightResource_ = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
	//データを書き込む
	directionalLightData_ = nullptr;
	//書き込むためのアドレスを取得
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	//書き込んでいく
	directionalLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_->direction = { 0.0f,-1.0f,0.0f };
	directionalLightData_->intensity = 1.0f;
}

Blocks::~Blocks()
{
	instancingResource_->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
}

void Blocks::Draw(const Camera& camera, std::list<IBlock>& blocks, BlendMode blendMode)
{
	uint32_t instaceNum = static_cast<uint32_t>(blocks.size());
	instaceNum = std::clamp<uint32_t>(instaceNum, 0, kNumInstance);
	uint32_t index = 0;

	for (std::list<IBlock>::iterator iter = blocks.begin(); iter != blocks.end(); iter++) {
		if (index >= kNumInstance) {
			break;
		}

		instancingData_[index].World = iter->transformMat_;
		instancingData_[index].WVP = instancingData_[index].World * camera.GetViewProjection();
		instancingData_[index].color = iter->color_;

		index++;
	}

	//materialData_->uvTransform = uvMatrix_;

	TextureManager* texManager = TextureManager::GetInstance();

	const ModelData* modelData = ModelDataManager::GetInstance()->GetModelData(meshHundle_);

	GraphicsPiplineManager::GetInstance()->SetBlendMode(piplineType, static_cast<uint32_t>(blendMode));

	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//Spriteの描画。変更に必要なものだけ変更する
	commandList->IASetVertexBuffers(0, 1, &modelData->mesh.vertexBufferView_); // VBVを設定
	//マテリアルCBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//TransformationMatrixCBufferの場所を設定
	//commandList->SetGraphicsRootConstantBufferView(1, instancingResource_->GetGPUVirtualAddress());
	commandList->SetGraphicsRootDescriptorTable(1, srvGPUDescriptorHandle_);
	//平行光源CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, directionalLightResource_->GetGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(UINT(modelData->mesh.verteces.size()), instaceNum, 0, 0);

}

void Blocks::SetMesh(uint32_t mesh)
{
	meshHundle_ = mesh;
	textureHundle_ = ModelDataManager::GetInstance()->GetTextureHundle(meshHundle_);
}

void Blocks::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kNumInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	srvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVCPUDescriptorHandle();
	srvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVGPUDescriptorHandle();

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srvCPUDescriptorHandle_);

}

//void BlockManager::Init(uint32_t maxCount)
//{
//	//blocks_.CreateBuffer(maxCount);
//
//	//// デバイスの取得
//	//auto* device = DirectXCommon::GetInstance()->GetDevice();
//	//// SRVヒープの取得
//	//auto* srvHeap = DirectXCommon::GetInstance()->GetSRVHeap();
//
//	//heapRange_ = srvHeap->RequestHeapAllocation(1);
//	//device->CreateShaderResourceView(blocks_.GetResources(), &blocks_.GetDesc(), heapRange_.GetHandle(0).cpuHandle_);
//}

void BlockManager::Draw(const Camera& camera)
{
	Blocks::PreDrow();

	//// 始点となる添え字
	//uint32_t index = 0u;
	//// サイズと始点を設定
	//std::list<std::pair<uint32_t, uint32_t>> sizeAndIndex;
	//// パーティクルのモデルと集合を1つづつ取得
	//for (auto& [model, blockList] : blockMap_) {
	//	// パーティクルのリストを取得
	//	const auto& blocks = blockList->GetBlockList();
	//	// もし、サイズが0なら飛ばす
	//	if (blocks.size() == 0u) { continue; }
	//	// パーティクルの書き込み先を設定 
	//	uint32_t targetIndex = index;
	//	// パーティクルをひとつづつ取得
	//	for (const auto& block : blocks) {
	//		// 書き込み先にパーティクルのデータを渡す
	//		blocks_[targetIndex].transform.World = block.transformMat_;
	//		blocks_[targetIndex].color = block.color_;
	//		targetIndex++;
	//	}
	//	// パーティクルの量と先頭位置を設定
	//	sizeAndIndex.push_back({ static_cast<uint32_t>(blocks.size()), index });
	//	blockList->SetLocation(index);
	//	// 次の先頭位置を更新
	//	index += static_cast<uint32_t>(blocks.size());
	//}
	//// 量と先頭位置のリストのイテレータ
	//std::list<std::pair<uint32_t, uint32_t>>::const_iterator indexItr = sizeAndIndex.cbegin();
	//// GPUアドレスを取得
	//const auto& gpuAddress = heapRange_.GetHandle(0u).gpuHandle_;

	//// 構造体のサイズ
	////const uint32_t matrixSize = sizeof(decltype(particles_)::map_matrix);

	// パーティクルのモデルと集合を1つづつ取得
	for (const auto& [model, particleList] : blockMap_) {

		// もし、サイズが0なら飛ばす
		if (particleList->GetBlockList().size() == 0u) { continue; }
		// インスタンシングで描画。
		//model->Draw(gpuAddress, indexItr->first, particleList->GetLoaction(), camera);
		particleList->Draw(camera);
		// 次の先頭位置に更新
		//indexItr++;
	}
}

IBlock* const BlockList::push_back(IBlock&& block)
{
	blocks_.push_back(std::move(block));
	return &blocks_.back();
}

BlockList::BlockList(uint32_t meshHundle)
{
	blocksResource_ = std::make_unique<Blocks>(meshHundle);
}

void BlockList::Draw(const Camera& camera)
{
	blocksResource_->Draw(camera, blocks_);
}
