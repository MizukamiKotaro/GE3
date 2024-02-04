#pragma once
#include "IEntity.h"
#include <cstdint>
#include "SoUtils/Graphics/Color.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "IWeapon.h"
#include "SoUtils/Math/Euler.h"

class Stage;
class MovingBall;

class Hole :public IEntity {
public:

	enum class WeaponType : uint32_t {
		kSword,
		kPunch,
		kNeedle,

		kMaxSize,
	};

public:

	Hole();
	~Hole() = default;

	static void StaticInit();

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void OnCollision(IEntity *other) override;

	const Vector3 &GetPosition() const { return position_; }
	float GetRadius() const { return radius_; }

	void SetPos(const Vector3 &pos) { position_ = pos; }
	const Vector3 &GetPos() const { return position_; }

	void SetWeapon(IWeapon *weapon);

	static void EndAttack() { Hole::isAttacking_ = false; }

	float GetReflectionPow() const { return 1.2f; }

private:

	void UpdateBallChacher(const float deltaTime);

	void CalcTransMat();

private:

	uint32_t playerCount_{};
	uint32_t enemyCount_{};

	uint32_t activeCount_ = 5u;

	uint32_t model_;

	Vector3 position_;
	SoLib::Math::Euler rotate_;

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	float radius_;

	Matrix4x4 transMat_;

	IWeapon *weapon_;

	WeaponType weaponType_;

private:
	static std::array<uint32_t, static_cast<uint32_t>(WeaponType::kMaxSize)> modelList_;

	static bool isAttacking_;

	const float kModelScale_ = 1.f;
};