#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include <wrl.h>
#include <d3d12.h>

class Camera {

public:

	struct CameraForGPU
	{
		Vector3 worldPosition;
	};

	Camera();
	~Camera();

	void Initialize();

	void Update();

	const Matrix4x4 GetViewProjection() const { return viewProjectionMatrix_; }

	const Matrix4x4 GetOrthographicMat() const { return orthographicMat_; }

	const D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return cameraForGPUResource_->GetGPUVirtualAddress(); }

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> cameraForGPUResource_;
	CameraForGPU* cameraForGPUData_;

private:

	void CreateResource();

public:

	Transform transform_;

private:

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

	Matrix4x4 orthographicMat_;

};