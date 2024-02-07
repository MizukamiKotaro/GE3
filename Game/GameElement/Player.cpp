#include "Player.h"
#include "calc.h"
#include <ModelDataManager.h>
#include "BlockManager.h"
#include "MovingBall.h"
#include "MapChip.h"
#include "Audio.h"
#include "Punch.h"
#include "Needle.h"
#include "BossState/IBossState.h"
#include "Stage.h"
#include "SoUtils/Math/Math.h"
#include "Boss.h"

Player::Player() {

	IBossState::SetPlayer(this);

	playerModel_ = std::make_unique<BoneModel>();
	modelTransform_ = std::make_unique<std::array<BoneModel::SimpleTransform, 8u>>();
	modelMat_ = std::make_unique<std::array<Matrix4x4, 8u>>();
}

void Player::Init() {
	//barrier_ = std::make_unique<BarrierItem>();
	//barrier_->Init();
	//barrier_->SetParent(this);

	ModelDataManager *const modelManager = ModelDataManager::GetInstance();

	model_ = modelManager->LoadObj("Sphere");
	color_ = 0xFFFFFFFF;
	sphere_.radius_ = 0.5f;

	rotate_ = Vector3{ 0.f, SoLib::Math::Angle::PI2, 0.f };
	rotateTarget_ = Vector3{ 0.f, SoLib::Math::Angle::PI2, 0.f };

	scale_ = Vector3::one * sphere_.radius_ * 2.f;
	velocity_ = {};
	acceleration_ = {};

	audio_ = Audio::GetInstance();

	jumpSE_ = audio_->LoadWave("Resources/playerSE/jump.wav");
	CalcTransMat();

	{
		playerModel_->Init();
		auto body = playerModel_->AddBone("Body", modelManager->LoadObj("playerBody"));
		auto eye = playerModel_->AddBone("Eye", body);
		eye->AddBox(modelManager->LoadObj("playerEyeL"));
		eye->AddBox(modelManager->LoadObj("playerEyeR"));

		playerModel_->AddBone("Mouth", modelManager->LoadObj("playerMouth"), body);

		playerModel_->SetNumber();

		//modelTransform_->at(playerModel_->GetIndex("Eye", 0)).scale_ = Vector3::one * 5.f;

	}

	health_ = vMaxHealth_;

}

void Player::Update([[maybe_unused]] const float deltaTime) {
	//barrier_->Update(deltaTime);
	sphere_.center_.z = 0.f;
	CalcTransMat();
	*modelMat_ = playerModel_->CalcTransMat(*modelTransform_, &transformMat_);
	if (GetState() != State::kFacing) {
		acceleration_.y -= 9.8f * deltaTime * 2.f;
	}
	//static const float kMaxAcceleration = 2.f;

	//for (uint8_t i = 0u; i < 3u; i++) {
	//	if (std::abs((&acceleration_.x)[i]) >= kMaxAcceleration) {
	//		(&acceleration_.x)[i] = std::clamp((&acceleration_.x)[i], -kMaxAcceleration, kMaxAcceleration);
	//	}
	//}

	//static const Quaternion rotLeft = Quaternion::LookAt(-Vector3::right);

	rotate_ = SoLib::Lerp(rotate_, rotateTarget_, 0.2f);

	invincibleTime_.Update(deltaTime);

	// 空気抵抗
	acceleration_ -= velocity_ * (0.6f / 60.f);

	if (playerState_) {
		playerState_->Update(deltaTime);

		if (playerState_->IsExit()) {
			playerState_ = nullptr;
		}
	}

	// 移動処理を行う
	UpdateRigidbody(deltaTime);

	// 移動処理の更新
	sphere_.center_ = mapData_->HitMap(beforePos_, sphere_.center_, 1.f);

	if (beforePos_.y == sphere_.center_.y && velocity_.y < 0.f && not isLanding_) {
		Landing();
	}

	if (beforePos_.y != sphere_.center_.y) {
		isLanding_ = false;
	}

	for (uint8_t i = 0u; i < 3u; i++) {
		if ((&beforePos_.x)[i] == (&sphere_.center_.x)[i]) {
			(&velocity_.x)[i] *= -kReflectionPower_;
		}
	}
	const auto &mapArray = pStage_->GetMapChip()->GetMapData();
	const Vector2 stageSize = Vector2{ static_cast<float>(mapArray.GetCols()) - 9.5f, static_cast<float>(mapArray.GetRows()) - 10.5f } *0.5f;

	for (uint32_t i = 0u; i < 2u; i++) {
		if (std::abs((&sphere_.center_.x)[i]) > (&stageSize.x)[i]) {
			(&velocity_.x)[i] *= -kReflectionPower_;
		}
		(&sphere_.center_.x)[i] = std::clamp((&sphere_.center_.x)[i], -(&stageSize.x)[i], (&stageSize.x)[i]);
	}

}

