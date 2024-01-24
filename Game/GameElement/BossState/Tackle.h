#pragma once
#include "IBossState.h"
#include <cstdint>
#include <array>
#include "SoUtils/SoLib/SoLib.h"
#include "Shape/Line.h"

class TackleState : public IBossState {
public:

	struct AttackParameter {
		float totalTime_ = 0.f;
		float damage_ = 0.f;
		void(TackleState:: *initFunc_)();
		void(TackleState:: *updateFunc_)();
	};

public:

	TackleState() = default;
	~TackleState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void ChangeState();

	void AttackStart();
	void AttackUpdate();

private:

	std::array<AttackParameter, 4u> stateArray_;
	uint32_t stateIndex_ = 0u;
	SoLib::Time::DeltaTimer stateTimer_;

	Segment attackVec_;

};