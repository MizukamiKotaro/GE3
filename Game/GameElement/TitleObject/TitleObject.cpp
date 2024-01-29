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

	rainbow_ = std::make_unique<Sprite>("Resources/rainbow.png");
	texcoodY_ = 0.0f;

	highLumi_ = std::make_unique<HighLumi>();

	gaussian_ = std::make_unique<GaussianBlur>();

	post_ = std::make_unique<PostEffect>();
}

void TitleObject::Initialize()
{
	texcoodY_ = 0.0f;
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
	
	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

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

	rainbow_->Draw();

	highLumi_->Draw(BlendMode::kBlendModeMultiply);

	post_->PostDrawScene();

	gaussian_->PreDrawScene();

	post_->Draw();

	gaussian_->PostDrawScene();

}
