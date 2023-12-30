#include "SceneManager.h"
#include "Kyoko.h"
#include "Input.h"

SceneManager::SceneManager()
{
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	//IScene::sceneNo_ = TITLE;
	IScene::sceneNo_ = STAGE;
	currentSceneNo_ = IScene::sceneNo_;
	IScene::stagrNo_ = 0;

	sceneArr_[currentSceneNo_]->Init();

	postEffect_ = std::make_unique<PostEffect>("Resources/uvChecker.png");
	postEffect_->Update();

	camera_ = std::make_unique<Camera>();
	camera_->Initialize();
}

SceneManager::~SceneManager()
{
}

int SceneManager::Run()
{
	// ウィンドウの×ボタンが押されるまでループ
	while (true) {
		// フレームの開始
		if (Kyoko::ProcessMessage() || (Input::GetInstance()->PressedKey(DIK_ESCAPE) && IScene::sceneNo_ == TITLE)) {
			break;
		}

		Kyoko::FirstUpdateInLoop();

		// 更新処理
		preSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();

		if (preSceneNo_ != currentSceneNo_) {
			sceneArr_[currentSceneNo_]->Init();
		}

		sceneArr_[currentSceneNo_]->Update();
		
		postEffect_->PreDrawScene();

		sceneArr_[currentSceneNo_]->Draw();

		postEffect_->PostDrawScene();

		Kyoko::PreDraw();

		// 描画処理ここから
		postEffect_->Draw(*camera_.get());

		// フレームの終了
		Kyoko::PostDraw();
	}

	return 0;
}
