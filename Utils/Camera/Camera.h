#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"

class Camera {

public:

	Camera();
	~Camera() = default;

	void Initialize();

	void Update();

	const Matrix4x4 GetViewProjection() { return viewProjectionMatrix_; }

public:

	Transform transform_;

private:

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

};