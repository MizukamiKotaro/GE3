#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"
#include "FrameInfo/FrameInfo.h"
#include "Notes/Notes.h"

StageScene::StageScene()
{
	FirstInit();

	skydome_ = std::make_unique<Skydome>();
	stage_ = std::make_unique<Stage>();
	screen_ = std::make_unique<Screen>();
	postScreen_ = std::make_unique<Screen>();

	screenSprite_ = std::make_unique<Sprite>("Resources/screen.png");
	screenSprite_->pos_ = { 640.0f,360.0f };
	screenSprite_->Update();

	se[0] = audio_->LoadWave("Resources/playerSE/Attack.wav");
	se[1] = audio_->LoadWave("Resources/playerSE/jump.wav");
	se[2] = audio_->LoadWave("Resources/Sounds/maou_short_14_shining_star.wav");

	directionalLight_ = std::make_unique<DirectionalLight>();

	pointLight_ = std::make_unique<PointLight>();

	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->light_->position = { 2.0f,1.25f,0.0f };
	spotLight_->light_->direction = { -1.0f,-1.0f,0.0f };
	spotLight_->light_->intensity = 4.0f;
	spotLight_->light_->decay = 2.0f;

	model = std::make_unique<Model>("Sphere");
	model->Initialize();

	model->SetDirectionalLight(directionalLight_.get());
	model->SetPointLight(pointLight_.get());
	model->SetSpotLight(spotLight_.get());

	particle = std::make_unique<Particle>("circle.png");
	particle->Initialize();

	postEffect_ = std::make_unique<PostEffect>("Resources/uvChecker.png");
	postEffect_->SetAnchorPoint({ 0.5f,0.5f });
	postEffect_->size_ = { 800.0f,400.0f };
	postEffect_->pos_ = { 640.0f,300.0f };
	postEffect_->Update();

	postScreen_->SetGPUHandle(postEffect_->GetSRVGPUDescriptorHandle());

	stageLights_ = std::make_unique<StageLights>();

	NotesList::GetInstance()->Init();

	score_ = std::make_unique<Score>();

	musicHundle_ = audio_->LoadWave("Resources/Sounds/maou_short_14_shining_star.wav");


	for (int i = 0; i < 4; i++) {

		if (i < 3) {
			if (i < 2) {
				comboNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
				comboNum_[i]->pos_ = { float(i * 64.0f + 1000.0f), 200.0f };
				comboNum_[i]->size_ = { 64.0f, 64.0f };
				comboNum_[i]->Update();
			}

			sp_[i] = std::make_unique<Sprite>("Resources/gameScene.png");
			sp_[i]->pos_ = { 400.0f,80.0f * i };
			sp_[i]->size_ = { 400.0f, 128.0f };
			sp_[i]->SetTextureTopLeft({ i * 200.0f, 0.0f });
			sp_[i]->SetTextureSize({ 200.0f, 64.0f });
		}

		scoreNum_[i] = std::make_unique<Sprite>("Resources/scoreNum.png");
		scoreNum_[i]->pos_ = { float(i * 64.0f + 64.0f), 120.0f };
		scoreNum_[i]->size_ = { 64.0f, 64.0f };
		scoreNum_[i]->Update();
	}

	sp_[0]->pos_ = { 160.0f,50.0f };
	sp_[1]->pos_ = { 1110.0f,100.0f };
	sp_[2]->pos_ = { 640.0f,300.0f };
	sp_[0]->Update();
	sp_[1]->Update();
	sp_[2]->Update();

	hitSp_ = std::make_unique<Sprite>("Resources/hit.png");
	hitSp_->pos_ = { 110.0f, 300.0f };

	hitSp_->size_ = { 220.0f,64.0f };
	hitSp_->Update();
}

void StageScene::Init()
{
	time_ = 0.0f;
	stageLights_->Initialize();

	NotesList::GetInstance()->PopCommands();

	score_->Reset();
	hitCount_ = 0;
	finishCount_ = 0.0f;
	isMusicFinish_ = false;
	isStartMusic_ = false;

	camera_->Initialize();
}

