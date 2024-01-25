#pragma once
#include <cstdint>

#include "SoUtils/Math/Euler.h"
#include "Stage.h"
#include "Vector3.h"
#include "Slot/Slot.h"
#include "BossState/IBossState.h"
#include "SimpleTransform.h"
#include "HPBar/HPBar.h"

class Boss : public IEntity {
public:
	Boss();
	~Boss() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void OnCollision(IEntity *other) override;

	void Damage(IWeapon *weapon);

	void Damage(float damage);

	void SetHPBar(HPBar *hpBar);

	void SetCamera(Camera *camera);

	auto &GetTransform() { return transform_; }

	template<SoLib::IsBased<IBossState> T>
	void ChangeState();

	bool IsAttacked() const {
		return  bossState_ && bossState_->IsAttacked();
	}

	const Sphere &GetCollision() const {
		return sphere_;
	}

private:

	void CalcTransMat();

	void CalcCollision();

	void TransferData();

private:

	std::unique_ptr<IBossState> nextState_ = nullptr;
	std::unique_ptr<IBossState> bossState_ = nullptr;

	VariantItem<float> vTackleDamage_{ "TackleDamage", 5.f };

	VariantItem<float> vMaxHealth_{ "MaxHealth", 200.f };
	float health_;

	HPBar *pHPBar_ = nullptr;

	Sphere sphere_;

	SimpleTransform transform_;

	//Vector3 scale_;
	//SoLib::Math::Euler rotate_;
	//Vector3 translate_;

	//Matrix4x4 transMat_;

	std::unique_ptr<Slot> slot_;

	Camera *pCamera_ = nullptr;

	float maxHealth_;
	float nowHealth_;

};

template<SoLib::IsBased<IBossState> T>
inline void Boss::ChangeState() {
	nextState_ = std::make_unique<T>();
}
