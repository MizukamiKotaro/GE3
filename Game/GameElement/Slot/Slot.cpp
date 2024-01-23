#include "Slot.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Externals/imgui/imgui.h"
#include "RandomGenerator/RandomGenerator.h"
#include <numbers>
#include "RandomGenerator/RandomGenerator.h"
#include "Input.h"
#include <algorithm>
#include <numbers>
#include "FrameInfo/FrameInfo.h"

Slot::Slot()
{
	back_ = std::make_unique<Sprite>("Resources/white.png");
	back_->size_ = { 1280.0f,720.0f };
	back_->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	back_->Update();

	plane_ = std::make_unique<Model>("plane");
	plane_->UnUsedLight();
	plane_->transform_.rotate_.y = std::numbers::pi_v<float>;
	plane_->transform_.scale_ = { 3.2f,1.8f,1.0f };
	plane_->Update();

	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Update();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			slot_[i][j] = std::make_unique<Sprite>("Resources/white.png");
		}

		blurs_[i] = std::make_unique<Blur>();
		blurs_[i]->blurData_->angle = std::numbers::pi_v<float> / 2.0f;
		blurs_[i]->blurData_->isCenterBlur = 0;

		backs_[i] = std::make_unique<Sprite>("Resources/white.png");
	}

	TextureManager* texMan = TextureManager::GetInstance();
	slot_[0][1]->SetTextureHandle(texMan->LoadTexture("Resources/slot/a1.png"));
	slot_[1][1]->SetTextureHandle(texMan->LoadTexture("Resources/slot/a2.png"));
	slot_[2][1]->SetTextureHandle(texMan->LoadTexture("Resources/slot/a3.png"));
	slot_[0][0]->SetTextureHandle(texMan->LoadTexture("Resources/slot/b1.png"));
	slot_[1][0]->SetTextureHandle(texMan->LoadTexture("Resources/slot/b2.png"));
	slot_[2][0]->SetTextureHandle(texMan->LoadTexture("Resources/slot/b3.png"));
	slot_[0][2]->SetTextureHandle(texMan->LoadTexture("Resources/slot/c1.png"));
	slot_[1][2]->SetTextureHandle(texMan->LoadTexture("Resources/slot/c2.png"));
	slot_[2][2]->SetTextureHandle(texMan->LoadTexture("Resources/slot/c3.png"));

	interval_ = 200.0f;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			slot_[i][j]->size_ *= 3.0f;
			slot_[i][j]->pos_ = { 640.0f + slot_[i][j]->size_.x * (-1 + i), 360.0f + (slot_[i][j]->size_.y + interval_) * (-1 + j) };
			slot_[i][j]->Update();
		}
		isRotStop_[i] = true;

		backs_[i]->size_ = { slot_[i][0]->size_.x,720.0f };
		backs_[i]->pos_.x = slot_[i][0]->pos_.x;
		backs_[i]->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		backs_[i]->Update();
	}
	DownToTop();

	plane_->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());

	limitSpeed_ = 20.0f;

	//globalVariable_ = GlobalVariables::GetInstance();

	SetGlobalVariable();
}

void Slot::Initialize() {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			slot_[i][j]->pos_ = { 640.0f + slot_[i][j]->size_.x * (-1 + i), 360.0f + (slot_[i][j]->size_.y + interval_) * (-1 + j) };
			slot_[i][j]->Update();
		}
		isRotStop_[i] = true;
	}
	SetGlobalVariable();
	DownToTop();
}

void Slot::Update(Camera* camera) {

	Input* input = Input::GetInstance();


#ifdef _DEBUG
	if (input->PressedKey(DIK_T)) {
		StartRotation();
	}
	if (input->PressedKey(DIK_Y)) {
		StopRotation();
	}
	if (input->PressedKey(DIK_U)) {
		DownLevel();
	}
	ApplyGlobalVariable();

#endif // _DEBUG

	//Rotation();
	Rotation2();

	plane_->Update();

	DownToTop();

	PostEffectWright(camera);
}

void Slot::Draw(Camera* camera) {

	plane_->Draw(*camera);
}

void Slot::StartRotation()
{
	isRot_ = true;
	isStop_ = false;

	RandomGenerator* rand = RandomGenerator::GetInstance();
	timeCount_ = 0.0f;

	faceType_ = FaceType::kOko;

	for (int i = 0; i < 3; i++) {
		isRotStop_[i] = false;
		rotSpeed_[i] = rand->RandFloat(70.0f, 100.0f);
	}
}

void Slot::StopRotation()
{
	isStop_ = true;
}

