#pragma once
#include "IEntity.h"
#include <cstdint>
#include "SoUtils/Graphics/Color.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "IWeapon.h"

class Stage;
class MovingBall;

class Hole :public IEntity {
public:

	struct BallCatcher {
		MovingBall *ball_;
		float time_;

		Vector3 begin_;
	};


public:

	Hole();
	~Hole() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void OnCollision(IEntity *other) override;

	const Vector3 &GetPosition() const { return position_; }
	float GetRadius() const { return radius_; }

	void SetPos(const Vector3 &pos) { position_ = pos; }
	const Vector3 &GetPos() const { return position_; }

	void AddWeapon(IWeapon *weapon);

private:

	void UpdateBallChacher(const float deltaTime);

	void CalcTransMat();

private:

	uint32_t playerCount_{};
	uint32_t enemyCount_{};

	uint32_t activeCount_ = 5u;

	uint32_t model_;

	Vector3 position_;

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	float radius_;

	Matrix4x4 transMat_;

	std::list<BallCatcher> ballList_;

	std::vector<IWeapon *> weapons_;

private:

	const float kModelScale_ = 1.f;
};