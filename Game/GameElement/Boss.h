#pragma once
#include <cstdint>

#include "SoUtils/Math/Euler.h"
#include "Stage.h"
#include "Vector3.h"
#include "Slot/Slot.h"
#include "BossState/IBossState.h"

class Boss : public IEntity {
public:
	Boss();
	~Boss() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void Draw() override;

	void OnCollision(IEntity *other) override;

	void SetCamera(Camera *camera);

private:

	void CalcTransMat();

	void CalcCollision();

	void TransferData();

private:

	std::unique_ptr<IBossState> S = nullptr;

	Sphere sphere_;

	Vector3 scale_;
	SoLib::Math::Euler rotate_;
	Vector3 translate_;

	Matrix4x4 transMat_;

	std::unique_ptr<Slot> slot_;

	Camera *pCamera_ = nullptr;

};