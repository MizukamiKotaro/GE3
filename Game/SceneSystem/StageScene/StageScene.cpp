#include "StageScene.h"
#include "Externals/imgui/imgui.h"

StageScene::StageScene()
{
	FirstInit();
}

void StageScene::Init()
{
	sprite = std::make_unique<Sprite>("Resources/uvChecker.png");
	sprite->pos_.x += 100.0f;
	sprite->Update();

	sprite1 = std::make_unique<Sprite>("Resources/uvChecker.png");
	sprite1->Update();

	mesh1 = modelDataManager_->LoadObj("Cube");
	mesh2 = modelDataManager_->LoadObj("weapon");

	isMesh1 = true;

	model = std::make_unique<Model>(mesh1);
	model->Initialize();

	particle = std::make_unique<Particle>("circle.png");
	particle->Initialize();

	camera_->transform_.translate_ = { 0.0f,2.0f,-50.0f };

	for (uint32_t i = 0; i < 4; i++) {
		blocks_[i] = BlockManager::GetInstance()->AddBox(mesh1, IBlock());
		blocks_[i]->transformMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,1.0f }, {}, { 2.0f * i,0.0f,0.0f });
		blocks_[i]->color_.x = 0.3f * i;
	}
}

void StageScene::Update()
{
	if (input_->PressedKey(DIK_SPACE)) {
		if (isMesh1) {
			isMesh1 = false;
			model->SetMesh(mesh2);
		}
		else {
			isMesh1 = true;
			model->SetMesh(mesh1);
		}
	}

	if (input_->PressedKey(DIK_A)) {
		model->SetTex(textureManager_->LoadTexture("Resources/uvChecker.png"));
	}

	ImGui::Begin("a");
	ImGui::SliderFloat3("a", &model->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraTra", &camera_->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraRot", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();

	model->Update();
	particle->Update();
	camera_->Update();
}

void StageScene::Draw()
{
	sprite->Draw(*camera_.get(), BlendMode::kBlendModeNormal);
	sprite1->Draw(*camera_.get(), BlendMode::kBlendModeAdd);

	model->Draw(*camera_.get());

	particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);

	BlockManager::GetInstance()->Draw(*camera_.get());
}
