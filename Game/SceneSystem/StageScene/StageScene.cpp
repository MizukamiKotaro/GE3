#include "StageScene.h"
#include "Externals/imgui/imgui.h"
#include "FrameInfo/FrameInfo.h"
#include <calc.h>
#include "Kyoko.h"
#include <numbers>
#include "GlobalVariables/GlobalVariables.h"
#include <algorithm>
#include "Ease/Ease.h"

StageScene::StageScene() {
	FirstInit();
	pBlockManager_ = BlockManager::GetInstance();

	collisionRenderer_ = CollisionRenderer::GetInstance();

}

void StageScene::Init()
{
	collisionRenderer_->Init();

	camera_->transform_.translate_ = { 0.0f,2.0f,-50.0f };
	camera_->Update();

	firstCameraPos_ = camera_->transform_.translate_;
	titleCameraPos_ = camera_->transform_.translate_;
	GlobalVariables *globalVariable = GlobalVariables::GetInstance();
	globalVariable->CreateGroup("StageCamera");
	globalVariable->AddItem("StageCamera", "ポジション", firstCameraPos_);
	globalVariable->AddItem("StageCamera", "タイトル待機時のポジション", titleCameraPos_);
	Vector3 prePos = titleCameraPos_;
	firstCameraPos_ = globalVariable->GetVector3Value("StageCamera", "ポジション");
	titleCameraPos_ = globalVariable->GetVector3Value("StageCamera", "タイトル待機時のポジション");

	if (prePos != firstCameraPos_) {
		camera_->transform_.translate_ = titleCameraPos_;
		camera_->Update();
	}

	if (not csv_) {
		csv_ = SoLib::IO::File("Resources/LevelData/Level1.csv");
	}

	stage_ = std::make_unique<Stage>();
	stage_->Init();
	stage_->LoadCSV(csv_);
	stage_->SetWeapon();

	stage_->GetSpawnerList()->push_back(std::make_unique<BallSpawner>());
	auto spawner = stage_->GetSpawnerList()->back().get();
	spawner->Init();
	BallSpawner::SetStage(stage_.get());

	/*ballList_.push_back(std::make_unique<MovingBall>());
	ballList_.back()->Init();*/
	MovingBall::SetMapChip(stage_->GetMapChip());

	player_ = std::make_unique<Player>();
	player_->Init();
	player_->SetMapChip(stage_->GetMapChip());
	player_->SetBallList(stage_->GetBallList());

	stageUI_ = std::make_unique<StageUI>();
	stageUI_->Init();

	boss_ = std::make_unique<Boss>();
	boss_->Init();
	boss_->SetCamera(camera_.get());

	swordBlur_ = std::make_unique<Blur>();
	isDrawSwordBlur_ = false;

	needleBlur_ = std::make_unique<Blur>();
	isDrawNeedleBlur_ = false;

	punchBlur_ = std::make_unique<Blur>();
	isDrawPunchBlur_ = false;

	playerHPBar_ = std::make_unique<HPBar>("PlayerHPBar");
	// playerの最大HP
	playerHPBar_->SetMaxHP(10.0f);
	// playerに合わせて動かす場合、ワールドポジションのポインタをポインタセット
	//playerHPBar_->SetParent();
	playerHPBar_->Initialize();
	playerHPBar_->Update(camera_.get());

	bossHPBar_ = std::make_unique<HPBar>("BossHPBar");
	// bossの最大HP
	bossHPBar_->SetMaxHP(10.0f);
	// bossに合わせて動かす場合、ワールドポジションのポインタをセット
	// bossHPBar_->SetParent();
	bossHPBar_->Initialize();
	bossHPBar_->Update(camera_.get());

	titleObj_ = std::make_unique<TitleObject>();
	titleObj_->WrightPostEffect(camera_.get());

	isTitle_ = true;
	isStart_ = false;
	postEffect_ = std::make_unique<PostEffect>();
	for (int i = 0; i < 2; i++) {
		slotMirrors_[i] = std::make_unique<Sprite>("Resources/white.png");
		slotMirrors_[i]->SetSRVGPUDescriptorHandle_(postEffect_->GetSRVGPUDescriptorHandle());
	}
	slotMirrors_[1]->SetIsFlipX(true);
	slotMirrorsPosX_ = 700.0f;
	slotMirrors_[0]->pos_.x -= slotMirrorsPosX_;
	slotMirrors_[1]->pos_.x += slotMirrorsPosX_;
	slotMirrors_[0]->Update();
	slotMirrors_[1]->Update();


	player_->SetHPBar(playerHPBar_.get());
	boss_->SetHPBar(bossHPBar_.get());
}

	[[maybe_unused]] const float deltaTime = std::clamp(ImGui::GetIO().DeltaTime, 0.f, 0.1f);
	stage_->Update(deltaTime);

	CreatePostEffects();
}

