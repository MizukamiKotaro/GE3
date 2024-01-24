#pragma once
#include "IBossState.h"

class TackleState : public IBossState {
public:

	struct AttackParameter {

	};

	TackleState() = default;
	~TackleState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

};