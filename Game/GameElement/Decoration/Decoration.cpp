#include "Decoration.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ModelDataManager.h"
#include <cmath>

Decoration::Decoration()
{
	decrations_[Sword] = std::make_unique<Model>("decoration");
	decrations_[Sword]->UnUsedLight();
	decrations_[Sword]->transform_.translate_ = { 13.6f,5.8f,5.0f };
	decrations_[Sword]->transform_.scale_ = { 1.0f,8.0f,8.0f };
	decrations_[Sword]->transform_.rotate_.y = -1.57f;
	decrations_[Sword]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	decrations_[Sword]->Update();

	decrations_[SwordB] = std::make_unique<Model>("decorationB");
	decrations_[SwordB]->UnUsedLight();
	decrations_[SwordB]->transform_.translate_ = { -13.6f,5.8f,5.0f };
	decrations_[SwordB]->transform_.scale_ = { 1.0f,8.0f,8.0f };
	decrations_[SwordB]->transform_.rotate_.y = -1.57f;
	decrations_[SwordB]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	decrations_[SwordB]->Update();

	decrations_[Punch] = std::make_unique<Model>("decoration2");
	decrations_[Punch]->UnUsedLight();
	decrations_[Punch]->transform_.translate_ = { 13.6f,-7.8f,5.0f };
	decrations_[Punch]->transform_.scale_ = { 1.0f,8.0f,8.0f };
	decrations_[Punch]->transform_.rotate_.y = -1.57f;
	decrations_[Punch]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	decrations_[Punch]->Update();

	decrations_[PunchB] = std::make_unique<Model>("decoration2B");
	decrations_[PunchB]->UnUsedLight();
	decrations_[PunchB]->transform_.translate_ = { -13.6f,-7.8f,5.0f };
	decrations_[PunchB]->transform_.scale_ = { 1.0f,8.0f,8.0f };
	decrations_[PunchB]->transform_.rotate_.y = -1.57f;
	decrations_[PunchB]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	decrations_[PunchB]->Update();

	decrations_[Needle] = std::make_unique<Model>("decoration1");
	decrations_[Needle]->UnUsedLight();
	decrations_[Needle]->transform_.translate_ = { 0.0f,-15.8f,5.0f };
	decrations_[Needle]->transform_.scale_ = { 1.0f,8.0f,8.0f };
	decrations_[Needle]->transform_.rotate_.y = -1.57f;
	decrations_[Needle]->SetColor({ 0.2f,0.2f,0.2f,1.0f });
	decrations_[Needle]->Update();

	for (int i = 0; i < 6; i++) {
		numberModelDatas_[i] = ModelDataManager::GetInstance()->LoadObj(std::to_string(i));
	}

	float radius = 1.3f;
	float angle = 3.14f / kMaxGauge_ * 2;

	for (int i = 0; i < EndModelType; i++) {
		nums_[i] = kMaxGauge_;

		numbers_[i] = std::make_unique<Model>(numberModelDatas_[nums_[i]]);
		numbers_[i]->UnUsedLight();
		numbers_[i]->transform_.rotate_.y = -3.14f;
		numbers_[i]->transform_.scale_ = { 2.0f,2.0f,1.0f };

		if (i == SwordB || i == PunchB) {
			numbers_[i]->transform_.translate_ = decrations_[i]->transform_.translate_ + Vector3{ -6.0f,3.0f,-2.0f };
		}
		else {
			numbers_[i]->transform_.translate_ = decrations_[i]->transform_.translate_ + Vector3{ 4.0f,3.0f,-2.0f };
		}

		numbers_[i]->SetColor({ 1.0f,0.14f,0.31f,1.0f });
		numbers_[i]->Update();


		for (int j = 0; j < kMaxGauge_; j++) {
			gauges_[i][j] = std::make_unique<Model>("gauge");
			gauges_[i][j]->UnUsedLight();
			gauges_[i][j]->SetColor({ 0.1f,1.0f,0.1f,1.0f });
			gauges_[i][j]->transform_.rotate_.y = -3.14f;
			if (kMaxGauge_ == 5) {
				gauges_[i][j]->transform_.rotate_.z = angle * 2 - angle * j;
			}
			else {
				gauges_[i][j]->transform_.rotate_.z = angle - angle * j;
			}
			gauges_[i][j]->transform_.scale_ = { 0.4f,0.4f,1.0f };
			gauges_[i][j]->transform_.translate_ = numbers_[i]->transform_.translate_ +
				Vector3(radius * std::sinf(gauges_[i][j]->transform_.rotate_.z), -radius * std::cosf(gauges_[i][j]->transform_.rotate_.z), 1.0f);
			gauges_[i][j]->Update();
		}
	}

	rainbow_ = std::make_unique<Sprite>("Resources/rainbow.png");
	texcoodY_ = 0.0f;

	highLumi_ = std::make_unique<HighLumi>();
	highLumi_->highLumiData_->min = 0.01f;
	highLumi_->highLumiData_->isToWhite = true;

	//gaussian_ = std::make_unique<GaussianBlur>();

	post_ = std::make_unique<PostEffect>();
	isFirst_ = true;
}

