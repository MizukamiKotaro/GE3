#include "Particle.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "ModelCommon/ModelData/ModelDataManager/ModelDataManager.h"
#include "Engine/Base/DescriptorHeapManager/DescriptorHeapManager.h"
#include "ParticleCommon/ParticleCommon.h"

Particle::Particle(const std::string& fileName)
{

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj(fileName);

	textureHundle_ = modelManager->GetModelData(meshHundle_)->textureHundle_;

	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();

	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix) * kNumInstance);
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].WVP = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].World = Matrix4x4::MakeIdentity4x4();

		transform_[index] = Transform();
		transform_[index].translate_ = { index * 0.1f,index * 0.1f ,index * 0.1f };
		transform_[index].scale_ *= 10.0f;
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

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

Particle::~Particle()
{
	instancingResource_->Release();
	materialResource_->Release();
	directionalLightResource_->Release();
}

void Particle::Initialize()
{
	
}

void Particle::Update()
{
	for (uint32_t index = 0; index < kNumInstance; index++) {
		transform_[index].UpdateMatrix();
	}

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Particle::Draw(const Matrix4x4& viewProjection)
{
	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].World = transform_[index].worldMat_;
		instancingData_[index].WVP = transform_[index].worldMat_ * viewProjection;
	}
	materialData_->uvTransform = uvMatrix_;

	TextureManager* texManager = TextureManager::GetInstance();

	const ModelData* modelData = ModelDataManager::GetInstance()->GetModelData(meshHundle_);

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
	commandList->DrawInstanced(UINT(modelData->mesh.verteces.size()), kNumInstance, 0, 0);

}

void Particle::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = kNumInstance;
	srvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	srvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVCPUDescriptorHandle();
	srvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVGPUDescriptorHandle();

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srvCPUDescriptorHandle_);

}