void StageScene::Update()
{
	if (transitionRequest_) {
		transition_ = transitionRequest_.value();

		switch (transition_)
		{
		case Transition::kFromBlack:
			FromBlackInitialize();
			break;
		case Transition::kOperation:
			break;
		case Transition::kToBlack:
			ToBlackInitialize();
			break;
		default:
			break;
		}

		transitionRequest_ = std::nullopt;
	}

	switch (transition_)
	{
	case Transition::kFromBlack:
		FromBlackUpdate();
		break;
	case Transition::kOperation:
		Play();
		break;
	case Transition::kToBlack:
		ToBlackUpdate(CLEAR);
		break;
	default:
		break;
	}

}

void StageScene::Play()
{
	//transitionRequest_ = Transition::kToBlack;

	if (!isStartMusic_) {
		isStartMusic_ = true;
		musicVoice_ = audio_->Play(musicHundle_);

	}

	if (!audio_->IsPlaying(musicVoice_)) {
		isMusicFinish_ = true;
	}

	time_ += FrameInfo::GetInstance()->GetDeltaTime();

	std::list<std::unique_ptr<Notes>>& notesList = NotesList::GetInstance()->GetNotesList();

	for (std::unique_ptr<Notes>& notes : notesList) {
		notes->Update(time_);

		if (time_ >= notes->GetTime() + kHitErrorFrame_[kGood]) {
			notes->Dead();
			score_->AddMiss();

			hitCount_ = 1;
			hitNum_ = kMiss;
		}
	}

	HitTest();

	for (std::list<std::unique_ptr<Notes>>::iterator i = notesList.begin(); i != notesList.end(); i++) {
		if (i->get()->IsDead()) {
			notesList.erase(i);
			break;
		}
	}

	if (isMusicFinish_) {
		finishCount_ += FrameInfo::GetInstance()->GetDeltaTime();

		if (finishCount_ >= 0.75f) {
			transitionRequest_ = Transition::kToBlack;
		}
	}

//#ifdef _DEBUG
//
//
//	ImGui::Begin("a");
//	ImGui::SliderFloat3("modelPos", &model->transform_.translate_.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("modelScale", &model->transform_.scale_.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("modelRotate", &model->transform_.rotate_.x, -3.14f, 3.14f);
//	ImGui::SliderFloat3("cameraPos", &camera_->transform_.translate_.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("cameraRotate", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
//	ImGui::End();
//
//	ImGui::Begin("Lights");
//	ImGui::SliderFloat3("direction", &directionalLight_->light_->direction.x, -1.0f, 1.0f);
//	ImGui::SliderFloat("directionalLightIntesity", &directionalLight_->light_->intensity, 0.0f, 100.0f);
//	ImGui::SliderFloat3("pointLightPos", &pointLight_->light_->position.x, -100.0f, 100.0f);
//	ImGui::SliderFloat("pointLightIntesity", &pointLight_->light_->intensity, 0.0f, 100.0f);
//	ImGui::SliderFloat("pointLightRadius", &pointLight_->light_->radius, 0.0f, 100.0f);
//	ImGui::SliderFloat("pointLightDecay", &pointLight_->light_->decay, 0.0f, 100.0f);
//	ImGui::SliderFloat3("spotPos", &spotLight_->light_->position.x, -100.0f, 100.0f);
//	ImGui::SliderFloat3("spotDirection", &spotLight_->light_->direction.x, -1.0f, 1.0f);
//	ImGui::SliderFloat("spotLightIntesity", &spotLight_->light_->intensity, 0.0f, 100.0f);
//	ImGui::SliderFloat("spotLightRadius", &spotLight_->light_->distance, 0.0f, 100.0f);
//	ImGui::SliderFloat("spotLightDecay", &spotLight_->light_->decay, 0.0f, 100.0f);
//	ImGui::SliderFloat("spotLightCos", &spotLight_->light_->cosAngle, -1.0f, 1.0f);
//	ImGui::SliderFloat("spotLightCosFalloffStart", &spotLight_->light_->cosFalloffStart, -1.0f, 1.0f);
//
//	ImGui::End();
//
//	ImGui::Begin("Particles");
//	ImGui::DragFloat3("emitter0Pos", &particle->emitter.pos.x, 0.01f);
//	ImGui::DragFloat3("emitter0min", &particle->emitter.min.x, 0.01f);
//	ImGui::DragFloat3("emitter0max", &particle->emitter.max.x, 0.01f);
//	ImGui::End();
//
//#endif // _DEBUG

	directionalLight_->Update();
	pointLight_->Update();
	spotLight_->Update();

	stageLights_->Update(time_);

	model->Update();
	particle->Update();
	camera_->Update();
}