void Decoration::Initialize()
{
	texcoodY_ = 0.0f;
	isFirst_ = true;
}

void Decoration::Update(Camera* camera)
{
#ifdef _DEBUG

	/*ImGui::Begin("DecorationObj");
	ImGui::DragFloat3("スケール", &punchDecration_->transform_.scale_.x, 0.01f);
	ImGui::DragFloat3("ポジション", &punchDecration_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("回転", &punchDecration_->transform_.rotate_.x, 0.01f);
	ImGui::End();*/

#endif // _DEBUG

	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	for (int i = 0; i < EndModelType; i++) {
		//decrations_[i]->Update();
		//numbers_[i]->Update();
		//denominators_[i]->Update();
		//slashes_[i]->Update();
	}

	/*gaussian_->gaussianBlurData_->pickRange = 0.01f;
	gaussian_->gaussianBlurData_->stepWidth = 0.0025f;*/
	ChangeGaugeColor();

	WrightPostEffect(camera);
}

void Decoration::tcUpdate(Camera* camera)
{
	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	for (int i = 0; i < EndModelType; i++) {
		//decrations_[i]->Update();
		//numbers_[i]->Update();
		//denominators_[i]->Update();
		//slashes_[i]->Update();
	}

	/*gaussian_->gaussianBlurData_->pickRange = 0.01f;
	gaussian_->gaussianBlurData_->stepWidth = 0.0025f;*/
	ChangeGaugeColor();

	Wright(camera);
}

void Decoration::Draw(Camera* camera)
{
	for (int i = 0; i < EndModelType; i++) {
		decrations_[i]->Draw(*camera);
		//numbers_[i]->Draw(*camera);
		/*for (int j = 0; j < kMaxGauge_; j++) {
			gauges_[i][j]->Draw(*camera);
		}*/
	}

	if (*isSword_ || *isPunch_ || *isNeedle_) {
		post_->Draw();

		//gaussian_->Draw(BlendMode::kBlendModeAdd);
	}

	//for (int i = 0; i < EndModelType; i++) {
	//	//numbers_[i]->Draw(*camera);
	//	for (int j = 0; j < kMaxGauge_; j++) {
	//		gauges_[i][j]->Draw(*camera);
	//	}
	//}
}

void Decoration::tcDraw(Camera* camera)
{
	for (int i = 0; i < EndModelType; i++) {
		decrations_[i]->Draw(*camera);
	}

	post_->Draw();
}

void Decoration::WrightPostEffect(Camera* camera)
{
	if (*isSword_ || *isPunch_ || *isNeedle_ || isFirst_) {
		isFirst_ = false;

		highLumi_->PreDrawScene();

		if (*isSword_) {
			if (*isRight_) {
				decrations_[Sword]->Draw(*camera);
				//numbers_[Sword]->Draw(*camera);
			}
			else {
				decrations_[SwordB]->Draw(*camera);
				//numbers_[SwordB]->Draw(*camera);
			}
		}
		if (*isPunch_) {
			if (*isRight_) {
				decrations_[Punch]->Draw(*camera);
				//numbers_[Punch]->Draw(*camera);
			}
			else {
				decrations_[PunchB]->Draw(*camera);
				//numbers_[PunchB]->Draw(*camera);
			}
		}
		if (*isNeedle_) {
			decrations_[Needle]->Draw(*camera);
			//numbers_[Needle]->Draw(*camera);
		}

		highLumi_->PostDrawScene();

		post_->PreDrawScene();

		rainbow_->Draw();

		highLumi_->Draw(BlendMode::kBlendModeMultiply);

		post_->PostDrawScene();

		/*gaussian_->PreDrawScene();

		post_->Draw();

		gaussian_->PostDrawScene();*/
	}
}

void Decoration::Wright(Camera* camera)
{
	highLumi_->PreDrawScene();

	for (int i = 0; i < ModelType::EndModelType; i++) {
		decrations_[i]->Draw(*camera);
	}

	highLumi_->PostDrawScene();

	post_->PreDrawScene();

	rainbow_->Draw();

	highLumi_->Draw(BlendMode::kBlendModeMultiply);

	post_->PostDrawScene();
}

void Decoration::ChangeGaugeColor()
{
	for (int i = 0; i < EndModelType; i++) {
		int num = kMaxGauge_ - nums_[i];
		for (int j = 0; j < kMaxGauge_; j++) {
			if (nums_[i] == 0) {
				gauges_[i][j]->SetColor({1.0f,0.14f,0.31f,1.0f});
			}
			else {
				if (i >= num) {
					float t = float(num) / (kMaxGauge_ - 1);
					gauges_[i][j]->SetColor({ (1.0f - t) * 0.1f + t * 1.0f ,(1.0f - t) * 1.0f + t * 0.1f,0.1f,1.0f });
				}
				else {
					gauges_[i][j]->SetColor({ 1.0f,0.14f,0.31f,1.0f });
				}
			}
		}
	}
}
