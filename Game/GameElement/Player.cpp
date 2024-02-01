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

Player::Player() {

	IBossState::SetPlayer(this);
}

void Player::Init() {
	//barrier_ = std::make_unique<BarrierItem>();
	//barrier_->Init();
	//barrier_->SetParent(this);

	model_ = ModelDataManager::GetInstance()->LoadObj("Sphere");
	color_ = 0xFFFFFFFF;
	sphere_.radius_ = 0.4f;

	scale_ = Vector3::one * sphere_.radius_ * 2.f;
	velocity_ = {};
	acceleration_ = {};

	audio_ = Audio::GetInstance();

	jumpSE_ = audio_->LoadWave("Resources/playerSE/jump.wav");
	CalcTransMat();

	health_ = vMaxHealth_;

}

void Player::Update([[maybe_unused]] const float deltaTime) {
	//barrier_->Update(deltaTime);
	sphere_.center_.z = 0.f;
	CalcTransMat();
	if (GetState() != State::kFacing) {
		acceleration_.y -= 9.8f * deltaTime * 2.f;
	}
	//static const float kMaxAcceleration = 2.f;

	//for (uint8_t i = 0u; i < 3u; i++) {
	//	if (std::abs((&acceleration_.x)[i]) >= kMaxAcceleration) {
	//		(&acceleration_.x)[i] = std::clamp((&acceleration_.x)[i], -kMaxAcceleration, kMaxAcceleration);
	//	}
	//}

	invincibleTime_.Update(deltaTime);

	//if (GetState() == State::kMoving) {
	acceleration_ -= velocity_ * (0.6f / 60.f);
	//}

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

	blockManager->AddBox(model_, IBlock{ .transformMat_ = transformMat_,.color_ = color_ });
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
		inputRight = input->GetGamePadRStick();

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


	Vector2 inputLeft = input->GetGamePadLStick();
	if (inputLeft.x == 0.f) {
		inputLeft.x -= input->PressingKey(DIK_A);
		inputLeft.x += input->PressingKey(DIK_D);
	}
	if (inputLeft.x) {
		Move(inputLeft.x * 500.f, deltaTime * deltaTime);
	}

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
	transformMat_ = Matrix4x4::MakeAffinMatrix(scale_ * sphere_.radius_, rotate_, sphere_.center_);
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

void PlayerFacing::Init() {
	player_->velocity_ = {};
}

void PlayerFacing::Update(const float deltaTime) {
	player_->velocity_ = {};
}

void PlayerTackle::Init() {
	Vector2 inputDir = player_->preInputRStick_;
	const Vector3Norm input3d{ inputDir.x, inputDir.y, 0.f };
	player_->acceleration_ += input3d * 15.f;

	activeTime_.Start(player_->vAttackTime_);
}

void PlayerTackle::Update(const float deltaTime) {
	activeTime_.Update(deltaTime);
}

bool PlayerTackle::IsExit() const {
	return activeTime_.IsFinish();
}
