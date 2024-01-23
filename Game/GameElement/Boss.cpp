#include "Boss.h"

Boss::Boss() {
	slot_ = std::make_unique<Slot>();
}

void Boss::Init() {
	scale_ = Vector3{ 3.2f,1.8f,1.0f };
	rotate_ = Vector3{ 0.f, 180._deg, 0.f };
	translate_ = Vector3::zero;

	sphere_.radius_ = 1.f;

	if (slot_) {
		slot_->Initialize();
	}

	CalcTransMat();
	CalcCollision();

}

void Boss::Update([[maybe_unused]] const float deltaTime) {

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

	transMat_ = Matrix4x4::MakeAffinMatrix(scale_, rotate_, translate_);
	TransferData();

}

void Boss::CalcCollision() {

	sphere_.center_ = translate_;
	sphere_.center_.z = 0.f;

}

void Boss::TransferData() {
	auto &translate = slot_->GetTransform();
	translate.scale_ = scale_;
	translate.rotate_ = rotate_;
	translate.translate_ = translate_;
}
