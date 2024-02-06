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
	Hole::StaticInit();
	collisionRenderer_->Init();

	camera_->transform_.translate_ = { 0.0f,2.0f,-200.0f };
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

	/*stage_->GetSpawnerList()->push_back(std::make_unique<BallSpawner>());
	auto spawner = stage_->GetSpawnerList()->back().get();
	spawner->Init();
	BallSpawner::SetStage(stage_.get());*/

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
	isSwordAttack_ = false;

	needleBlur_ = std::make_unique<Blur>();
	isDrawNeedleBlur_ = false;
	isNeedleAttack_ = false;

	punchBlur_ = std::make_unique<Blur>();
	isDrawPunchBlur_ = false;
	isPunchAttack_ = false;

	playerBlur_ = std::make_unique<Blur>();
	isPlayerAttack_ = false;

	isRight_ = true;

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
	slotMirrorsPosX_ = 700.0f;
	slotMirrors_[0]->pos_.x -= slotMirrorsPosX_;
	slotMirrors_[1]->pos_.x += slotMirrorsPosX_;
	slotMirrors_[0]->Update();
	slotMirrors_[1]->Update();


	player_->SetHPBar(playerHPBar_.get());
	boss_->SetHPBar(bossHPBar_.get());

	stage_->Update(0.f);

	decoration_ = std::make_unique<Decoration>();
	decoration_->SetIsSword(&isSwordAttack_);
	decoration_->SetIsNeedle(&isNeedleAttack_);
	decoration_->SetIsPunch(&isPunchAttack_);
	decoration_->SetIsRight(&isRight_);
	decoration_->Initialize();
	decoration_->Update(camera_.get());

	farPostEffect_ = std::make_unique<PostEffect>();

	CreatePostEffects();

	space_ = std::make_unique<Sprite>("Resources/space.png");
	space_->pos_ = { 640.0f,600.0f };
	space_->size_ *= 0.6f;
	space_->Update();

	se_.Load("Resources/SE/start.wav");
	BGM.Load("Resources/SE/BGM.wav");
	BGM.Play(true, 0.4f);
}