void Slot::DownLevel()
{
	if (faceType_ != FaceType::kBad && faceType_ != FaceType::kGekiOko) {
		isRot_ = true;
		isStop_ = false;
		timeCount_ = 0.0f;

		RandomGenerator* rand = RandomGenerator::GetInstance();

		if (faceType_ == FaceType::kOko) {
			faceType_ = FaceType::kNormal;
		}
		else if (faceType_ == FaceType::kNormal) {
			faceType_ = FaceType::kSad;
		}
		else if (faceType_ == FaceType::kSad) {
			faceType_ = FaceType::kBad;
		}

		for (int i = 0; i < 3; i++) {
			isRotStop_[i] = false;
			rotSpeed_[i] = rand->RandFloat(70.0f, 100.0f);
		}
	}
}

void Slot::StartGekiOkoRotation()
{
	isRot_ = true;
	isStop_ = false;

	RandomGenerator* rand = RandomGenerator::GetInstance();

	faceType_ = FaceType::kGekiOko;
	timeCount_ = 0.0f;

	for (int i = 0; i < 3; i++) {
		isRotStop_[i] = false;
		rotSpeed_[i] = rand->RandFloat(70.0f, 100.0f);
	}
}

void Slot::SetGlobalVariable()
{
	/*v2Info_[EnumV2Info::Scale] = { plane_->transform_.scale_.x,plane_->transform_.scale_.y };

	v3Info_[EnumV3Info::Pos] = plane_->transform_.translate_;

	globalVariable_->CreateGroup(groupName_);

	for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		globalVariable_->AddItem(groupName_, v2ItemNames[i], v2Info_[i]);
	}

	for (int i = 0; i < EnumV3Info::V3EndCount; i++) {
		globalVariable_->AddItem(groupName_, v3ItemNames[i], v3Info_[i]);
	}

	ApplyGlobalVariable();*/
}

void Slot::ApplyGlobalVariable()
{
	/*for (int i = 0; i < EnumV2Info::V2EndCount; i++) {
		v2Info_[i] = globalVariable_->GetVector2Value(groupName_, v2ItemNames[i]);
	}

	for (int i = 0; i < EnumV3Info::V3EndCount; i++) {
		v3Info_[i] = globalVariable_->GetVector3Value(groupName_, v3ItemNames[i]);
	}

	plane_->transform_.scale_.x = v2Info_[EnumV2Info::Scale].x;
	plane_->transform_.scale_.y = v2Info_[EnumV2Info::Scale].y;

	plane_->transform_.translate_ = v3Info_[EnumV3Info::Pos];*/
}

void Slot::Rotation()
{
	if (isRot_) {

		for (int i = 0; i < 3; i++) {
			if (!isRotStop_[i]) {
				if (rotSpeed_[i] > limitSpeed_) {
					rotSpeed_[i] -= 0.2f;

					float t = std::clamp<float>(rotSpeed_[i], limitSpeed_ + 5.0f, 65.0f) - (limitSpeed_ + 5.0f);
					t = t / (60.0f - limitSpeed_);

					blurs_[i]->blurData_->pickRange = t * 0.14f + (1.0f - t) * 0.01f;
					blurs_[i]->blurData_->stepWidth = t * 0.04f + (1.0f - t) * 0.005f;
				}

				isAcross_[i] = false;

				for (int j = 0; j < 3; j++) {

					float posY = slot_[i][j]->pos_.y;
					slot_[i][j]->pos_.y += rotSpeed_[i];

					if (posY <= 360.0f && slot_[i][j]->pos_.y >= 360.0f) {
						isAcross_[i] = true;
						acrossNum_ = j;
					}
				}

				if (rotSpeed_[i] < limitSpeed_ || isStop_) {
					if (i == 0 && !isRotStop_[0]) {
						if (isAcross_[i]) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}

							isRotStop_[i] = true;
							faceType_ = acrossNum_;
						}
					}
					else if (isRotStop_[0] && i == 2) {
						if (isAcross_[i] && acrossNum_ == faceType_) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}

							isRotStop_[i] = true;
						}
					}
					else if (isRotStop_[2]) {
						if (isAcross_[i] && acrossNum_ == faceType_) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}
							isRotStop_[i] = true;
							isRot_ = false;
						}
					}
				}
			}

			for (int j = 0; j < 3; j++) {
				slot_[i][j]->Update();
			}
		}


	}
}

