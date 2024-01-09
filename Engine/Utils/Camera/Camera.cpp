#include "Camera.h"
#include "Engine/Base/WinApp/WinApp.h"
#include "DirectXCommon/DirectXCommon.h"

Camera::Camera()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,0.0f,-15.0f };
	transform_.UpdateMatrix();

	CreateResource();
	
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	orthographicMat_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
}

Camera::~Camera()
{
	cameraForGPUResource_->Release();
}



void Camera::Initialize()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,1.0f,-25.0f };
	transform_.UpdateMatrix();

	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;
}

void Camera::Update()
{
	transform_.UpdateMatrix();
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	cameraForGPUData_->worldPosition = transform_.GetWorldPosition();
}

void Camera::CreateResource()
{
	//WVP用のリソースを作る。Matrix4x4　1つ分のサイズを用意する
	cameraForGPUResource_ = DirectXCommon::CreateBufferResource(sizeof(CameraForGPU));
	cameraForGPUData_ = nullptr;
	cameraForGPUResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraForGPUData_));
	cameraForGPUData_->worldPosition = transform_.GetWorldPosition();
}
