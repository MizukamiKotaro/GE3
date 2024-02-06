#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

	clearSprite_ = std::make_unique<Sprite>("Resources/result.png");

	pBlockManager_ = BlockManager::GetInstance();

}

void ClearScene::Init()
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
	slot_->ClearInit();
	auto& hoge = slot_->GetTransform();
	hoge.translate_ = { 0.3f,-10.3f,-20.9f };
	hoge.rotate_ = { 0.52f,3.732f,0.0f };
	slot_->Update(camera_.get());

	space_ = std::make_unique<Sprite>("Resources/gameEnd.png");
	space_->pos_ = { 640.0f,600.0f };
	space_->size_ *= 0.4f;
	space_->Update();

	se_.Load("Resources/select.wav");
}

void ClearScene::Update()
{
#ifdef _DEBUG
	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(SELECT);
	}


#endif // _DEBUG

	if (input_->PressedGamePadButton(Input::GamePadButton::A)) {
		ChangeScene(STAGE);
		se_.Play(false, 0.8f);
	}

	player_->Update();
	decoration_->tcUpdate(camera_.get());
	slot_->Update(camera_.get());
}

void ClearScene::Draw()
{
	post_->PreDrawScene();
	pBlockManager_->clear();
	stage_->DrawFarObject();

	pBlockManager_->Draw(*camera_.get());

	post_->PostDrawScene();

	Kyoko::PreDraw();

	backgroundSprite_->Draw();

	post_->Draw();

	decoration_->tcDraw(camera_.get());

	pBlockManager_->clear();
	stage_->DrawNearObject();
	pBlockManager_->Draw(*camera_.get());

	slot_->Draw(camera_.get());
	player_->Draw(camera_.get());
	//clearSprite_->Draw();
	space_->Draw();

	BlackDraw();

	Kyoko::PostDraw();
}


