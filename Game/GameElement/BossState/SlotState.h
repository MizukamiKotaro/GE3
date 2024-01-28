#pragma once
#include "IBossState.h"
#include "SoUtils/SoLib/SoLib.h"

class SlotState : public IBossState {
public:
	SlotState();
	~SlotState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void OnCollision(IEntity *other) override;

	void ChangeState();

private:
	// スロットのタイマー
	SoLib::Time::DeltaTimer slotTimer_;
	// ストップタイマー
	SoLib::Time::DeltaTimer stopTimer_;
};