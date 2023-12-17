#include "Camera.h"
#include "Engine/Base/WinApp/WinApp.h"

Camera::Camera()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,0.0f,-15.0f };
	transform_.UpdateMatrix();
	
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transform_.worldMat_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

	orthographicMat_ = Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
}



void Camera::Initialize()
{
	transform_.scale_ = { 1.0f,1.0f,1.0f };
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	transform_.translate_ = { 0.0f,0.0f,-15.0f };
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
}
