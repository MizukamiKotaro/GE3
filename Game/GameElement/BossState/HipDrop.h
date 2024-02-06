#pragma once
#include "IBossState.h"
#include "SoUtils/SoLib/SoLib_Timer.h"
#include "../Boss.h"

class HipDrop : public IBossState {
public:

	enum class MoveState {
		kStartUp
	};

	struct AttackParameter {
		float totalTime_ = 0.f;
		bool damage_ = false;
		void(HipDrop:: *initFunc_)();
		void(HipDrop:: *updateFunc_)(const float deltaTime);
	};

public:
	HipDrop() = default;
	~HipDrop() = default;


	void Init() override;

	void Update(const float deltaTime) override;

	void ChangeState();

	void OnCollision(IEntity *other) override;

	void StartUpInit();

	void StartUpUpdate(const float deltaTime);

	void FollowUpdate(const float deltaTime);

	void FallDownInit();
	void FallDownUpdate(const float deltaTime);

	float MoveSpeed() const { return 5.f; }

	void NextParam();

	bool IsAttacked() const override;


private:

	const Vector2 &stageSize_ = GetBoss()->GetStageRange();

	Vector3 beginPos_;
	Vector3 endPos_;

	std::array<AttackParameter, 5u> stateArray_;
	uint32_t stateIndex_ = 0u;
	SoLib::DeltaTimer stateTimer_;

};