#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "SoUtils/Math/Angle.h"
#include "SoUtils/SoLib/SoLib.h"
#include "IEntity.h"

class Stage;

class BallSpawner : public IEntity {
public:
	BallSpawner();
	~BallSpawner() = default;

	void Init();

	void Update(const float deltaTime);

	void Draw();

	/// @brief 弾の発射
	void Fire();

	bool GetIsAlive() const { return isAlive_; }

	static void SetStage(Stage *stage) { pStage_ = stage; }

	//void OnCollision(IEntity *other) override;

private:

	Vector3 position_;

	// Matrix4x4 transMat_ = Matrix4x4::MakeIdentity4x4();

	float fireAngle_ = 60._deg;

	SoLib::Time::DeltaTimer fireSpanTimer_;
	float firePower_ = 5.f;

	float fireSpan_ = 1.f;

	bool isAlive_ = true;

	static Stage *pStage_;

};