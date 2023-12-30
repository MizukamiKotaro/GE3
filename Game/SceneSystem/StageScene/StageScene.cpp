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
	sprite1->pos_.x += 300.0f;
	sprite1->Update();

	screen_ = std::make_unique<Sprite>("Resources/screen.png");
	screen_->pos_ = { 640.0f,360.0f };
	screen_->Update();

	mesh1 = modelDataManager_->LoadObj("Sphere");
	mesh2 = modelDataManager_->LoadObj("weapon");

	isMesh1 = true;

	directionalLight_ = std::make_unique<DirectionalLight>();
	
	pointLight_ = std::make_unique<PointLight>();

	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->light_->position = { 2.0f,1.25f,0.0f };
	spotLight_->light_->direction = { -1.0f,-1.0f,0.0f };
	spotLight_->light_->intensity = 4.0f;
	spotLight_->light_->decay = 2.0f;

	model = std::make_unique<Model>(mesh1);
	model->Initialize();

	model->SetDirectionalLight(directionalLight_.get());
	model->SetPointLight(pointLight_.get());
	model->SetSpotLight(spotLight_.get());

	terrain = std::make_unique<Model>("terrain");
	terrain->Initialize();
	terrain->SetDirectionalLight(directionalLight_.get());
	terrain->SetPointLight(pointLight_.get());
	terrain->SetSpotLight(spotLight_.get());

	particle = std::make_unique<Particle>("circle.png");
	particle->Initialize();
	
	particle1 = std::make_unique<Particle>("uvChecker.png");
	particle1->Initialize();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {
			spotLights_[i][j] = std::make_unique<SpotLight>();
			spotLights_[i][j]->light_->position = { 2.0f,-1.25f,-10.0f + 5.0f * i};
			spotLights_[i][j]->light_->direction = { 1.0f,1.0f,0.0f };
			spotLights_[i][j]->light_->intensity = 4.0f;
			spotLights_[i][j]->light_->decay = 2.0f;

			if (j != 0) {
				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}

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

#ifdef _DEBUG


	ImGui::Begin("a");
	ImGui::SliderFloat3("modelPos", &model->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("modelScale", &model->transform_.scale_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("modelRotate", &model->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("cameraPos", &camera_->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraRotate", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();

	ImGui::Begin("Lights");
	ImGui::SliderFloat3("direction", &directionalLight_->light_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("directionalLightIntesity", &directionalLight_->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat3("pointLightPos", &pointLight_->light_->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat("pointLightIntesity", &pointLight_->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLightRadius", &pointLight_->light_->radius, 0.0f, 100.0f);
	ImGui::SliderFloat("pointLightDecay", &pointLight_->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat3("spotPos", &spotLight_->light_->position.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("spotDirection", &spotLight_->light_->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("spotLightIntesity", &spotLight_->light_->intensity, 0.0f, 100.0f);
	ImGui::SliderFloat("spotLightRadius", &spotLight_->light_->distance, 0.0f, 100.0f);
	ImGui::SliderFloat("spotLightDecay", &spotLight_->light_->decay, 0.0f, 100.0f);
	ImGui::SliderFloat("spotLightCos", &spotLight_->light_->cosAngle, -1.0f, 1.0f);
	ImGui::SliderFloat("spotLightCosFalloffStart", &spotLight_->light_->cosFalloffStart, -1.0f, 1.0f);

	ImGui::End();

	ImGui::Begin("Particles");
	ImGui::DragFloat3("emitter0Pos", &particle->emitter.pos.x, 0.01f);
	ImGui::DragFloat3("emitter0min", &particle->emitter.min.x, 0.01f);
	ImGui::DragFloat3("emitter0max", &particle->emitter.max.x, 0.01f);
	ImGui::DragFloat3("emitter1Pos", &particle1->emitter.pos.x, 0.01f);
	ImGui::DragFloat3("emitter1min", &particle1->emitter.min.x, 0.01f);
	ImGui::DragFloat3("emitter1max", &particle1->emitter.max.x, 0.01f);
	ImGui::End();

	ImGui::Begin("SpotLights");
	if (ImGui::TreeNode("0")) {
		ImGui::SliderFloat3("ポジション0", &spotLights_[0][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度0", &spotLights_[0][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度0", &spotLights_[0][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離0", &spotLights_[0][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率0", &spotLights_[0][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度0", &spotLights_[0][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度0", &spotLights_[0][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[0][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("1")) {
		ImGui::SliderFloat3("ポジション1", &spotLights_[1][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度1", &spotLights_[1][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度1", &spotLights_[1][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離1", &spotLights_[1][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率1", &spotLights_[1][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度1", &spotLights_[1][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度1", &spotLights_[1][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[1][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("2")) {
		ImGui::SliderFloat3("ポジション2", &spotLights_[2][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度2", &spotLights_[2][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度2", &spotLights_[2][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離2", &spotLights_[2][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率2", &spotLights_[2][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度2", &spotLights_[2][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度2", &spotLights_[2][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[2][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("3")) {
		ImGui::SliderFloat3("ポジション3", &spotLights_[3][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度3", &spotLights_[3][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度3", &spotLights_[3][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離3", &spotLights_[3][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率3", &spotLights_[3][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度3", &spotLights_[3][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度3", &spotLights_[3][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[3][0]->light_->color.x);
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("4")) {
		ImGui::SliderFloat3("ポジション4", &spotLights_[4][0]->light_->position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("ライトの角度4", &spotLights_[4][0]->light_->direction.x, -1.0f, 1.0f);
		ImGui::SliderFloat("輝度4", &spotLights_[4][0]->light_->intensity, 0.0f, 100.0f);
		ImGui::SliderFloat("距離4", &spotLights_[4][0]->light_->distance, 0.0f, 100.0f);
		ImGui::SliderFloat("減衰率4", &spotLights_[4][0]->light_->decay, 0.0f, 100.0f);
		ImGui::SliderFloat("開く角度4", &spotLights_[4][0]->light_->cosAngle, -1.0f, 1.0f);
		ImGui::SliderFloat("減衰し始める角度4", &spotLights_[4][0]->light_->cosFalloffStart, -1.0f, 1.0f);
		ImGui::ColorEdit4("色", &spotLights_[4][0]->light_->color.x);
		ImGui::TreePop();
	}

	ImGui::End();


#endif // _DEBUG

	directionalLight_->Update();
	pointLight_->Update();
	spotLight_->Update();

	model->Update();
	terrain->Update();
	particle->Update();
	particle1->Update();
	camera_->Update();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {

			if (j != 0) {
				*spotLights_[i][j]->light_ = (*spotLights_[i][0]->light_);

				spotLights_[i][j]->light_->position.x *= -1;
				spotLights_[i][j]->light_->direction.x *= -1;
			}

			spotLights_[i][j]->Update();
		}
	}
}

void StageScene::Draw()
{
	/*sprite->Draw(*camera_.get(), BlendMode::kBlendModeNormal);
	sprite1->Draw(*camera_.get(), BlendMode::kBlendModeAdd);*/

	model->Draw(*camera_.get());
	terrain->Draw(*camera_.get());
	/*pointLight_->Draw(*camera_.get());
	spotLight_->Draw(*camera_.get());*/

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 2; j++) {

			spotLights_[i][j]->Draw(*camera_.get());
		}
	}

	/*particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);
	particle1->Draw(*camera_.get(), BlendMode::kBlendModeScreen);*/
}

void StageScene::DrawPostEffect()
{
	sprite->Draw(*camera_.get(), BlendMode::kBlendModeNormal);
	sprite1->Draw(*camera_.get(), BlendMode::kBlendModeAdd);

	/*pointLight_->Draw(*camera_.get());
	spotLight_->Draw(*camera_.get());*/
	model->Draw(*camera_.get());
	terrain->Draw(*camera_.get());

	/*particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);
	particle1->Draw(*camera_.get(), BlendMode::kBlendModeScreen);*/

	screen_->Draw(*camera_.get(), BlendMode::kBlendModeMultiply);
}
