#include "SlotState.h"
#include "../Boss.h"
#include "../Player.h"
#include "Tackle.h"
#include "HipDrop.h"
#include "RotateState.h"

SlotState::SlotState()
{
}

void SlotState::Init()
{
	// スロットの開始
	GetBoss()->StartSlot();
	slotTimer_.Start(7.f);
}

void SlotState::Update(const float deltaTime)
{
	// スロットのタイマーの更新
	slotTimer_.Update(deltaTime);
	stopTimer_.Update(deltaTime);

	// スロットの制限時間が来たら停止 もしくは 顔が悪い場合は即終了
	if ((slotTimer_.IsActive() && slotTimer_.IsFinish()) || (GetBoss()->GetFaceType() == FaceType::kBad && not stopTimer_.IsActive())) {
		// スロットも停止
		GetBoss()->StopSlot();
		// 停止のタイマー
		stopTimer_.Start(1.f);
	}

	// 停止用のタイマーが停止したら
	if (stopTimer_.IsActive() && stopTimer_.IsFinish()) {
		// 顔の状態を転送
		SetFaceType(GetBoss()->GetFaceType());
		// 状態遷移
		ChangeState();
	}
}

void SlotState::OnCollision([[maybe_unused]] IEntity *other) {
	// プレイヤであった場合
	Player *player = dynamic_cast<Player *>(other);
	if (player) {
		Vector3 toPlayer = Vector3{ player->GetSphere().center_ - GetBoss()->GetTransform().translate_ }.Normalize();

		player->AddAcceleration(toPlayer * 3.f);

		GetBoss()->AddAcceleration(toPlayer * -1.5f);
	}

	// 武器であった場合
	IWeapon *weapon = dynamic_cast<IWeapon *>(other);
	if (weapon) {
		if (not stopTimer_.IsActive()) {
			GetBoss()->DownGrade();
			slotTimer_.Start();

		}
	}
}

void SlotState::ChangeState() {
	auto *const boss = GetBoss();
	switch (GetFaceType()) {
	case IBossState::FaceState::kOko:
		boss->ChangeState<TackleState>();
		break;
	case IBossState::FaceState::kNormal:
		boss->ChangeState<HipDrop>();
		break;
	case IBossState::FaceState::kSad:
		boss->ChangeState<RotateState>();
		break;
	case IBossState::FaceState::kBad:
		boss->ChangeState<HipDrop>();
		break;
	default:
		break;
	}
}
