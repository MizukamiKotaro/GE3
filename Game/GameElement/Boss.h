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

	/// @brief Transformクラスの取得
	/// @return Transformクラス
	auto &GetTransform() { return transform_; }

	/// @brief 行動パターンの遷移
	/// @tparam T ボスのステート
	template<SoLib::IsBased<IBossState> T>
	void ChangeState();

	/// @brief 攻撃中か
	/// @return 攻撃中ならtrue
	bool IsAttacked() const {
		return  bossState_ && bossState_->IsAttacked();
	}

	/// @brief 当たり判定の取得
	/// @return 球体コライダ
	const Sphere &GetCollision() const { return sphere_; }

	/// @brief スロットの開始
	void StartSlot();

	/// @brief スロットの停止
	void StopSlot();

	FaceType GetFaceType() const;

	Vector2 GetStageRange() const;

	void DownGrade();

	void AddAcceleration(const Vector3 &vec) { acceleration_ += vec; }
	void SetAcceleration(const Vector3 &vec) { acceleration_ = vec; }
	void SetVelocity(const Vector3 &vec) { velocity_ = vec; }

private:

	/// @brief 行列の計算
	void CalcTransMat();

	/// @brief 当たり判定の計算
	void CalcCollision();

	/// @brief Slotへのデータの転送
	void TransferData();

private:

	std::unique_ptr<IBossState> nextState_ = nullptr;
	std::unique_ptr<IBossState> bossState_ = nullptr;

	VariantItem<float> vTackleDamage_{ "TackleDamage", 1.f };

	VariantItem<float> vMaxHealth_{ "MaxHealth", 10.f };
	float health_;

	HPBar *pHPBar_ = nullptr;

	Sphere sphere_;

	Vector3 velocity_;
	Vector3 acceleration_;

	SimpleTransform transform_;

	MapChip *mapChip_ = nullptr;
	const SoLib::Containers::Array2D<uint32_t> *mapData_ = nullptr;

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
