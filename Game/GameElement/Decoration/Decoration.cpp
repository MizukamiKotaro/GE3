#include "Decoration.h"
#include "ImGuiManager/ImGuiManager.h"

Decoration::Decoration()
{
	swordDecration_ = std::make_unique<Model>("decoration");
	swordDecration_->UnUsedLight();
	swordDecration_->transform_.translate_ = { 13.6f,5.8f,5.0f };
	swordDecration_->transform_.scale_ = { 1.0f,8.0f,8.0f };
	swordDecration_->transform_.rotate_.y = -1.57f;
	swordDecration_->Update();

	rainbow_ = std::make_unique<Sprite>("Resources/rainbow.png");
	texcoodY_ = 0.0f;

	highLumi_ = std::make_unique<HighLumi>();

	gaussian_ = std::make_unique<GaussianBlur>();

	post_ = std::make_unique<PostEffect>();
}

void Decoration::Initialize()
{
	texcoodY_ = 0.0f;
}

void Decoration::Update(Camera* camera)
{
#ifdef _DEBUG

	ImGui::Begin("DecorationObj");
	ImGui::DragFloat3("スケール", &swordDecration_->transform_.scale_.x, 0.01f);
	ImGui::DragFloat3("ポジション", &swordDecration_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("回転", &swordDecration_->transform_.rotate_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	swordDecration_->Update();

	gaussian_->gaussianBlurData_->pickRange = 0.01f;
	gaussian_->gaussianBlurData_->stepWidth = 0.0025f;

	WrightPostEffect(camera);
}

void Decoration::Draw(Camera* camera)
{
	swordDecration_->Draw(*camera);


	if (*isSword_) {
		post_->Draw();

		gaussian_->Draw(BlendMode::kBlendModeAdd);
	}
}

void Decoration::WrightPostEffect(Camera* camera)
{
	if (*isSword_) {

		highLumi_->PreDrawScene();

		swordDecration_->Draw(*camera);

		highLumi_->PostDrawScene();

		post_->PreDrawScene();

		rainbow_->Draw();

		highLumi_->Draw(BlendMode::kBlendModeMultiply);

		post_->PostDrawScene();

		gaussian_->PreDrawScene();

		post_->Draw();

		gaussian_->PostDrawScene();
	}
}
