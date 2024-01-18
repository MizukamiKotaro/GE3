#include "Punch.h"

Punch::Punch() {
}

void Punch::Init()
{
	scale_ = Vector3::one;
	rotate_ = Vector3::zero;
	sphere_ = Sphere{ .center_ = Vector3::zero, .radius_ = 1.f };

	CalcTransMat();

}

void Punch::Update(const float deltaTime)
{
}

void Punch::Draw()
{
}

void Punch::Attack()
{
}

bool Punch::IsAttacked() const
{
	return false;
}

float Punch::GetDamage() const
{
	return 0.0f;
}

void Punch::CalcTransMat() {

	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, sphere_.center_);
}
