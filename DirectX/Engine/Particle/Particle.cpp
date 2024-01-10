#include "Particle.h"

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

Particle::Particle(const std::string& fileName)
{

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj("Plane");

	textureHundle_ = TextureManager::GetInstance()->LoadTexture("Resources/"+ fileName);

	CreateResources();

	CreateSRV();

	InitVariables();
}

Particle::Particle(uint32_t textureHundle)
{
	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj("Plane");

	textureHundle_ = textureHundle;

	CreateResources();

	CreateSRV();

	InitVariables();
}

Particle::~Particle()
{
	instancingResource_->Release();
	materialResource_->Release();
}

void Particle::Initialize()
{
	
}

void Particle::Update()
{
	GenerateParticle();

	for (std::list<Active>::iterator iter = actives_.begin(); iter != actives_.end();) {
		if (iter->lifeTime <= iter->currentTime) {
			iter = actives_.erase(iter);
			continue;
		}

		iter->transform.translate_ += iter->velocity;
		iter->transform.UpdateMatrix();

		iter->currentTime++;

		++iter;
	}

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Particle::Draw(const Camera& camera, BlendMode blendMode)
{
	PreDraw();

	Matrix4x4 billboardMat{};

	if (billboardTypeOpt_) {
		billbordType = billboardTypeOpt_.value();

		float pi = std::numbers::pi_v<float>;

		switch (billbordType)
		{
		case Particle::BillboardType::X:
			billboardMat = Matrix4x4::MakeRotateXMatrix(pi) * camera.transform_.worldMat_;
			break;
		case Particle::BillboardType::Y:
			billboardMat = Matrix4x4::MakeRotateYMatrix(pi) * camera.transform_.worldMat_;
			break;
		case Particle::BillboardType::Z:
			billboardMat = Matrix4x4::MakeRotateZMatrix(pi) * camera.transform_.worldMat_;
			break;
		case Particle::BillboardType::ALL:
			billboardMat = Matrix4x4::MakeRotateXYZMatrix(Vector3{}) * camera.transform_.worldMat_;
			break;
		default:
			break;
		}

		billboardMat.m[3][0] = 0.0f;
		billboardMat.m[3][1] = 0.0f;
		billboardMat.m[3][2] = 0.0f;
	}

	uint32_t instaceNum = static_cast<uint32_t>(actives_.size());
	instaceNum = std::clamp<uint32_t>(instaceNum, 0, kNumInstance);
	uint32_t index = 0;

	for (std::list<Active>::iterator iter = actives_.begin(); iter != actives_.end(); iter++) {
		if (index >= kNumInstance) {
			break;
		}

		if (billboardTypeOpt_) {
			instancingData_[index].World = Matrix4x4::MakeScaleMatrix(iter->transform.scale_) * billboardMat *
				Matrix4x4::MakeTranslateMatrix(iter->transform.translate_);
		}
		else {
			instancingData_[index].World = iter->transform.worldMat_;
		}
		instancingData_[index].WVP = instancingData_[index].World * camera.GetViewProjection();
		instancingData_[index].color = iter->color;

		index++;
	}

	materialData_->uvTransform = uvMatrix_;

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
	commandList->SetGraphicsRootConstantBufferView(3, light_.GetDirectionalLightGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, texManager->GetSRVGPUDescriptorHandle(textureHundle_));
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(UINT(modelData->mesh.verteces.size()), instaceNum, 0, 0);

}

void Particle::GenerateParticle()
{
	if (emitter.countTime_ >= emitter.generateCoolTime_) {

		for (uint32_t index = 0; index < emitter.generateParticleNum_; index++) {

			if (actives_.size() >= kNumInstance) {
				break;
			}

			actives_.push_back(CreateActive());
		}

		emitter.countTime_ = 0.0f;
	}

	emitter.countTime_++;
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
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	srvCPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVCPUDescriptorHandle();
	srvGPUDescriptorHandle_ = DescriptorHeapManager::GetInstance()->GetNewSRVGPUDescriptorHandle();

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(instancingResource_.Get(), &srvDesc, srvCPUDescriptorHandle_);

}

void Particle::CreateResources()
{
	CreateMaterialResource();

	CreateInstancingResource();
}

void Particle::CreateMaterialResource()
{
	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	*materialData_ = { Vector4(1.0f, 1.0f, 1.0f, 1.0f) , 0 };
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
}

void Particle::CreateInstancingResource()
{
	
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	instancingResource_ = DirectXCommon::CreateBufferResource(sizeof(ParticleForGPU) * kNumInstance);
	instancingData_ = nullptr;
	instancingResource_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData_));

	for (uint32_t index = 0; index < kNumInstance; index++) {
		instancingData_[index].WVP = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].World = Matrix4x4::MakeIdentity4x4();
		instancingData_[index].color = { 1.0f,1.0f,1.0f,0.0f };
	}
}

void Particle::InitVariables()
{
	light_.Init();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);

	RandomGenerator* rand = RandomGenerator::GetInstance();

	emitter.generateParticleNum_ = rand->RandInt(3, 6);

	emitter.generateCoolTime_ = 120.0f;

	emitter.min = { -2.5f,0.5f,-0.5f };
	emitter.max = { 2.5f,2.5f,0.5f };
}

Particle::Active Particle::CreateActive()
{
	RandomGenerator* rand = RandomGenerator::GetInstance();

	Active active{};
	active.transform.translate_ = rand->RandVector3(emitter.min, emitter.max) + emitter.pos;
	active.velocity = rand->RandVector3(-2.0f / 60.0f, 2.0f / 60.0f);
	active.color = { rand->RandFloat(0.0f,1.0f),rand->RandFloat(0.0f,1.0f),rand->RandFloat(0.0f,1.0f),1.0f };
	active.transform.UpdateMatrix();
	active.lifeTime = rand->RandFloat(120.0f, 240.0f);
	return active;
}

