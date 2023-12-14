#include "Camera.h"
#include "Engine/WinApp/WinApp.h"

Camera::Camera()
{
	transforms_.scale_ = { 1.0f,1.0f,1.0f };
	transforms_.rotate_ = { 0.0f,0.0f,0.0f };
	transforms_.translate_ = { 0.0f,0.0f,-15.0f };
	transforms_.UpdateMatrix();
	
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transforms_.worldMat_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

}



void Camera::Initialize()
{
	transforms_.scale_ = { 1.0f,1.0f,1.0f };
	transforms_.rotate_ = { 0.0f,0.0f,0.0f };
	transforms_.translate_ = { 0.0f,0.0f,-15.0f };
	transforms_.UpdateMatrix();

	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transforms_.worldMat_);
	projectionMatrix_ = Matrix4x4::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight), 0.1f, 1050.0f);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;
}

void Camera::Update()
{
	transforms_.UpdateMatrix();
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(transforms_.worldMat_);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;
}
