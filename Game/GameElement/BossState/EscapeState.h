#pragma once
#include "IBossState.h"
#include "SoUtils/SoLib/SoLib.h"

class EscapeState : public IBossState {
public:
	EscapeState() = default;
	~EscapeState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void ChangeState();

private:

	SoLib::Time::DeltaTimer changeTimer_;

};