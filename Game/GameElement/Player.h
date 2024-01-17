#pragma once
#pragma once
#include <memory>
#include "Barrier.h"
#include <SoUtils/Math/Euler.h>
#include "Input/Input.h"
#include "Audio.h"
#include "IEntity.h"

class MapChip;

class Player : public IEntity {
public:
	Player() = default;
	~Player() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	const Matrix4x4 &GetTransMat() const { return transformMat_; }
	const Vector3 &GetGrobalPos() const { return *reinterpret_cast<const Vector3 *>(transformMat_.m[3]); }

	float GetRadius() const { return radius_; }

	void InputAction(Input *const input, const float deltaTime);

	void Move(const float power, const float deltaTime);

	void UpdateRigidbody(const float deltaTime);

	void SetMapChip(MapChip *const mapChip);

	void SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList);

	void Landing();

	void CalcTransMat();

private:

	std::list<std::unique_ptr<MovingBall>> *ballList_;
	bool isLanding_ = false;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Vector3 transform_;

	float radius_ = 1.f;

	Matrix4x4 transformMat_;

	Vector3 beforePos_;

	uint32_t model_;
	SoLib::Color::RGB4 color_;

	std::unique_ptr<BarrierItem> barrier_ = nullptr;

	Vector3 velocity_;

	Vector3 acceleration_;

	MapChip *mapData_;

	Audio *audio_ = nullptr;

	uint32_t jumpSE_;
};