#include "StageScene.h"
#include "Externals/imgui/imgui.h"
#include "FrameInfo/FrameInfo.h"
#include <calc.h>
#include "Kyoko.h"

StageScene::StageScene() {
	FirstInit();
	pBlockManager_ = BlockManager::GetInstance();

}

void StageScene::Init()
{

	camera_->transform_.translate_ = { 0.0f,2.0f,-50.0f };
	camera_->Update();

	//for (uint32_t i = 0; i < 4; i++) {
	//	blocks_[i] = BlockManager::GetInstance()->AddBox(mesh1, IBlock{});
	//	blocks_[i]->transformMat_ = Matrix4x4::MakeAffinMatrix({ 1.0f,1.0f,1.0f }, {}, { 2.0f * i,0.0f,0.0f });
	//	blocks_[i]->color_.r = 0.3f * i;
	//}

	file_.Load("Resources/LevelData/Level1.csv");
	csv_ = file_;

	stage_ = std::make_unique<Stage>();
	stage_->Init();
	stage_->LoadCSV(csv_);

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

	slot_ = std::make_unique<Slot>();
	slot_->Initialize();
	slot_->PostEffectWright(camera_.get());
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

	ImGui::Begin("a");
	ImGui::SliderFloat3("cameraTra", &camera_->transform_.translate_.x, -100.0f, 100.0f);
	ImGui::SliderFloat3("cameraRot", &camera_->transform_.rotate_.x, -3.14f, 3.14f);
	ImGui::End();

	ImGui::Begin("StageEditor");
	stage_->ImGuiWidget();
	ImGui::End();
#endif // _DEBUG

	camera_->Update();
	stage_->Update(deltaTime);


	player_->InputAction(input_, deltaTime);
	player_->Update(deltaTime);

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

	for (const auto &sword : *stage_->GetSwordList()) {
		auto *collision = sword->GetCollision();
		if (collision) {
			if (Collision::IsCollision(player_->GetSphere(), *collision)) {
				player_->OnCollision(sword.get());
				sword->OnCollision(player_.get());
			}
		}
	}

	stageUI_->Update();

	slot_->Update(camera_.get());
}

void StageScene::Draw() {


	// 描画開始
	Kyoko::PreDraw();

	pBlockManager_->clear();
	stage_->Draw();

	player_->Draw();

	slot_->Draw(camera_.get());

	pBlockManager_->Draw(*camera_.get());

	stageUI_->Draw(*camera_.get());


	// フレームの終了
	BlackDraw();
	Kyoko::PostDraw();
}
