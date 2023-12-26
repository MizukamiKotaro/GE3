#pragma once
#include "../Collision/Collision.h"
#include <Camera.h>
#include "SoUtils/Graphics/Color.h"

class MovingBall {
public:
	MovingBall() = default;
	~MovingBall() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	const Vector3 &GetBeforePos() const { return beforePos_; }
	const Vector3 &GetNowPos() const { return sphere_.center_; }

private:

	void UpdateRigidbody(const float deltaTime);

	Sphere sphere_;

	Matrix4x4 transformMat_;
	SoLib::Color::RGB4 color_;

	uint32_t model_;
	float modelScale_ = 0.5f;


	Vector3 beforePos_;

	Vector3 velocity_;

	Vector3 acceleration_;

};