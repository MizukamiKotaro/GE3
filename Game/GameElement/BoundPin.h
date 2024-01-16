#pragma once
#include "IEntity.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cstdint>
#include "SoUtils/Graphics/Color.h"
#include <Shape/Sphere.h>

class BoundPin :public IEntity {
public:
	BoundPin();
	~BoundPin() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void SetPos(const Vector3 &pos) { position_ = pos; }
	const Vector3 &GetPos() const { return position_; }

	const auto &GetTransMat() const { return transMat_; }


private:

	void CalcTransMat();

private:

	SoLib::Color::RGB4 color_ = 0xFFFFFFFF;

	// Sphere sphere_;

	Vector3 position_;

	float radius_;

	uint32_t model_;

	Matrix4x4 transMat_;

};