void StageScene::Update()
{
	// 時間差分
	[[maybe_unused]] const float deltaTime = std::clamp(ImGui::GetIO().DeltaTime, 0.f, 0.1f);

#ifdef _DEBUG
	if (input_->PressedKey(DIK_SPACE)) {
		// シーン切り替え
		ChangeScene(CLEAR);
		BGM.Stop();
	}

	ImGui::Begin("StageEditor");
	stage_->ImGuiWidget();
	ImGui::End();

	GlobalVariables *globalVariable = GlobalVariables::GetInstance();
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
	if (bossHPBar_->GetHP() <= 0.0f) {
		// シーン切り替え
		ChangeScene(CLEAR);
		BGM.Stop();
	}
	if (playerHPBar_->GetHP() <= 0.0f) {
		// シーン切り替え
		ChangeScene(STAGE);
		BGM.Stop();
	}

	if (isTitle_) {
		TitleUpdate(deltaTime);
	}
	else {
		camera_->Update();
		stage_->Update(deltaTime);


		player_->InputAction(input_, deltaTime);
		player_->Update(deltaTime);
		player_->ImGuiWidget();

		for (auto &ball : *stage_->GetBallList()) {

			const auto &sphere = ball->GetSphere();

			if (player_->GetRadius() + sphere.radius_ >= Calc::MakeLength(player_->GetGrobalPos() - sphere.center_)) {

				player_->OnCollision(ball.get());

				ball->OnCollision(player_.get());

			}

			for (const auto &pin : *stage_->GetPinList()) {

				if (pin->GetRadius() + sphere.radius_ >= Calc::MakeLength(pin->GetPos() - sphere.center_)) {

					pin->OnCollision(ball.get());

					ball->OnCollision(pin.get());

				}
			}

			for (const auto &hole : *stage_->GetHoleList()) {

				Vector3 holePos = hole->GetPos();
				holePos.z = 0.f;

				if (hole->GetRadius() + sphere.radius_ >= Calc::MakeLength(holePos - sphere.center_)) {

					hole->OnCollision(ball.get());

					ball->OnCollision(hole.get());

				}
			}

		}
		{
			const Sphere &sphere = player_->GetSphere();
			for (const auto &hole : *stage_->GetHoleList()) {

				Vector3 holePos = hole->GetPos();
				holePos.z = 0.f;

				if (hole->GetRadius() + sphere.radius_ >= Calc::MakeLength(holePos - sphere.center_)) {

					hole->OnCollision(player_.get());

					player_->OnCollision(hole.get());

				}
			}
		}

		for (const auto &sword : *stage_->GetSwordList()) {
			auto *collision = sword->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(sword.get());
					sword->OnCollision(player_.get());
				}
			}
		}

		for (const auto &punch : *stage_->GetPunchList()) {
			auto *collision = punch->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(punch.get());
					punch->OnCollision(player_.get());
				}
			}
		}
		for (const auto &needle : *stage_->GetNeedleList()) {
			auto *collision = needle->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(needle.get());
					needle->OnCollision(player_.get());
				}
			}
		}

		for (const auto &sword : *stage_->GetSwordList()) {
			auto *collision = sword->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(sword.get());
					sword->OnCollision(player_.get());
				}

				if (not sword->GetIsHitBoss() && Collision::IsCollision(boss_->GetCollision(), *collision)) {
					boss_->OnCollision(sword.get());
					sword->OnCollision(boss_.get());
				}
			}
		}

		for (const auto &punch : *stage_->GetPunchList()) {
			auto *collision = punch->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(punch.get());
					punch->OnCollision(player_.get());
				}

				if (not punch->GetIsHitBoss() && Collision::IsCollision(boss_->GetCollision(), *collision)) {
					boss_->OnCollision(punch.get());
					punch->OnCollision(boss_.get());
				}
			}
		}
		for (const auto &needle : *stage_->GetNeedleList()) {
			auto *collision = needle->GetCollision();
			if (collision) {
				if (Collision::IsCollision(player_->GetSphere(), *collision)) {
					player_->OnCollision(needle.get());
					needle->OnCollision(player_.get());
				}

				if (not needle->GetIsHitBoss() && Collision::IsCollision(boss_->GetCollision(), *collision)) {
					boss_->OnCollision(needle.get());
					needle->OnCollision(boss_.get());
				}
			}
		}


		auto &bossCollision = boss_->GetCollision();
		//if (boss_->IsAttacked()) {

		if (Collision::IsCollision(player_->GetSphere(), bossCollision)) {
			player_->OnCollision(boss_.get());
			boss_->OnCollision(player_.get());
		}

		//}

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

		for (const std::unique_ptr<Sprite> &sprite : slotMirrors_) {
			sprite->Draw();
		}

		if (!isStart_) {
			space_->Draw();
		}
	}
	else {
		backgroundSprite_->Draw();

		farPostEffect_->Draw();
		decoration_->Draw(camera_.get());

		pBlockManager_->clear();
		stage_->DrawNearObject();
		/*stage_->Draw();

		player_->Draw();

		boss_->Draw();*/

		collisionRenderer_->AddCollision(player_->GetSphere());

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto &punchList = *stage_->GetPunchList();
		for (const auto &punch : punchList) {
			const auto *const swordCollision = punch->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto &swordList = *stage_->GetSwordList();
		for (const auto &sword : swordList) {
			const auto *const swordCollision = sword->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// トゲの配列から、当たり判定を取得して有効なら描画する
		const auto &needleList = *stage_->GetNeedleList();
		for (const auto &needle : needleList) {
			const auto *const needleCollision = needle->GetCollision();
			if (needleCollision) {
				collisionRenderer_->AddCollision(*needleCollision);
			}
		}

		collisionRenderer_->AddCollision(player_->GetSphere());

		pBlockManager_->Draw(*camera_.get());

		if (isPlayerAttack_) {
			playerBlur_->Draw(BlendMode::kBlendModeAdd);
		}

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
	stage_->Update(0.f);

	if (!isStart_ && input_->PressedGamePadButton(Input::GamePadButton::A)) {
		isStart_ = true;
		countEaseTime_ = 0.0f;
		se_.Play(false, 0.6f);
	}

	if (isStart_) {
		float maxTime = 0.6f;

		//float ratio = std::tanf(0.225f) * (titleCameraPos_.z) * 2 * 16 / 9;
		float hoge = slotMirrorsPosX_ * firstCameraPos_.z / titleCameraPos_.z * 1.6f;

		countEaseTime_ += FrameInfo::GetInstance()->GetDeltaTime();
		countEaseTime_ = std::clamp(countEaseTime_, 0.0f, maxTime);

		camera_->transform_.translate_ = Ease::UseEase(titleCameraPos_, firstCameraPos_, countEaseTime_, maxTime, Ease::Constant);
		slotMirrors_[0]->pos_.x = Ease::UseEase(-slotMirrorsPosX_ + 640.0f, -slotMirrorsPosX_ - hoge + 640.0f, countEaseTime_, maxTime, Ease::Constant);
		slotMirrors_[1]->pos_.x = Ease::UseEase(slotMirrorsPosX_ + 640.0f, slotMirrorsPosX_ + hoge + 640.0f, countEaseTime_, maxTime, Ease::Constant);
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
	isSwordAttack_ = false;

	pBlockManager_->clear();

	for (const auto &sword : *swordList) {
		if (sword->IsPreAttacked()) {
			isSwordAttack_ = true;
			if (sword->GetPos().x > 0) {
				isRight_ = true;
			}
			else {
				isRight_ = false;
			}
		}

		if (sword->IsAttacked()) {
			sword->Draw();
			if (sword->GetPos().x > 0) {
				isRight_ = true;
			}
			else {
				isRight_ = false;
			}
			isSwordAttack_ = true;
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
	isNeedleAttack_ = false;

	pBlockManager_->clear();

	for (const auto &needle : *needleList) {
		if (needle->IsPreAttacked()) {
			isNeedleAttack_ = true;
		}

		if (needle->IsAttacked()) {
			needle->Draw();
			isNeedleAttack_ = true;
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
	isPunchAttack_ = false;

	pBlockManager_->clear();

	for (const auto &punch : *punchList) {
		if (punch->IsPreAttacked()) {
			isPunchAttack_ = true;
			isRight_ = punch->IsRight();
		}

		if (punch->IsAttacked()) {
			punch->Draw();
			isRight_ = punch->IsRight();
			isPunchAttack_ = true;
			isDrawPunchBlur_ = true;
			punchBlur_->blurData_->isCenterBlur = 0;
			punchBlur_->blurData_->pickRange = 0.08f;
			punchBlur_->blurData_->stepWidth = 0.005f;

			if (isRight_) {
				punchBlur_->blurData_->angle = 0.0f;
			}
			else {
				punchBlur_->blurData_->angle = std::numbers::pi_v<float>;
			}
			break;
		}
	}

	if (isDrawPunchBlur_) {
		punchBlur_->PreDrawScene();

		pBlockManager_->Draw(*camera_.get());

		punchBlur_->PostDrawScene();
	}

	if (player_->GetState() == Player::State::kTackle) {
		isPlayerAttack_ = true;
		playerBlur_->blurData_->isCenterBlur = 0;

		Vector3 velocity = player_->GetVelocity();
		//float speed = velocity.Length();

		playerBlur_->blurData_->pickRange = 0.08f;
		playerBlur_->blurData_->stepWidth = 0.005f;

		float angle = std::asinf(velocity.Normalize().y);
		if (velocity.x <= 0) {
			playerBlur_->blurData_->angle = -angle;
		}
		else {
			playerBlur_->blurData_->angle = std::numbers::pi_v<float> +angle;
		}

		pBlockManager_->clear();

		player_->Draw();

		playerBlur_->PreDrawScene();

		pBlockManager_->Draw(*camera_.get());

		playerBlur_->PostDrawScene();

	}
	else {
		isPlayerAttack_ = false;
	}

	decoration_->Update(camera_.get());

	farPostEffect_->PreDrawScene();

	pBlockManager_->clear();
	stage_->DrawFarObject();
	player_->Draw();
	boss_->Draw();

	pBlockManager_->Draw(*camera_.get());

	farPostEffect_->PostDrawScene();

	if (isTitle_) {

		postEffect_->PreDrawScene();

		farPostEffect_->Draw();
		decoration_->Draw(camera_.get());

		pBlockManager_->clear();
		stage_->DrawNearObject();
		/*stage_->Draw();

		player_->Draw();

		boss_->Draw();*/

		collisionRenderer_->AddCollision(player_->GetSphere());

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto &punchList = *stage_->GetPunchList();
		for (const auto &punch : punchList) {
			const auto *const swordCollision = punch->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// 剣の配列から、当たり判定を取得して有効なら描画する
		const auto &swordList = *stage_->GetSwordList();
		for (const auto &sword : swordList) {
			const auto *const swordCollision = sword->GetCollision();
			if (swordCollision) {
				collisionRenderer_->AddCollision(*swordCollision);
			}
		}

		// トゲの配列から、当たり判定を取得して有効なら描画する
		const auto &needleList = *stage_->GetNeedleList();
		for (const auto &needle : needleList) {
			const auto *const needleCollision = needle->GetCollision();
			if (needleCollision) {
				collisionRenderer_->AddCollision(*needleCollision);
			}
		}

		collisionRenderer_->AddCollision(player_->GetSphere());

		pBlockManager_->Draw(*camera_.get());

		if (isPlayerAttack_) {
			playerBlur_->Draw(BlendMode::kBlendModeAdd);
		}

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
