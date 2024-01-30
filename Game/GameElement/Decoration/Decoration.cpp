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

	swordDecrationB_ = std::make_unique<Model>("decorationB");
	swordDecrationB_->UnUsedLight();
	swordDecrationB_->transform_.translate_ = { -13.6f,5.8f,5.0f };
	swordDecrationB_->transform_.scale_ = { 1.0f,8.0f,8.0f };
	swordDecrationB_->transform_.rotate_.y = -1.57f;
	swordDecrationB_->Update();

	punchDecration_ = std::make_unique<Model>("decoration2");
	punchDecration_->UnUsedLight();
	punchDecration_->transform_.translate_ = { 13.6f,-7.8f,5.0f };
	punchDecration_->transform_.scale_ = { 1.0f,8.0f,8.0f };
	punchDecration_->transform_.rotate_.y = -1.57f;
	punchDecration_->Update();

	punchDecrationB_ = std::make_unique<Model>("decoration2B");
	punchDecrationB_->UnUsedLight();
	punchDecrationB_->transform_.translate_ = { -13.6f,-7.8f,5.0f };
	punchDecrationB_->transform_.scale_ = { 1.0f,8.0f,8.0f };
	punchDecrationB_->transform_.rotate_.y = -1.57f;
	punchDecrationB_->Update();

	needleDecration_ = std::make_unique<Model>("decoration1");
	needleDecration_->UnUsedLight();
	needleDecration_->transform_.translate_ = { 0.0f,-15.8f,5.0f };
	needleDecration_->transform_.scale_ = { 1.0f,8.0f,8.0f };
	needleDecration_->transform_.rotate_.y = -1.57f;
	needleDecration_->Update();


	rainbow_ = std::make_unique<Sprite>("Resources/rainbow.png");
	texcoodY_ = 0.0f;

	highLumi_ = std::make_unique<HighLumi>();

	gaussian_ = std::make_unique<GaussianBlur>();

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

	swordDecration_->Update();
	swordDecrationB_->Update();
	punchDecration_->Update();
	punchDecrationB_->Update();
	needleDecration_->Update();

	gaussian_->gaussianBlurData_->pickRange = 0.01f;
	gaussian_->gaussianBlurData_->stepWidth = 0.0025f;

	WrightPostEffect(camera);
}

void Decoration::Draw(Camera* camera)
{
	swordDecration_->Draw(*camera);
	swordDecrationB_->Draw(*camera);
	punchDecration_->Draw(*camera);
	punchDecrationB_->Draw(*camera);
	needleDecration_->Draw(*camera);

	if (*isSword_ || *isPunch_ || *isNeedle_) {
		post_->Draw();

		//gaussian_->Draw(BlendMode::kBlendModeAdd);
	}
}

void Decoration::WrightPostEffect(Camera* camera)
{
	if (*isSword_ || *isPunch_ || *isNeedle_ || isFirst_) {
		isFirst_ = false;

		highLumi_->PreDrawScene();

		if (*isSword_) {
			if (*isRight_) {
				swordDecration_->Draw(*camera);
			}
			else {
				swordDecrationB_->Draw(*camera);
			}
		}
		if (*isPunch_) {
			if (*isRight_) {
				punchDecration_->Draw(*camera);
			}
			else {
				punchDecrationB_->Draw(*camera);
			}
		}
		if (*isNeedle_) {
			needleDecration_->Draw(*camera);
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
