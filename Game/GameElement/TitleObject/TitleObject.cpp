#include "TitleObject.h"
#include "Camera.h"
#include "TextureManager.h"
#include <algorithm>
#include "FrameInfo/FrameInfo.h"
#include "Input.h"
#include "Externals/imgui/imgui.h"

TitleObject::TitleObject()
{
	titleObj_ = std::make_unique<Model>("title");
	titleObj_->UnUsedLight();
	titleObj_->transform_.translate_ = { -1.3f,15.3f,-5.5f };
	titleObj_->transform_.scale_ = { 1.0f,13.0f,10.5f };
	titleObj_->transform_.rotate_.y = -1.57f;
	titleObj_->Update();

	for (int i = 0; i < 10; i++) {
		sprites_[i] = std::make_unique<Sprite>("Resources/white.png");
		if (i == 9) {
			sprites_[i]->size_ = { 1280.0f,720.0f };
		}
		else {
			sprites_[i]->size_ = { 1280.0f,40.0f };
			sprites_[i]->pos_.y -= 40.0f * i;
			sprites_[i]->SetColor({ 1.0f * ((i + 2) % 4) / 3,1.0f * (i % 5) / 4,1.0f * ((i + 3) % 7) / 6,1.0f });
		}
		sprites_[i]->Update();
	}

	highLumi_ = std::make_unique<HighLumi>();

	gaussian_ = std::make_unique<GaussianBlur>();

	post_ = std::make_unique<PostEffect>();
}

void TitleObject::Initialize()
{
}

void TitleObject::Update(Camera* camera)
{
#ifdef _DEBUG

	ImGui::Begin("TitleObj");
	ImGui::DragFloat3("スケール", &titleObj_->transform_.scale_.x, 0.01f);
	ImGui::DragFloat3("ポジション", &titleObj_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("回転", &titleObj_->transform_.rotate_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
	for (int i = 0; i < 9; i++) {
		sprites_[i]->pos_.y -= 20.0f;

		if (sprites_[i]->pos_.y <= -20.0f) {
			sprites_[i]->pos_.y += 360.0f;
		}
		sprites_[i]->Update();
	}

	titleObj_->Update();

	gaussian_->gaussianBlurData_->pickRange = 0.01f;
	gaussian_->gaussianBlurData_->stepWidth = 0.0025f;

	WrightPostEffect(camera);
}

void TitleObject::Draw(Camera* camera)
{
	titleObj_->Draw(*camera);

	post_->Draw();

	gaussian_->Draw(BlendMode::kBlendModeAdd);
}

void TitleObject::WrightPostEffect(Camera* camera)
{
	highLumi_->PreDrawScene();

	titleObj_->Draw(*camera);

	highLumi_->PostDrawScene();

	post_->PreDrawScene();

	for (int i = 0; i < 9; i++) {
		sprites_[i]->Draw();
	}

	highLumi_->Draw(BlendMode::kBlendModeMultiply);

	post_->PostDrawScene();

	gaussian_->PreDrawScene();

	post_->Draw();

	gaussian_->PostDrawScene();

}
