#include "Model.h"

#include <cassert>
#include "TextureManager/TextureManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include "ModelData/ModelDataManager/ModelDataManager.h"
#include "Camera.h"
#include "Externals/imgui/imgui.h"

Model::Model(const std::string& fileName)
{

	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = modelManager->LoadObj(fileName);

	textureHundle_ = modelManager->GetModelData(meshHundle_)->textureHundle_;

	srvGPUDescriptorHandle_ = TextureManager::GetInstance()->GetSRVGPUDescriptorHandle(textureHundle_);

	CreateResources();

	InitVariables();
}

Model::Model(uint32_t meshHundle)
{
	ModelDataManager* modelManager = ModelDataManager::GetInstance();

	meshHundle_ = meshHundle;

	textureHundle_ = modelManager->GetModelData(meshHundle_)->textureHundle_;

	srvGPUDescriptorHandle_ = TextureManager::GetInstance()->GetSRVGPUDescriptorHandle(textureHundle_);

	CreateResources();

	InitVariables();
}

Model::~Model()
{
	transformationResource_->Release();
	materialResource_->Release();
}

void Model::Initialize()
{
	
}

void Model::Update()
{
	transform_.UpdateMatrix();

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}

void Model::Draw(const Camera& camera, BlendMode blendMode)
{

	PreDraw();

	transformationData_->World = transform_.worldMat_;
	transformationData_->WVP = transform_.worldMat_ * camera.GetViewProjection();
	transformationData_->WorldInverse = Matrix4x4::Inverse(Matrix4x4::MakeScaleMatrix(transform_.scale_)) *
		Matrix4x4::MakeRotateXYZMatrix(transform_.rotate_) * Matrix4x4::MakeTranslateMatrix(transform_.translate_);
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
	commandList->SetGraphicsRootConstantBufferView(1, transformationResource_->GetGPUVirtualAddress());

	//平行光源CBufferの場所を設定
	commandList->SetGraphicsRootConstantBufferView(3, light_.GetDirectionalLightGPUVirtualAddress());
	// カメラの設定
	commandList->SetGraphicsRootConstantBufferView(4, camera.GetGPUVirtualAddress());
	// pointLight の設定
	commandList->SetGraphicsRootConstantBufferView(5, light_.GetPointLightGPUVirtualAddress());
	// spotLight の設定
	commandList->SetGraphicsRootConstantBufferView(6, light_.GetSpotLightGPUVirtualAddress());

	commandList->SetGraphicsRootDescriptorTable(2, srvGPUDescriptorHandle_);
	//描画!!!!（DrawCall/ドローコール）
	commandList->DrawInstanced(UINT(modelData->mesh.verteces.size()), 1, 0, 0);

}

void Model::SetTex(uint32_t hundle)
{
	textureHundle_ = hundle;
	srvGPUDescriptorHandle_ = TextureManager::GetInstance()->GetSRVGPUDescriptorHandle(textureHundle_);
}

void Model::SetMesh(uint32_t hundle)
{
	meshHundle_ = hundle;

	textureHundle_ = ModelDataManager::GetInstance()->GetTextureHundle(hundle);

	srvGPUDescriptorHandle_ = TextureManager::GetInstance()->GetSRVGPUDescriptorHandle(textureHundle_);
}

void Model::CreateResources()
{
	CreateMaterialResource();

	CreateTransformationResource();
}

void Model::CreateMaterialResource()
{
	materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));

	materialData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData_->enableLighting = 3;
	materialData_->uvTransform = Matrix4x4::MakeIdentity4x4();
	materialData_->shininess = 40.0f;
	materialData_->supeqularColor = { 1.0f, 1.0f, 1.0f };
}

void Model::CreateTransformationResource()
{
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	transformationResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
	transformationData_ = nullptr;
	transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData_));
	*transformationData_ = { Matrix4x4::MakeIdentity4x4() ,Matrix4x4::MakeIdentity4x4(), Matrix4x4::Inverse(Matrix4x4::MakeIdentity4x4()) };
}

void Model::InitVariables()
{
	light_.Init();

	transform_ = Transform();

	uvScale_ = { 1.0f,1.0f,1.0f };
	uvRotate_ = { 0.0f,0.0f,0.0f };
	uvPos_ = { 0.0f,0.0f,0.0f };

	uvMatrix_ = Matrix4x4::MakeAffinMatrix(uvScale_, uvRotate_, uvPos_);
}



