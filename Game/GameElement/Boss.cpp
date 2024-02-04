#include "Boss.h"
#include "BossState/EscapeState.h"

Boss::Boss() {
	slot_ = std::make_unique<Slot>();

	IBossState::SetBoss(this);

	//IBossState = 
}

void Boss::Init() {
	transform_.scale_ = Vector3{ 3.2f,1.8f,1.0f };
	transform_.rotate_ = Vector3{ 0.f, 180._deg, 0.f };
	transform_.translate_ = Vector3::zero;

	sphere_.radius_ = 1.f;

	if (slot_) {
		slot_->Initialize();
	}

	//bossState_ = std::make_unique<EscapeState>();
	//bossState_->Init();
	ChangeState<EscapeState>();

	CalcTransMat();
	CalcCollision();

	health_ = vMaxHealth_;

	mapChip_ = pStage_->GetMapChip();
	mapData_ = &mapChip_->GetMapData();

	acceleration_ = {};
	velocity_ = {};

}

void Boss::Update([[maybe_unused]] const float deltaTime) {

	// 空気抵抗
	acceleration_ -= velocity_ * (2.f / 60.f);

	velocity_ += acceleration_;
	transform_.translate_ += velocity_ * deltaTime;
	acceleration_ = {};

	if (nextState_) {
		bossState_ = std::move(nextState_);
		bossState_->Init();
	}
	if (bossState_) {
		bossState_->Update(deltaTime);

		// ステージのサイズに合わせて座標を調整
		const Vector2 stageSize{ static_cast<float>(mapData_->GetCols()) / 2.f - (transform_.scale_.x * 2.f + 1.5f), static_cast<float>(mapData_->GetRows()) / 2.f - (transform_.scale_.y * 2.f + 3.f) };

		// 画面内に収める処理
		transform_.translate_.x = std::clamp<float>(transform_.translate_.x, -stageSize.x, stageSize.x);
		transform_.translate_.y = std::clamp<float>(transform_.translate_.y, -stageSize.y, stageSize.y);
		// 前後の計算誤差の修正
		transform_.translate_.z = 0.f;

	}

	CalcTransMat();
	CalcCollision();

	slot_->Update(pCamera_);

}

void Boss::Draw() {
	slot_->Draw(pCamera_);
}

void Boss::OnCollision(IEntity *other) {
	if (bossState_) {
		bossState_->OnCollision(other);
	}

	IWeapon *weapon = dynamic_cast<IWeapon *>(other);
	if (weapon) {
		Damage(weapon);
	}

}

void Boss::Damage(IWeapon *weapon) {

	// 一度も当たってなかった場合ダメージ処理
	if (not weapon->GetIsHitBoss()) {
		Damage(weapon->GetDamage());

		weapon->SetIsHitBoss(true);
	}
}

void Boss::Damage(float damage) {
	health_ -= damage;
	pHPBar_->SetDamage(damage);
}

void Boss::SetHPBar(HPBar *hpBar) {
	pHPBar_ = hpBar;
	pHPBar_->SetMaxHP(vMaxHealth_);
}

void Boss::SetCamera(Camera *camera) {
	// 有効な値なら代入
	if (camera) {
		pCamera_ = camera;
		slot_->PostEffectWright(camera);
	}
}

void Boss::StartSlot() {
	// 回転の開始
	slot_->StartRotation();
}

void Boss::StopSlot() {
	slot_->StopRotation();
}

FaceType Boss::GetFaceType() const
{
	return slot_->GetFaceType();
}

void Boss::DownGrade() {
	slot_->DownLevel();
}

void Boss::CalcTransMat() {

	transform_.CalcTransMat();
	TransferData();

}

void Boss::CalcCollision() {

	sphere_.center_ = transform_.translate_;
	sphere_.center_.z = 0.f;

}

void Boss::TransferData() {
	auto &translate = slot_->GetTransform();
	translate.scale_ = transform_.scale_;
	translate.rotate_ = transform_.rotate_;
	translate.translate_ = transform_.translate_;
}
