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
#include "StateParametor.h"
#include "Hole.h"
#include "DirectionArrow.h"
#include "BoneModel.h"

class MapChip;

class IPlayerState;
class PlayerFacing;
class PlayerTackle;

class Player : public IEntity {
	friend PlayerFacing;
	friend PlayerTackle;

public:

	enum class State {
		kFacing,	// 方向転換
		kTackle,	// 突進中
		kMoving,	// 通常移動
	};

public:

	Player();
	~Player() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void SetHPBar(HPBar *hpBar);

	const Matrix4x4 &GetTransMat() const { return transformMat_; }
	const Vector3 &GetGrobalPos() const { return *reinterpret_cast<const Vector3 *>(transformMat_.m[3]); }
	const Vector3 &GetPos() const { return sphere_.center_; }

	float GetRadius() const { return sphere_.radius_; }

	void InputAction(Input *const input, const float deltaTime);

	void Move(const float power, const float deltaTime);

	void UpdateRigidbody(const float deltaTime);

	void SetMapChip(MapChip *const mapChip);

	void SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList);

	void Landing();

	void AttackStart();

	void CalcTransMat();

	/// @brief ダメージを与える
	/// @param weapon 武器のアドレス
	/// @return ダメージが入ったか
	bool Damage(IWeapon *weapon);

	/// @brief ダメージを与える
	/// @param damage ダメージ量
	/// @return ダメージが入ったか
	bool Damage(float damage);

	void OnCollision(IEntity *other) override;

	const Sphere &GetSphere() const { return sphere_; }

	template <SoLib::IsBased<IPlayerState> T>
	void SetPlayerState();

	State GetState() const;

	bool ImGuiWidget();

	void ReflectHole(const Hole &hole);

	// 追加
	const Vector3 &GetVelocity() const { return velocity_; }

	void AddAcceleration(const Vector3 &vec) { acceleration_ += vec; }

private:

	// StateParametor<Player> stateParametor_;

	std::unique_ptr<BoneModel> playerModel_ = nullptr;
	std::unique_ptr<std::array<BoneModel::SimpleTransform, 8u>> modelTransform_;
	std::unique_ptr<std::array<Matrix4x4, 8u>> modelMat_;
	SoLib::DeltaTimer inputFacingTimer_;

	std::unique_ptr<IPlayerState> playerState_;

	float health_;
	VariantItem<float> vMaxHealth_{ "MaxHealth", 8.f };
	HPBar *pHPBar_;

	VariantItem<float> vAttackTime_{ "AttackTime", 0.75f };
	std::list<std::unique_ptr<MovingBall>> *ballList_;
	bool isLanding_ = false;

	Vector2 preInputRStick_;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Sphere sphere_{ .center_ = Vector3::zero, .radius_ = 1.f };

	Matrix4x4 transformMat_;

	Vector3 beforePos_;

	uint32_t model_;
	SoLib::Color::RGB4 color_;

	uint32_t arrowModel_;
	Quaternion arrowFacing_;
	Matrix4x4 arrowMat_;

	std::unique_ptr<DirectionArrow> directionArrow_ = nullptr;

	SoLib::Time::DeltaTimer invincibleTime_;

	Vector3 velocity_;

	Vector3 acceleration_;

	MapChip *mapData_;

	Audio *audio_ = nullptr;

	uint32_t jumpSE_;

	float kReflectionPower_ = 0.9f;

	Vector2 kMaxSpeed_ = Vector2{ 1.f,1.f }*15.f;

	SoLib::Math::Euler rotateTarget_;
};

class IPlayerState {
public:
	IPlayerState(Player *player) : player_(player) {}
	virtual ~IPlayerState() = default;

	virtual void Init() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual bool IsExit() const = 0;

	virtual Player::State GetState() const = 0;

	Player *const GetPlayer() const { return player_; }
protected:
	Player *const player_;
};

class PlayerFacing : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	~PlayerFacing() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	bool IsExit() const override { return false; }

	Player::State GetState() const override { return Player::State::kFacing; }
};

class PlayerTackle : public IPlayerState {
public:
	using IPlayerState::IPlayerState;
	~PlayerTackle() = default;

	void Init() override;
	void Update(const float deltaTime) override;
	bool IsExit() const override;

	Player::State GetState() const override { return Player::State::kTackle; }

	SoLib::DeltaTimer activeTime_;
};

template<SoLib::IsBased<IPlayerState> T>
inline void Player::SetPlayerState() {
	// 変更先と現在の状態が一致した場合は無視
	if (dynamic_cast<T *>(playerState_.get())) { return; }

	// 状態を変更
	playerState_ = std::make_unique<T>(this);
	// 初期化
	playerState_->Init();

}