void StageScene::HitTest()
{
	std::list<std::unique_ptr<Notes>>& notesList = NotesList::GetInstance()->GetNotesList();

	for (std::unique_ptr<Notes>& notes : notesList) {
		if ((input_->PressedKey(DIK_LEFT) && LEFT_ARROW == notes->GetType()) ||
			(input_->PressedKey(DIK_RIGHT) && RIGHT_ARROW == notes->GetType()) ||
			(input_->PressedKey(DIK_UP) && UP_ARROW == notes->GetType()) ||
			(input_->PressedKey(DIK_DOWN) && DOWN_ARROW == notes->GetType())) {
			float errorFrame = std::abs(time_ - notes->GetTime());

			if (errorFrame <= kHitErrorFrame_[kPerfect]) {
				notes->Dead();
				

				score_->AddPer();

				hitCount_ = 1;
				hitNum_ = kPerfect;

			}
			else if (errorFrame <= kHitErrorFrame_[kGreat]) {
				notes->Dead();
				

				score_->AddGre();
				hitCount_ = 1;
				hitNum_ = kGreat;

			}
			else if (errorFrame <= kHitErrorFrame_[kGood]) {
				notes->Dead();
				

				score_->AddGood();

				hitCount_ = 1;
				hitNum_ = kGood;
			}
		}
	}
}

void StageScene::UIDraw()
{
	int score = score_->GetScore();

	for (int i = 0; i < 4; i++) {
		int num = score * int(pow(10, i)) / 1000;
		score = score % (1000 / int(pow(10, i)));

		if (num > 9) {
			num = 0;
		}
		scoreNum_[i]->SetTextureTopLeft({ float(num * kNumSize), 0.0f });
		scoreNum_[i]->SetTextureSize({ float(kNumSize), float(kNumSize) });
		scoreNum_[i]->Update();
		scoreNum_[i]->Draw(*camera_.get());
	}
	sp_[0]->Update();
	sp_[0]->Draw(*camera_.get());

	score = score_->GetCom();
	int k = 0;

	for (int i = 0; i < 2; i++) {
		int num = score * int(pow(10, i)) / 10;
		score = score % (10 / int(pow(10, i)));

		if (num > 9) {
			num = 0;
		}

		if (num != 0) {
			k++;
		}

		if (k != 0) {
			comboNum_[i]->SetTextureTopLeft({ float(num * kNumSize), 0.0f });
			comboNum_[i]->SetTextureSize({ float(kNumSize), float(kNumSize) });
			comboNum_[i]->Update();
			comboNum_[i]->Draw(*camera_.get());
		}
	}
	sp_[1]->Update();
	sp_[1]->Draw(*camera_.get());

	if (hitCount_ != 0) {

		hitSp_->SetTextureTopLeft({ float(hitNum_ * kHitSizeX_), 0.0f });
		hitSp_->SetTextureSize({ float(kHitSizeX_), float(kHitSizeY_) });
		hitSp_->Update();
		hitSp_->Draw(*camera_.get());

		hitCount_++;
	}

	if (isMusicFinish_) {
		sp_[2]->Update();
		sp_[2]->Draw(*camera_.get());
	}
}

void StageScene::Draw()
{

	postEffect_->PreDrawScene();

	/*pointLight_->Draw(*camera_.get());
	spotLight_->Draw(*camera_.get());*/
	model->Draw(*camera_.get());

	skydome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	screen_->Draw(camera_.get());

	/*particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);
	particle1->Draw(*camera_.get(), BlendMode::kBlendModeScreen);*/

	particle->Draw(*camera_.get(), BlendMode::kBlendModeScreen);

	screenSprite_->Draw(*camera_.get(), BlendMode::kBlendModeMultiply);

	postEffect_->PostDrawScene();


	Kyoko::PreDraw();

	// 描画処理ここから
	//postEffect_->Draw(*camera_.get());

	skydome_->Draw(camera_.get());
	stage_->Draw(camera_.get());
	postScreen_->Draw(camera_.get());

	model->Draw(*camera_.get());
	/*pointLight_->Draw(*camera_.get());
	spotLight_->Draw(*camera_.get());*/

	stageLights_->Draw(camera_.get());

	NotesList::GetInstance()->Draw(camera_.get());

	UIDraw();
	BlackDraw();

	// フレームの終了
	Kyoko::PostDraw();
}

