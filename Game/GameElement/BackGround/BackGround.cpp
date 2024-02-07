#include "BackGround.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ModelDataManager.h"
#include <cmath>
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"

BackGround::BackGround()
{
	back_ = std::make_unique<Sprite>("Resources/back.png");
	ho_ = std::make_unique<Sprite>("Resources/backB.png");
	//gaus_ = std::make_unique<GaussianBlur>();


	/*gaus_->PreDrawScene();
	back_->Draw();
	gaus_->PostDrawScene();*/

	blur0_ = std::make_unique<Blur>();
	blur1_ = std::make_unique<Blur>();
	post_ = std::make_unique<PostEffect>();
	model_ = std::make_unique<Model>("plane");
	model_->SetSRVGPUDescriptorHandle_(post_->GetSRVGPUDescriptorHandle());
	model_->UnUsedLight();
	model_->transform_.rotate_.y = -3.15f;
	model_->transform_.scale_ = { 34.8f,19.8f,1.0f };
	model_->Update();

	rainbow_ = std::make_unique<Sprite>("Resources/rainbow.png");
	texcoodY_ = 0.0f;

	af_ = std::make_unique<PostEffect>();

	high_ = std::make_unique<HighLumi>();

	blur0_->blurData_->angle = 1.57f;
	blur0_->blurData_->pickRange = 0.006f;
	blur0_->blurData_->stepWidth = 0.001f;

	blur1_->blurData_->pickRange = 0.004f;
	blur1_->blurData_->stepWidth = 0.001f;

	high_->PreDrawScene();
	ho_->Draw();
	high_->PostDrawScene();

	af_->PreDrawScene();
	rainbow_->Draw();
	high_->Draw(BlendMode::kBlendModeMultiply);
	af_->PostDrawScene();

	blur0_->PreDrawScene();
	//back_->Draw();
	af_->Draw();
	blur0_->PostDrawScene();

	blur1_->PreDrawScene();
	//back_->Draw();
	af_->Draw();
	blur1_->PostDrawScene();

	post_->PreDrawScene();
	back_->Draw();
	af_->Draw();
	blur0_->Draw(BlendMode::kBlendModeAdd);
	blur1_->Draw(BlendMode::kBlendModeAdd);
	post_->PostDrawScene();
}

void BackGround::Initialize()
{

}

void BackGround::Update()
{
#ifdef _DEBUG
	/*ImGui::Begin("iga");
	ImGui::DragFloat3("adf", &model_->transform_.translate_.x, 0.1f);
	ImGui::DragFloat3("adfas", &model_->transform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("f", &model_->transform_.scale_.x, 0.1f);
	ImGui::End();
	model_->Update();*/
#endif // _DEBUG
	texcoodY_ += 10.0f;
	if (texcoodY_ >= 720.0f) {
		texcoodY_ -= 720.0f;
	}
	rainbow_->SetTextureTopLeft({ 0.0f,texcoodY_ });

	af_->PreDrawScene();
	rainbow_->Draw();
	high_->Draw(BlendMode::kBlendModeMultiply);
	af_->PostDrawScene();

	blur0_->PreDrawScene();
	//back_->Draw();
	af_->Draw();
	blur0_->PostDrawScene();

	blur1_->PreDrawScene();
	//back_->Draw();
	af_->Draw();
	blur1_->PostDrawScene();

	post_->PreDrawScene();
	back_->Draw();
	af_->Draw();
	blur0_->Draw(BlendMode::kBlendModeAdd);
	blur1_->Draw(BlendMode::kBlendModeAdd);
	post_->PostDrawScene();
}

void BackGround::Draw()
{
	/*back_->Draw();
	gaus_->Draw(BlendMode::kBlendModeAdd);*/
	post_->Draw();
}

void BackGround::cDraw(Camera* camera)
{
	model_->Draw(*camera);
}