void Player::Draw() {
	static auto *const blockManager = BlockManager::GetInstance();

	//barrier_->Draw();
	playerModel_->Draw(*modelMat_);

	//blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_,.color_ = color_ });
}

void Player::SetHPBar(HPBar *hpBar) {
	pHPBar_ = hpBar;

	pHPBar_->SetMaxHP(vMaxHealth_);
}

void Player::InputAction(Input *const input, const float deltaTime) {
	Vector2 inputRight;

	inputRight.x = static_cast<float>(input->PressingKey(DIK_RIGHT) - input->PressingKey(DIK_LEFT));
	inputRight.y = static_cast<float>(input->PressingKey(DIK_UP) - input->PressingKey(DIK_DOWN));

	if (Calc::MakeLength(inputRight) <= 0.f) {
		inputRight.x = static_cast<float>(input->PressingKey(DIK_D) - input->PressingKey(DIK_A));
		inputRight.y = static_cast<float>(input->PressingKey(DIK_W) - input->PressingKey(DIK_S));
	}

	if (Calc::MakeLength(inputRight) <= 0.f) {
		inputRight = input->GetGamePadRStick();

	}
	if (Calc::MakeLength(inputRight) <= 0.f) {
		inputRight = input->GetGamePadLStick();

	}

	// 入力があったら
	if (Calc::MakeLength(inputRight) > 0.5f) {
		// 状態が存在しないなら変更
		if (not playerState_) {
			SetPlayerState<PlayerFacing>();
		}
	}
	else {
		// 現在の状態が facingStateであるか
		auto facingState = dynamic_cast<PlayerFacing *>(playerState_.get());
		// そうであれば変更
		if (facingState) {
			SetPlayerState<PlayerTackle>();
		}
	}


	/*Vector2 inputLeft = input->GetGamePadLStick();
	if (inputLeft.x == 0.f) {
		inputLeft.x -= input->PressingKey(DIK_A);
		inputLeft.x += input->PressingKey(DIK_D);
	}
	if (inputLeft.x) {
		Move(inputLeft.x * 500.f, deltaTime * deltaTime);
	}*/

	/*if (isLanding_ && (input->PressingKey(DIK_SPACE) || input->PressingGamePadButton(Input::GamePadButton::A) || input->PressingGamePadButton(Input::GamePadButton::RIGHT_SHOULDER))) {
		acceleration_.y += 15.f;
		audio_->Play(jumpSE_, false, 0.6f);
	}*/

	// 0.5以上の入力があった場合
	if (Calc::MakeLength(inputRight) > 0.5f) {
		preInputRStick_ = inputRight;
	}
}

void Player::Move(const float power, [[maybe_unused]] const float deltaTime) {
	acceleration_.x += power * deltaTime;
}



void Player::UpdateRigidbody([[maybe_unused]] const float deltaTime) {
	beforePos_ = sphere_.center_;

	velocity_ += acceleration_;
	acceleration_ = {};
	/*for (uint32_t i = 0u; i < 2u; i++) {
		(&velocity_.x)[i] = std::clamp((&velocity_.x)[i], -(&kMaxSpeed_.x)[i], (&kMaxSpeed_.x)[i]);
	}*/
	Vector3 fixVelocity = velocity_ * deltaTime;


	sphere_.center_ += fixVelocity;
}

