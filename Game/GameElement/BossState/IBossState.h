#pragma once
#include <cstdint>

class Boss;

class IBossState {
public:

	enum class SlotState {
		kLow,
		kMiddle,
		kHigh,
		kMaxmum
	};

public:
	IBossState() = default;
	virtual ~IBossState() = default;

	virtual void Init() = 0;

	virtual void Update(const float deltaTime) = 0;

	virtual void Draw() = 0;

	static void SetBoss(Boss *boss) { if (boss) { pBoss_ = boss; } }

protected:

	SlotState slotState_;

	static Boss *const GetBoss() { return pBoss_; }

private:

	static Boss *pBoss_;

};