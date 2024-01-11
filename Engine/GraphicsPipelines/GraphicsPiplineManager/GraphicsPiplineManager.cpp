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

	spriteGPS_ = SpriteGraphicsPipeline::GetInstance();
	spriteGPS_->Initialize();

	modelGPS_ = ModelGraphicsPipline::GetInstance();
	modelGPS_->Initialize();

	particleGPS_ = ParticleGraphicsPipeline::GetInstance();
	particleGPS_->Initialize();

	pointLightPSO_ = PointLightGraphicsPipline::GetInstance();
	pointLightPSO_->Initialize();

	spotLightPSO_ = SpotLightGraphicsPipline::GetInstance();
	spotLightPSO_->Initialize();

	blocksGPS_ = BlockGraphicsPipeline::GetInstance();
	blocksGPS_->Initialize();

	spriteGPS_->PreDraw();
}

void GraphicsPiplineManager::PreDraw(PiplineType type)
{
	if (currentPiplineType_ != type) {
		currentPiplineType_ = type;

		switch (type)
		{
		case GraphicsPiplineManager::PiplineType::SPRITE:
			spriteGPS_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::MODEL:
			modelGPS_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::PARTICLE:
			particleGPS_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::POINT_LIGHT:
			pointLightPSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::SPOT_LIGHT:
			spotLightPSO_->PreDraw();
			break;
		case GraphicsPiplineManager::PiplineType::BLOCKS:
			blocksGPS_->PreDraw();
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
		spriteGPS_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::MODEL:
		modelGPS_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::PARTICLE:
		particleGPS_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::POINT_LIGHT:
		pointLightPSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::SPOT_LIGHT:
		spotLightPSO_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::BLOCKS:
		blocksGPS_->SetBlendMode(blendMode);
		break;
	case GraphicsPiplineManager::PiplineType::COUNT_PIPLINE_TYPE:
		break;
	default:
		break;
	}
}

