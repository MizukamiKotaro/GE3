#include "SlotState.h"
#include "../Boss.h"
#include "../Player.h"
#include "Tackle.h"

SlotState::SlotState()
{
}

void SlotState::Init()
{
	// スロットの開始
	GetBoss()->StartSlot();
	slotTimer_.Start(10.f);
}

void SlotState::Update(const float deltaTime)
{
	// スロットのタイマーの更新
	slotTimer_.Update(deltaTime);
	stopTimer_.Update(deltaTime);

	// スロットの制限時間が来たら停止
	if (slotTimer_.IsActive() && slotTimer_.IsFinish()) {
		GetBoss()->StopSlot();
		stopTimer_.Start(1.f);
	}

	if (stopTimer_.IsActive() && stopTimer_.IsFinish()) {
		ChangeState();
	}
}

void SlotState::OnCollision([[maybe_unused]] IEntity *other)
{
}

void SlotState::ChangeState()
{
	GetBoss()->ChangeState<TackleState>();
}
