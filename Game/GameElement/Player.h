#pragma once
#pragma once
#include <memory>
#include "Barrier.h"
#include <SoUtils/Math/Euler.h>
#include "Input/Input.h"
#include "Audio.h"
#include "IEntity.h"
#include "Shape/Sphere.h"
#include "Sword.h"
#include "HPBar/HPBar.h"

class MapChip;

class Player : public IEntity {
public:
	Player();
	~Player() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void SetHPBar(HPBar *hpBar);

	const Matrix4x4 &GetTransMat() const { return transformMat_; }
	const Vector3 &GetGrobalPos() const { return *reinterpret_cast<const Vector3 *>(transformMat_.m[3]); }

	float GetRadius() const { return sphere_.radius_; }

	void InputAction(Input *const input, const float deltaTime);

	void Move(const float power, const float deltaTime);

	void UpdateRigidbody(const float deltaTime);

	void SetMapChip(MapChip *const mapChip);

	void SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList);

	void Landing();

	void CalcTransMat();
	/// @brief ダメージを与える
	/// @param weapon ブキ
	/// @return ダメージが入ったか
	bool Damage(IWeapon *weapon);

	/// @brief ダメージを与える
	/// @param damage ダメージ量
	/// @return ダメージが入ったか
	bool Damage(float damage);

	void OnCollision(IEntity *other) override;

	const auto &GetSphere() const { return sphere_; }

private:

	float health_;
	VariantItem<float> vMaxHealth_{ "MaxHealth", 10.f };
	HPBar *pHPBar_;

	std::list<std::unique_ptr<MovingBall>> *ballList_;
	bool isLanding_ = false;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Sphere sphere_{ .center_ = Vector3::zero, .radius_ = 1.f };

	Matrix4x4 transformMat_;

	Vector3 beforePos_;

	uint32_t model_;
	SoLib::Color::RGB4 color_;

	std::unique_ptr<BarrierItem> barrier_ = nullptr;

	SoLib::Time::DeltaTimer invincibleTime_;

	Vector3 velocity_;

	Vector3 acceleration_;

	MapChip *mapData_;

	Audio *audio_ = nullptr;

	uint32_t jumpSE_;
};