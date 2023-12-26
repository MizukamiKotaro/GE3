#pragma once
#include <Vector2.h>
#include "SoUtils/SoLib/SoLib_Timer.h"
#include "SoUtils/Graphics/Color.h"
#include "SoUtils/Math/Euler.h"
#include "SoUtils/Math/Angle.h"

class Player;

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

private:
	Player *parent_;

	uint32_t model_;

	Vector2 direction_;
	Matrix4x4 transformMat_;
	SoLib::Math::Euler eulerRot_;

	SoLib::Color::RGB4 color_;

	SoLib::Time::DeltaTimer attackTimer_;

};