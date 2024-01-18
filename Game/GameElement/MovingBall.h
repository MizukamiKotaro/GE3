#pragma once
#include "../Collision/Collision.h"
#include <Camera.h>
#include "SoUtils/Graphics/Color.h"
#include "Vector3.h"
#include <SoUtils/SoLib/SoLib_Timer.h>
#include "IEntity.h"
#include "SoUtils/Math/Euler.h"

class MapChip;
class BoundPin;

class MovingBall : public IEntity {
public:
	enum class Team : uint32_t {
		kPlayer = 0b01,
		kEnemy = 0b10
	};


	MovingBall() = default;
	~MovingBall() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	const Vector3 &GetBeforePos() const { return beforePos_; }
	const Vector3 &GetNowPos() const { return sphere_.center_; }

	void AddAcceleration(const Vector3 &vec);

	static void SetMapChip(MapChip *mapChip);

	void SetIsAlive(const bool isAlive) { isAlive_ = isAlive; }
	bool GetIsAlive() const { return isAlive_; }

	const auto &GetSphere() const { return sphere_; }

	void SetPos(const Vector3 &pos) { sphere_.center_ = pos; }
	void SetVelocity(const Vector3 &vec) { velocity_ = vec; }

	void SetTeam(const Team team);

	void OnCollision(IEntity *other) override;

	void ReflectPin(const BoundPin &pin);

private:

	bool isChathed_ = false;

	Team team_ = Team::kPlayer;

	SoLib::Time::DeltaTimer aliveTime_{ 4.f };

	bool isAlive_ = true;

	void UpdateRigidbody(const float deltaTime);

	Vector3 scale_ = Vector3::one;
	SoLib::Math::Euler rotate_;
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