void StageScene::Update()
{
	// 時間差分
	[[maybe_unused]] const float deltaTime = std::clamp(ImGui::GetIO().DeltaTime, 0.f, 0.1f);

#ifdef _DEBUG
	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(CLEAR);
	}

	ImGui::Begin("StageEditor");
	stage_->ImGuiWidget();
	ImGui::End();

	GlobalVariables* globalVariable = GlobalVariables::GetInstance();
	Vector3 prePos;
	if (isTitle_) {
		prePos = titleCameraPos_;
	}
	else {
		prePos = firstCameraPos_;
	}

	titleCameraPos_ = globalVariable->GetVector3Value("StageCamera", "タイトル待機時のポジション");
	firstCameraPos_ = globalVariable->GetVector3Value("StageCamera", "ポジション");
	if (prePos != firstCameraPos_) {
		if (isTitle_) {
			camera_->transform_.translate_ = titleCameraPos_;
		}
		else {
			camera_->transform_.translate_ = firstCameraPos_;
		}
		camera_->Update();
	}

	collisionRenderer_->ImGuiWidget("Collision");

#endif // _DEBUG

	if (isTitle_) {
		TitleUpdate(deltaTime);
	}
	else {
		camera_->Update();
		stage_->Update(deltaTime);


		player_->InputAction(input_, deltaTime);
		player_->Update(deltaTime);

		for (auto& ball : *stage_->GetBallList()) {

			const auto& sphere = ball->GetSphere();

			if (player_->GetRadius() + sphere.radius_ >= Calc::MakeLength(player_->GetGrobalPos() - sphere.center_)) {

				player_->OnCollision(ball.get());

				ball->OnCollision(player_.get());

			}

			for (const auto& pin : *stage_->GetPinList()) {

				if (pin->GetRadius() + sphere.radius_ >= Calc::MakeLength(pin->GetPos() - sphere.center_)) {

					pin->OnCollision(ball.get());

					ball->OnCollision(pin.get());

				}
			}

			for (const auto& hole : *stage_->GetHoleList()) {

				Vector3 holePos = hole->GetPos();
				holePos.z = 0.f;

				if (hole->GetRadius() + sphere.radius_ >= Calc::MakeLength(holePos - sphere.center_)) {

					hole->OnCollision(ball.get());

					ball->OnCollision(hole.get());

				}
			}

		}

		for (const auto& sword : *stage_->GetSwordList()) {
			auto* collision = sword->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(sword.get());
					sword->OnCollision(player_.get());
				}
			}
		}
		for (const auto& punch : *stage_->GetPunchList()) {
			auto* collision = punch->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(punch.get());
					punch->OnCollision(player_.get());
				}
			}
		}

		stageUI_->Update();

		boss_->Update(deltaTime);

		playerHPBar_->Update(camera_.get());
		bossHPBar_->Update(camera_.get());
	}

}

