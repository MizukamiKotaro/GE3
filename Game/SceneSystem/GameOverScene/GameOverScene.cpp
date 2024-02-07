#include "GameOverScene.h"
#include "Kyoko.h"

GameOverScene::GameOverScene()
{
	FirstInit();

	clearSprite_ = std::make_unique<Sprite>("Resources/result.png");

	pBlockManager_ = BlockManager::GetInstance();

}

void GameOverScene::Init()
{
	camera_->transform_.translate_ = { -19.8f,-17.0f,-67.2f };
	camera_->transform_.rotate_ = { -0.2f,0.3f,0.0f };
	camera_->Update();

	if (not csv_) {
		csv_ = SoLib::IO::File("Resources/LevelData/Level1.csv");
	}

	stage_ = std::make_unique<Stage>();
	stage_->Init();
	stage_->LoadCSV(csv_);
	stage_->SetWeapon();

	player_ = std::make_unique<ClearPlayer>();
	player_->Initialize();

	post_ = std::make_unique<PostEffect>();

	decoration_ = std::make_unique<Decoration>();
	isSwordAttack_ = false;
	isNeedleAttack_ = false;
	isPunchAttack_ = false;
	isRight_ = true;

	decoration_->SetIsSword(&isSwordAttack_);
	decoration_->SetIsNeedle(&isNeedleAttack_);
	decoration_->SetIsPunch(&isPunchAttack_);
	decoration_->SetIsRight(&isRight_);
	decoration_->Initialize();

	isSwordAttack_ = false;
	isNeedleAttack_ = false;
	isPunchAttack_ = false;
	isRight_ = true;
	stage_->Update(0.f);
	decoration_->tcUpdate(camera_.get());

	slot_ = std::make_unique<Slot>();
	slot_->GameOverInit();
	auto& hoge = slot_->GetTransform();
	hoge.translate_ = { -7.9f,-5.0f,-5.0f };
	hoge.rotate_ = { 0.0f,3.2f,0.0f };
	slot_->Update(camera_.get());

	space_ = std::make_unique<Sprite>("Resources/gameEnd.png");
	space_->pos_ = { 640.0f,600.0f };
	space_->size_ *= 0.4f;
	space_->Update();

	se_.Load("Resources/select.wav");

	back_ = std::make_unique<BackGround>();
	clear_ = std::make_unique<Sprite>("Resources/gameover.png");
	clear_->size_ *= 0.8f;
	clear_->pos_.y = 220.0f;
	clear_->Update();

	ea_.Load("Resources/SE/gameOver.wav");
	ea_.Play(false, 0.3f);
}

void GameOverScene::Update()
{
#ifdef _DEBUG
	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}


#endif // _DEBUG

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		ChangeScene(STAGE);
		se_.Play(false, 0.5f);
		ea_.Stop();
	}

	vel_ -= 0.04f;

	auto& hoge = slot_->GetTransform();
	hoge.translate_.y += vel_;

	if (hoge.translate_.y <= -5.0f) {
		hoge.translate_.y = -5.0f;
		vel_ = 0.7f;
	}

	player_->oUpdate();
	decoration_->tcUpdate(camera_.get());
	slot_->Update(camera_.get());
	back_->Update();
}

void GameOverScene::Draw()
{
	post_->PreDrawScene();
	pBlockManager_->clear();
	stage_->DrawFarObject();

	pBlockManager_->Draw(*camera_.get());

	post_->PostDrawScene();

	Kyoko::PreDraw();

	backgroundSprite_->Draw();
	back_->cDraw(camera_.get());

	post_->Draw();

	decoration_->tcDraw(camera_.get());

	pBlockManager_->clear();
	stage_->DrawNearObject();
	pBlockManager_->Draw(*camera_.get());

	slot_->Draw(camera_.get());
	player_->Draw(camera_.get());
	//clearSprite_->Draw();
	space_->Draw();
	clear_->Draw();

	BlackDraw();

	Kyoko::PostDraw();
}


