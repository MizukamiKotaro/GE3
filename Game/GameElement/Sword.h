#pragma once
#include <Vector3.h>
#include <Matrix4x4.h>
#include <SoUtils/Math/Euler.h>
#include <SoUtils/Graphics/Color.h>

class Sword
{
public:
	Sword();
	~Sword() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	void SetPos(const Vector3 &pos) { position_ = pos; }

	const Vector3 &GetPos() const { return position_; }

	void CalcTransMat();

private:

	uint32_t model_;

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Vector3 position_;

	Matrix4x4 transMat_;

};