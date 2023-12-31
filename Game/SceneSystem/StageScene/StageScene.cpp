#include "StageScene.h"
#include "Externals/imgui/imgui.h"

StageScene::StageScene() {
	FirstInit();
	pBlockManager_ = BlockManager::GetInstance();
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

	/*particle = std::make_unique<Particle>("circle.png");
	particle->Initialize();*/

	camera_->transform_.translate_ = { 0.0f,2.0f,-50.0f };

	//for (uint32_t i = 0; i < 4; i++) {
	//	blocks_[i] = BlockManager::GetInstance()->AddBox(mesh1, IBlock{});
	//	blocks_[i]->transformMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,1.0f }, {}, { 2.0f * i,0.0f,0.0f });
	//	blocks_[i]->color_.r = 0.3f * i;
	//}

	file_.Load("Resources/LevelData/Level1.csv");
	csv_ = file_;

	mapChip_ = std::make_unique<MapChip>();
	mapChip_->Init();
	mapChip_->Load(csv_);

	ballList_.push_back(std::make_unique<MovingBall>());
	ballList_.back()->Init();
	MovingBall::SetMapChip(mapChip_.get());

	player_ = std::make_unique<Player>();
	player_->Init();
	player_->SetMapChip(mapChip_.get());
	player_->SetBallList(&ballList_);

	model->transform_.translate_.y = -500.f;
	model->Update();

	stageUI_ = std::make_unique<StageUI>();
	stageUI_->Init();
}

void StageScene::Update()
{
	// 時間差分
	[[maybe_unused]] const float deltaTime = std::clamp(ImGui::GetIO().DeltaTime, 0.f, 0.1f);

	/*if (input_->PressedKey(DIK_SPACE)) {
		if (isMesh1) {
			isMesh1 = false;
			model->SetMesh(mesh2);
		}
		else {
			isMesh1 = true;
			model->SetMesh(mesh1);
		}
	}*/

	/*if (input_->PressedKey(DIK_A)) {
		model->SetTex(textureManager_->LoadTexture("Resources/uvChecker.png"));
	}*/

	ImGui::Begin("a");
	ImGui::SliderFloat3("a", &model->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraTra", &camera_->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraRot", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();

	model->Update();
	//particle->Update();
	camera_->Update();
	mapChip_->Update(deltaTime);

	for (auto &ball : ballList_) {
		ball->Update(deltaTime);
	}

	player_->InputAction(input_, deltaTime);
	player_->Update(deltaTime);

	stageUI_->Update();
}

void StageScene::Draw() {
	//sprite->Draw(*camera_.get(), BlendMode::kBlendModeNormal);
	//sprite1->Draw(*camera_.get(), BlendMode::kBlendModeAdd);

	model->Draw(*camera_.get());

	//particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);

	pBlockManager_->clear();
	mapChip_->Draw();
	for (auto &ball : ballList_) {
		ball->Draw();
	}
	player_->Draw();

	pBlockManager_->Draw(*camera_.get());

	stageUI_->Draw(*camera_.get());
}
