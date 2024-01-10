#include "ClearScene.h"
#include "Kyoko.h"

ClearScene::ClearScene()
{
	FirstInit();

	
}

void ClearScene::Init()
{

}

void ClearScene::Update()
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
		if (input_->PressedKey(DIK_SPACE)) {
			transitionRequest_ = Transition::kToBlack;
		}
		break;
	case Transition::kToBlack:
		ToBlackUpdate(TITLE);
		break;
	default:
		break;
	}
}

void ClearScene::Draw()
{
	Kyoko::PreDraw();

	

	BlackDraw();

	Kyoko::PostDraw();
}


