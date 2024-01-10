#include "StageScene.h"
#include "ImGuiManager/ImGuiManager.h"
#include "Kyoko.h"

StageScene::StageScene()
{
	FirstInit();

	
}

void StageScene::Init()
{
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
		
		break;
	case Transition::kToBlack:
		ToBlackUpdate(CLEAR);
		break;
	default:
		break;
	}

}

void StageScene::Draw()
{

	Kyoko::PreDraw();



	
	BlackDraw();

	// フレームの終了
	Kyoko::PostDraw();
}

