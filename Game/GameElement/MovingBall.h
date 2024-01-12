#pragma once
#include "../Collision/Collision.h"
#include <Camera.h>
#include "SoUtils/Graphics/Color.h"
#include "Vector3.h"
#include <SoUtils/SoLib/SoLib_Timer.h>

class MapChip;

class MovingBall {
public:
	MovingBall() = default;
	~MovingBall() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	const Vector3 &GetBeforePos() const { return beforePos_; }
	const Vector3 &GetNowPos() const { return sphere_.center_; }

	void AddAcceleration(const Vector3 &vec);

	static void SetMapChip(MapChip *mapChip);

	bool GetIsAlive() const { return isAlive_; }

	void SetPos(const Vector3 &pos) { sphere_.center_ = pos; }
	void SetVelocity(const Vector3 &vec) { velocity_ = vec; }

private:

	SoLib::Time::DeltaTimer aliveTime_{ 2.f };

	bool isAlive_ = true;

	void UpdateRigidbody(const float deltaTime);

	Sphere sphere_;

	Matrix4x4 transformMat_;
	SoLib::Color::RGB4 color_;

	uint32_t model_;
	float modelScale_ = 0.5f;


	Vector3 beforePos_;

	Vector3 velocity_;

	Vector3 acceleration_;

	static MapChip *mapChip_;

};