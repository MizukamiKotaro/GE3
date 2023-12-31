#pragma once
#include "SoUtils/Graphics/Color.h"
#include "SoUtils/Math/Angle.h"
#include "SoUtils/Math/Euler.h"
#include "SoUtils/SoLib/SoLib_Timer.h"
#include "Vector2.h"

class Player;
class MovingBall;

class BarrierItem
{
public:
	BarrierItem() = default;
	~BarrierItem() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void Attack(const Vector2 direction);

	void SetParent(Player *const parent) { parent_ = parent; }

	void SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList);

private:
	Player *parent_;

	uint32_t model_;

	std::list<std::unique_ptr<MovingBall>> *ballList_;

	Vector2 direction_;
	Matrix4x4 transformMat_;
	SoLib::Math::Euler eulerRot_;

	float radius_ = 2.f;
	float barrierAngle_ = std::cos(90._deg);

	SoLib::Color::RGB4 color_;

	SoLib::Time::DeltaTimer attackTimer_;

};