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

}

void Boss::Update([[maybe_unused]] const float deltaTime) {

	if (nextState_) {
		bossState_ = std::move(nextState_);
		bossState_->Init();
	}
	if (bossState_) {
		bossState_->Update(deltaTime);
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
