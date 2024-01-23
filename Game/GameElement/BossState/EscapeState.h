#pragma once
#include "IBossState.h"

class EscapeState : public IBossState {
public:
	EscapeState() = default;
	~EscapeState() = default;

	void Init() override;

	void Update(const float deltaTime) override;

private:



};