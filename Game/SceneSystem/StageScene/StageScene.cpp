#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"

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

	mesh1 = modelDataManager_->LoadObj("Sphere");
	mesh2 = modelDataManager_->LoadObj("weapon");

	isMesh1 = true;

	directionalLight_ = std::make_unique<DirectionalLight>();
	
	pointLight_ = std::make_unique<PointLight>();

	model = std::make_unique<Model>(mesh1);
	model->Initialize();

	model->SetDirectionalLight(directionalLight_.get());
	model->SetPointLight(pointLight_.get());

	terrain = std::make_unique<Model>("terrain");
	terrain->Initialize();
	terrain->SetDirectionalLight(directionalLight_.get());
	terrain->SetPointLight(pointLight_.get());

	particle = std::make_unique<Particle>("circle.png");
	particle->Initialize();

	camera_->transform_.translate_ = { 0.0f,2.0f,-50.0f };
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
	ImGui::SliderFloat3("modelPos", &model->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("modelScale", &model->transform_.scale_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("modelRotate", &model->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("direction", &directionalLight_->light_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("directionalLightIntesity", &directionalLight_->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat3("pointLightPos", &pointLight_->light_->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("pointLightIntesity", &pointLight_->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLightRadius", &pointLight_->light_->radius, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLightDecay", &pointLight_->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat3("cameraPos", &camera_->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraRotate", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();

	directionalLight_->Update();
	pointLight_->Update();

	model->Update();
	terrain->Update();
	particle->Update();
	camera_->Update();
}

void StageScene::Draw()
{
	sprite->Draw(*camera_.get(), BlendMode::kBlendModeNormal);
	sprite1->Draw(*camera_.get(), BlendMode::kBlendModeAdd);

	model->Draw(*camera_.get());
	terrain->Draw(*camera_.get());

	particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);
}
