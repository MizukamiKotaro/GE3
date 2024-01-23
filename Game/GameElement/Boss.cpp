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

	bossState_ = std::make_unique<EscapeState>();
	bossState_->Init();

	CalcTransMat();
	CalcCollision();

}

void Boss::Update([[maybe_unused]] const float deltaTime) {

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
