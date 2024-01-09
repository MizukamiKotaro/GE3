#include "SceneManager.h"
#include "Kyoko.h"
#include "Input.h"
#include "FrameInfo/FrameInfo.h"

SceneManager::SceneManager()
{
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[SELECT] = std::make_unique<SelectScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();

	IScene::sceneNo_ = TITLE;
	//IScene::sceneNo_ = STAGE;
	currentSceneNo_ = IScene::sceneNo_;
	IScene::stagrNo_ = SHINING_STAR;

	sceneArr_[currentSceneNo_]->Init();

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

		//Kyoko::PreDraw();

		// 描画処理ここから
		
		sceneArr_[currentSceneNo_]->Draw();

		// フレームの終了
		//Kyoko::PostDraw();

		FrameInfo::GetInstance()->End();
	}

	return 0;
}
