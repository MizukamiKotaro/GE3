#pragma once
#include "Model.h"
#include <memory>

class Camera;

class Screen {
public: 

	Screen();

	void Initialize();

	void Update();

	void Draw(Camera* camera);

	void SetGPUHandle(D3D12_GPU_DESCRIPTOR_HANDLE srvGPUDescriptorHandle);

private:
	
	std::unique_ptr<Model> model_;
};