void Slot::Rotation2()
{
	if (isRot_) {
		
		if (faceType_ == FaceType::kBad) {
			// はずれの場合

			timeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

			for (int i = 0; i < 3; i++) {
				if (!isRotStop_[i]) {
					float t = std::clamp<float>(rotSpeed_[i], limitSpeed_ + 5.0f, 65.0f) - (limitSpeed_ + 5.0f);
					t = t / (60.0f - limitSpeed_);

					blurs_[i]->blurData_->pickRange = t * 0.14f + (1.0f - t) * 0.01f;
					blurs_[i]->blurData_->stepWidth = t * 0.04f + (1.0f - t) * 0.005f;

					isAcross_[i] = false;

					for (int j = 0; j < 3; j++) {

						float posY = slot_[i][j]->pos_.y;
						slot_[i][j]->pos_.y += rotSpeed_[i];

						if (posY <= 360.0f && slot_[i][j]->pos_.y >= 360.0f) {
							isAcross_[i] = true;
							acrossNum_ = j;
						}
					}

					if (timeCount_ >= 0.2f && i == 0 && !isRotStop_[0] && isAcross_[i]) {
						float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

						for (int j = 0; j < 3; j++) {
							slot_[i][j]->pos_.y += y;
						}

						isRotStop_[i] = true;
						faceTypes_[i] = acrossNum_;
						timeCount_ = 0.0f;
					}
					else if (timeCount_ >= 0.2f && isRotStop_[0] && i == 2 && isAcross_[i]) {
						if (isAcross_[i] && acrossNum_ != faceTypes_[0]) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}
							faceTypes_[i] = acrossNum_;
							isRotStop_[i] = true;
							timeCount_ = 0.0f;
						}
					}
					else if (isRotStop_[2] && timeCount_ >= 0.2f) {
						if (isAcross_[i] && acrossNum_ != faceTypes_[0] && acrossNum_ != faceTypes_[2]) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}
							isRotStop_[i] = true;
							isRot_ = false;
							timeCount_ = 0.0f;
						}
					}
				}

				for (int j = 0; j < 3; j++) {
					slot_[i][j]->Update();
				}
			}
		}
		else if (faceType_ == FaceType::kGekiOko) {
			// 激おこの場合
		}
		else {
			// その他の場合

			timeCount_ += FrameInfo::GetInstance()->GetDeltaTime();

			for (int i = 0; i < 3; i++) {
				if (!isRotStop_[i]) {
					float t = std::clamp<float>(rotSpeed_[i], limitSpeed_ + 5.0f, 65.0f) - (limitSpeed_ + 5.0f);
					t = t / (60.0f - limitSpeed_);

					blurs_[i]->blurData_->pickRange = t * 0.14f + (1.0f - t) * 0.01f;
					blurs_[i]->blurData_->stepWidth = t * 0.04f + (1.0f - t) * 0.005f;

					isAcross_[i] = false;

					for (int j = 0; j < 3; j++) {

						float posY = slot_[i][j]->pos_.y;
						slot_[i][j]->pos_.y += rotSpeed_[i];

						if (j == faceType_ && posY <= 360.0f && slot_[i][j]->pos_.y >= 360.0f) {
							isAcross_[i] = true;
							acrossNum_ = j;
						}
					}

					if (timeCount_ >= 1.0f && i == 0 && !isRotStop_[0] && isAcross_[i]) {
						float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

						for (int j = 0; j < 3; j++) {
							slot_[i][j]->pos_.y += y;
						}

						isRotStop_[i] = true;
						faceType_ = acrossNum_;
						timeCount_ = 0.0f;
					}
					else if (timeCount_ >= 0.3f && isRotStop_[0] && i == 2 && isAcross_[i]) {
						if (isAcross_[i] && acrossNum_ == faceType_) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}

							isRotStop_[i] = true;
							timeCount_ = 0.0f;
						}
					}
					else if (isRotStop_[2] && isStop_) {
						if (isAcross_[i] && acrossNum_ == faceType_) {
							float y = 360.0f - slot_[i][acrossNum_]->pos_.y;

							for (int j = 0; j < 3; j++) {
								slot_[i][j]->pos_.y += y;
							}
							isRotStop_[i] = true;
							isRot_ = false;
							timeCount_ = 0.0f;
						}
					}
				}

				for (int j = 0; j < 3; j++) {
					slot_[i][j]->Update();
				}
			}
		}
	}
}

void Slot::PostEffectWright(Camera* camera)
{
	if (isRot_) {
		for (int i = 0; i < 3; i++) {

			if (blurs_[i]->blurData_->pickRange > 0.01f && !isRotStop_[i]) {
				blurs_[i]->PreDrawScene();

				backs_[i]->Draw();
				for (int j = 0; j < 3; j++) {
					slot_[i][j]->Draw(*camera);
				}

				blurs_[i]->PostDrawScene();
			}
		}
	}

	postEffect_->PreDrawScene();

	back_->Draw(*camera);

	for (int i = 0; i < 3; i++) {

		if (isRot_ && blurs_[i]->blurData_->pickRange > 0.01f && !isRotStop_[i]) {
			blurs_[i]->Draw();
		}
		for (int j = 0; j < 3; j++) {
			slot_[i][j]->Draw(*camera);
		}
	}


	postEffect_->PostDrawScene();
}

void Slot::DownToTop()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {

			if (slot_[i][j]->pos_.y >= 720.0f + slot_[i][j]->size_.y / 2.0f) {
				slot_[i][j]->pos_.y -= (slot_[i][j]->size_.y + interval_) * 3.0f;
				slot_[i][j]->Update();
			}
		}
	}
}
