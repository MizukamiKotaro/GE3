#include "EscapeState.h"
#include "../Boss.h"
#include "../Player.h"

void EscapeState::Init() {

}

void EscapeState::Update([[maybe_unused]] const float deltaTime) {

	// ステージのポインタ
	const auto *const stage = IEntity::GetStage();
	// マップチップ
	const auto &mapData = stage->GetMapChip()->GetMapData();

	auto &bossTransform = GetBoss()->GetTransform();

	// プレイヤに対してのベクトル
	Vector3 toPlayer = GetPlayer()->GetGrobalPos() - bossTransform.translate_;

	// プレイヤから離れるベクトル
	Vector3 escapeVector = -toPlayer.Normalize() * deltaTime;
	// zベクトルを0に
	escapeVector.z = 0.f;

	// 離れる
	bossTransform.translate_ += escapeVector;

	bossTransform.translate_.x = std::clamp<float>(bossTransform.translate_.x, -static_cast<float>(mapData.GetCols()) / 2.f + (bossTransform.scale_.x * 2.f + 1.5f), static_cast<float>(mapData.GetCols()) / 2.f - (bossTransform.scale_.x * 2.f + 1.5f));
	bossTransform.translate_.y = std::clamp<float>(bossTransform.translate_.y, -static_cast<float>(mapData.GetRows()) / 2.f + (bossTransform.scale_.y * 2.f + 3.f), static_cast<float>(mapData.GetRows()) / 2.f - (bossTransform.scale_.y * 2.f + 3.f));
}
