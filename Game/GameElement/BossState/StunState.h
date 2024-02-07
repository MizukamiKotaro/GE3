#pragma once
#include "IBossState.h"
#include "SoUtils/SoLib/SoLib.h"

class StunState : public IBossState {
public:
	StunState();
	~StunState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void OnCollision(IEntity *other) override;

	void ChangeState();

private:
	// ストップタイマー
	SoLib::Time::DeltaTimer stateTimer_;

	SoLib::Time::DeltaTimer invincibleTime_;

};