#pragma once
#include "../Collision/Collision.h"
#include <Camera.h>

class MovingBall
{
public:
	MovingBall() = default;
	~MovingBall() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw(const Camera &camera) const;

	const Vector3 &GetBeforePos() const { return beforePos_; }
	const Vector3 &GetNowPos() const { return sphere_.center_; }

private:

	void UpdateRigidbody(const float deltaTime);

	Sphere sphere_;

	Vector3 beforePos_;

	Vector3 velocity_;

	Vector3 acceleration_;

};