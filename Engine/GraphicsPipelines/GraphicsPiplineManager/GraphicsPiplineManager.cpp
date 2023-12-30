#include "GraphicsPiplineManager.h"
#include "Engine/Base/DirectXCommon/DirectXCommon.h"
#include <cassert>
#include "Engine/Base/DebugLog/DebugLog.h"
#include <format>

GraphicsPiplineManager* GraphicsPiplineManager::GetInstance()
{
	static GraphicsPiplineManager instance;
	return &instance;
}

void GraphicsPiplineManager::Init()
{

	spritePSO_ = SpriteGraphicsPipeline::GetInstance();
	spritePSO_->Initialize();

	modelPSO_ = ModelGraphicsPipline::GetInstance();
	modelPSO_->Initialize();

	particlePSO_ = ParticleGraphicsPipeline::GetInstance();
	particlePSO_->Initialize();

	pointLightPSO_ = PointLightGraphicsPipline::GetInstance();
	pointLightPSO_->Initialize();

	spotLightPSO_ = SpotLightGraphicsPipline::GetInstance();
	spotLightPSO_->Initialize();

	spritePSO_->PreDraw();
}

void GraphicsPiplineManager::PreDraw()
{
	currentPiplineType_ = PiplineType::SPRITE;

	spritePSO_->PreDraw();
}

void GraphicsPiplineManager::PreDraw(PiplineType type)
{
	if (currentPiplineType_ != type) {
		currentPiplineType_ = type;

		switch (type)
		{
		case GraphicsPiplineManager::PiplineType::SPRITE:
			spritePSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::MODEL:
			modelPSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::PARTICLE:
			particlePSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::POINT_LIGHT:
			pointLightPSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::SPOT_LIGHT:
			spotLightPSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::COUNT_PIPLINE_TYPE:
			break;
		default:
			break;
		}
	}
}

void GraphicsPiplineManager::SetBlendMode(PiplineType type, uint32_t blendMode)
{
	switch (type)
	{
	case GraphicsPiplineManager::PiplineType::SPRITE:
		spritePSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::MODEL:
		modelPSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::PARTICLE:
		particlePSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::POINT_LIGHT:
		pointLightPSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::SPOT_LIGHT:
		spotLightPSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::COUNT_PIPLINE_TYPE:
		break;
	default:
		break;
	}
}

