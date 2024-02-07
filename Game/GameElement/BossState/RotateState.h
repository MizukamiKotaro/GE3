#pragma once
#include "IBossState.h"
#include <vector>
#include "SoUtils/SoLib/SoLib_Timer.h"

class RotateState :public IBossState {
public:
	struct AttackParameter {
		float totalTime_ = 0.f;
		bool damage_ = false;
		void(RotateState:: *initFunc_)();
		void(RotateState:: *updateFunc_)(const float deltaTime);
	};
public:
	RotateState() = default;
	~RotateState() override = default;

	void Init() override;

	void Update(const float deltaTime) override;

	void OnCollision(IEntity *other) override;

	bool IsAttacked() const override;

	void ChangeState();

	void NextParam();

public:

	void StartUpInit();

	void StartUpUpdate(const float deltaTime);

	void RotateUpdate(const float deltaTime);

private:

	std::vector<AttackParameter> stateArray_;
	uint32_t stateIndex_ = 0u;
	SoLib::DeltaTimer stateTimer_;

	uint32_t scaleCount_;
	SoLib::DeltaTimer scaleTimer_;
};