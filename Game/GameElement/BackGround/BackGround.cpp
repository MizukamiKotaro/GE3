#include "BackGround.h"
#include "ImGuiManager/ImGuiManager.h"
#include "ModelDataManager.h"
#include <cmath>
#include "Camera.h"
#include "FrameInfo/FrameInfo.h"

BackGround::BackGround()
{
	back_ = std::make_unique<Sprite>("Resources/back.png");
	//gaus_ = std::make_unique<GaussianBlur>();


	/*gaus_->PreDrawScene();
	back_->Draw();
	gaus_->PostDrawScene();*/

	blur0_ = std::make_unique<Blur>();
	blur1_ = std::make_unique<Blur>();
	post_ = std::make_unique<PostEffect>();

	blur0_->blurData_->angle = 1.57f;
	blur0_->blurData_->pickRange = 0.006f;
	blur0_->blurData_->stepWidth = 0.001f;

	blur1_->blurData_->pickRange = 0.004f;
	blur1_->blurData_->stepWidth = 0.001f;

	blur0_->PreDrawScene();
	back_->Draw();
	blur0_->PostDrawScene();

	blur1_->PreDrawScene();
	back_->Draw();
	blur1_->PostDrawScene();

	post_->PreDrawScene();
	back_->Draw();
	blur0_->Draw(BlendMode::kBlendModeAdd);
	blur1_->Draw(BlendMode::kBlendModeAdd);
	post_->PostDrawScene();
}

void BackGround::Initialize()
{

}

void BackGround::Update()
{
}

void BackGround::Draw()
{
	/*back_->Draw();
	gaus_->Draw(BlendMode::kBlendModeAdd);*/
	post_->Draw();
}

void BackGround::cDraw(Camera* camera)
{
}