void StageScene::Draw() {

	CreatePostEffects();

	// 描画開始
	Kyoko::PreDraw();

	if (isTitle_) {
		backgroundSprite_->Draw();

		postEffect_->Draw();

		titleObj_->Draw(camera_.get());

		for (const std::unique_ptr<Sprite>& sprite : slotMirrors_) {
			sprite->Draw();
		}
	}
	else {
		backgroundSprite_->Draw();

		pBlockManager_->clear();
		stage_->Draw();

		player_->Draw();

		boss_->Draw();

		collisionRenderer_->AddCollision(player_->GetSphere());

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto& punchList = *stage_->GetPunchList();
		for (const auto& punch : punchList) {
			const auto* const swordCollision = punch->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto& swordList = *stage_->GetSwordList();
		for (const auto& sword : swordList) {
			const auto* const swordCollision = sword->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// トゲの配列から、当たり判定を取得して有効なら描画する
		const auto& needleList = *stage_->GetNeedleList();
		for (const auto& needle : needleList) {
			const auto* const needleCollision = needle->GetCollision();
			if (needleCollision) {
				collisionRenderer_->AddCollision(*needleCollision);
			}
		}

		collisionRenderer_->AddCollision(player_->GetSphere());

		pBlockManager_->Draw(*camera_.get());

		if (isDrawSwordBlur_) {
			swordBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		if (isDrawNeedleBlur_) {
			needleBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		if (isDrawPunchBlur_) {
			punchBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		playerHPBar_->Draw();
		bossHPBar_->Draw();
		stageUI_->Draw(*camera_.get());
	}

	// フレームの終了
	BlackDraw();
	Kyoko::PostDraw();
}

void StageScene::TitleUpdate(float deltaTime)
{
	stage_->Update(deltaTime);

	if (!isStart_ && input_->PressedGamePadButton(Input::GamePadButton::A)) {
		isStart_ = true;
		countEaseTime_ = 0.0f;
	}

	if (isStart_) {
		float maxTime = 1.5f;

		countEaseTime_ += FrameInfo::GetInstance()->GetDeltaTime();
		countEaseTime_ = std::clamp(countEaseTime_, 0.0f, maxTime);

		camera_->transform_.translate_ = Ease::UseEase(titleCameraPos_, firstCameraPos_, countEaseTime_, maxTime, Ease::Constant);
		slotMirrors_[0]->pos_.x = Ease::UseEase(-slotMirrorsPosX_ + 640.0f, -slotMirrorsPosX_, countEaseTime_, maxTime, Ease::Constant);
		slotMirrors_[1]->pos_.x = Ease::UseEase(slotMirrorsPosX_ + 640.0f, slotMirrorsPosX_ + 640.0f * 2.0f, countEaseTime_, maxTime, Ease::Constant);
		slotMirrors_[0]->Update();
		slotMirrors_[1]->Update();

		if (countEaseTime_ >= maxTime) {
			isTitle_ = false;
		}
	}
	camera_->Update();

	titleObj_->Update(camera_.get());
}

void StageScene::CreatePostEffects()
{
	// 剣のブラー
	auto swordList = stage_->GetSwordList();
	isDrawSwordBlur_ = false;

	pBlockManager_->clear();

	for (const auto &sword : *swordList) {
		if (sword->IsAttacked()) {
			sword->Draw();
			isDrawSwordBlur_ = true;
			swordBlur_->blurData_->isCenterBlur = 0;
			swordBlur_->blurData_->pickRange = 0.04f;
			swordBlur_->blurData_->stepWidth = 0.005f;
			float postRot = sword->GetBeforeRotate();
			float rot = sword->GetRotateZ();
			if (rot - postRot <= 0.0f) {
				swordBlur_->blurData_->angle = std::numbers::pi_v<float> +rot;
			}
			else {
				swordBlur_->blurData_->angle = rot;
			}
			break;
		}
	}

	if (isDrawSwordBlur_) {
		swordBlur_->PreDrawScene();

		pBlockManager_->Draw(*camera_.get());

		swordBlur_->PostDrawScene();
	}

	// 槍のブラー
	auto needleList = stage_->GetNeedleList();
	isDrawNeedleBlur_ = false;

	pBlockManager_->clear();

	for (const auto &needle : *needleList) {
		if (needle->IsAttacked()) {
			needle->Draw();
			isDrawNeedleBlur_ = true;
			needleBlur_->blurData_->isCenterBlur = 0;
			needleBlur_->blurData_->pickRange = 0.08f;
			needleBlur_->blurData_->stepWidth = 0.005f;
			needleBlur_->blurData_->angle = -std::numbers::pi_v<float> / 2.0f;
			break;
		}
	}

	if (isDrawNeedleBlur_) {
		needleBlur_->PreDrawScene();

		pBlockManager_->Draw(*camera_.get());

		needleBlur_->PostDrawScene();
	}

	// パンチのブラー
	auto punchList = stage_->GetPunchList();
	isDrawPunchBlur_ = false;

	pBlockManager_->clear();

	for (const auto &punch : *punchList) {
		if (punch->IsAttacked()) {
			punch->Draw();
			isDrawPunchBlur_ = true;
			punchBlur_->blurData_->isCenterBlur = 0;
			punchBlur_->blurData_->pickRange = 0.08f;
			punchBlur_->blurData_->stepWidth = 0.005f;
			punchBlur_->blurData_->angle = 0.0f;
			break;
		}
	}

	if (isDrawPunchBlur_) {
		punchBlur_->PreDrawScene();

		pBlockManager_->Draw(*camera_.get());

		punchBlur_->PostDrawScene();
	}

	if (isTitle_) {

		postEffect_->PreDrawScene();

		pBlockManager_->clear();
		stage_->Draw();

		player_->Draw();

		boss_->Draw();

		collisionRenderer_->AddCollision(player_->GetSphere());

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto& punchList = *stage_->GetPunchList();
		for (const auto& punch : punchList) {
			const auto* const swordCollision = punch->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto& swordList = *stage_->GetSwordList();
		for (const auto& sword : swordList) {
			const auto* const swordCollision = sword->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// トゲの配列から、当たり判定を取得して有効なら描画する
		const auto& needleList = *stage_->GetNeedleList();
		for (const auto& needle : needleList) {
			const auto* const needleCollision = needle->GetCollision();
			if (needleCollision) {
				collisionRenderer_->AddCollision(*needleCollision);
			}
		}

		collisionRenderer_->AddCollision(player_->GetSphere());

		pBlockManager_->Draw(*camera_.get());

		if (isDrawSwordBlur_) {
			swordBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		if (isDrawNeedleBlur_) {
			needleBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		if (isDrawPunchBlur_) {
			punchBlur_->Draw(BlendMode::kBlendModeAdd);
		}

		postEffect_->PostDrawScene();
	}
}
