#include "EscapeState.h"
#include "../Boss.h"
#include "../Player.h"
#include "Tackle.h"
#include "SlotState.h"

void EscapeState::Init() {
	changeTimer_.Start(5.f);
}

void EscapeState::Update([[maybe_unused]] const float deltaTime) {
	changeTimer_.Update(deltaTime);

	//// ステージのポインタ
	//const auto *const stage = IEntity::GetStage();
	//// マップチップ
	//const auto &mapData = stage->GetMapChip()->GetMapData();

	auto &bossTransform = GetBoss()->GetTransform();

	// プレイヤに対してのベクトル
	Vector3 toPlayer = GetPlayer()->GetPos() - bossTransform.translate_;

	// プレイヤから離れるベクトル
	Vector3 escapeVector = -toPlayer.Normalize() * deltaTime;
	// zベクトルを0に
	escapeVector.z = 0.f;

	// 離れる
	bossTransform.translate_ += escapeVector;

	if (changeTimer_.IsFinish()) {
		ChangeState();
	}
}

void EscapeState::ChangeState() {
	GetBoss()->ChangeState<SlotState>();
}

void EscapeState::OnCollision([[maybe_unused]] IEntity *other) {

	// ボスであった場合
	Player *player = dynamic_cast<Player *>(other);
	if (player) {
		Vector3 toPlayer = Vector3{ player->GetSphere().center_ - GetBoss()->GetTransform().translate_ }.Normalize();

		player->AddAcceleration(toPlayer * 5.f);

		GetBoss()->AddAcceleration(toPlayer * -0.5f);
	}
}