void Player::SetMapChip(MapChip *const mapChip) {
	mapData_ = mapChip;
}

void Player::SetBallList(std::list<std::unique_ptr<MovingBall>> *ballList) {
	//barrier_->SetBallList(ballList);
	ballList_ = ballList;
}

void Player::Landing() {
	isLanding_ = true;

	//for (uint8_t i = 0u; i < 2u; i++) {
	//	ballList_->push_back(std::make_unique<MovingBall>());
	//	auto ball = ballList_->back().get();
	//	ball->Init();
	//	ball->SetPos(sphere_.center_ + Vector3{ .x = -1.f + 2.f * i });
	//	ball->SetVelocity(Vector3{ .y = 10.f });
	//}
}

void Player::AttackStart() {
}

void Player::CalcTransMat() {
	transformMat_ = SoLib::Math::Affine(scale_ * sphere_.radius_, rotate_, sphere_.center_);
}

bool Player::Damage([[maybe_unused]] IWeapon *weapon) {

	return Damage(weapon->GetDamage());

}

bool Player::Damage(float damage) {

	// 無敵時間が終わっていたら
	if (invincibleTime_.IsFinish()) {
		// 無敵時間を開始
		invincibleTime_.Start(1.5f);

		// 体力を減らす
		health_ -= damage;

		pHPBar_->SetDamage(damage);

		return true;

	}

	return false;
}

void Player::OnCollision(IEntity *other) {
	IWeapon *weapon = dynamic_cast<IWeapon *>(other);
	if (weapon) {
		Damage(weapon);
	}

	Hole *hole = dynamic_cast<Hole *>(other);
	if (hole) {
		ReflectHole(*hole);
	}

}

Player::State Player::GetState() const {
	if (playerState_) {
		return playerState_->GetState();
	}
	return State::kMoving;
}

bool Player::ImGuiWidget() {
	bool isChange = false;

#ifdef _DEBUG

	isChange |= SoLib::ImGuiWidget(&vMaxHealth_);
	isChange |= SoLib::ImGuiWidget(&vAttackTime_);

#endif // _DEBUG

	return isChange;
}

void Player::ReflectHole(const Hole &hole)
{

	Vector3 normalVec = (this->sphere_.center_ - hole.GetPos());
	normalVec.z = 0.f;

	// 法線の向きがvelocityと逆であれば反射する
	if (Calc::Dot(velocity_, normalVec) < 0.f) {
		velocity_ = velocity_.Reflect(normalVec.Normalize(), hole.GetReflectionPow());
	}
}

void PlayerFacing::Init() {
	player_->velocity_ = {};
}

void PlayerFacing::Update(const float deltaTime) {
	player_->acceleration_ = {};
	Vector2 inputDir = player_->preInputRStick_;
	bool isLeft = false;

	// 向いている左右方向
	if (inputDir.x > 0.f) {
		player_->rotateTarget_.y = SoLib::Math::Angle::PI;
	}
	else {
		player_->rotateTarget_.y = SoLib::Math::Angle::PI2;
		isLeft = true;
	}

	// 向き統一
	inputDir.x = std::abs(inputDir.x);
	// 回転作成
	player_->rotateTarget_.z = std::atan2(inputDir.y, inputDir.x);

	if (isLeft) {
		// 上下反転
		player_->rotateTarget_.z *= -1.f;
	}

}

void PlayerTackle::Init() {
	Vector2 inputDir = player_->preInputRStick_;
	const Vector3Norm input3d{ inputDir.x, inputDir.y, 0.f };
	player_->acceleration_ += input3d * 15.f;

	activeTime_.Start(player_->vAttackTime_);

	// 向いてる上下方向
	player_->rotateTarget_.z = 0.f;
}

void PlayerTackle::Update(const float deltaTime) {
	activeTime_.Update(deltaTime);
}

bool PlayerTackle::IsExit() const {
	return activeTime_.IsFinish();
}
