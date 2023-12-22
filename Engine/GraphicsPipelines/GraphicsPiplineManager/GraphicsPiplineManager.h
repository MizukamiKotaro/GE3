#pragma once

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include <string>
#include <d3d12.h>
#include <array>
#include "GraphicsPipelines/ModelGraphicsPipline/ModelGraphicsPipline.h"
#include "GraphicsPipelines/ParticleGraphicsPipeline/ParticleGraphicsPipeline.h"
#include "GraphicsPipelines/SpirteGraphicsPipeline/SpriteGraphicsPipeline.h"
#include "GraphicsPipelines/BlockGraphicsPipeline/BlockGraphicsPipeline.h"

enum class BlendMode
{
	kBlendModeNone, // ブレンドなし
	kBlendModeNormal, // デフォルト
	kBlendModeAdd, // 加算
	kBlendModeSubtract, // 減算
	kBlendModeMultiply, // 乗算
	kBlendModeScreen, // スクリーン
};

// スプライト共通部分
class GraphicsPiplineManager
{
public:

	enum class PiplineType {
		SPRITE,
		MODEL,
		PARTICLE,
		BLOCKS,
		COUNT_PIPLINE_TYPE,
	};

	static GraphicsPiplineManager* GetInstance();

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Init();

	void PreDraw(PiplineType type);

	void SetBlendMode(PiplineType type, uint32_t blendMode);

private:

private:
	GraphicsPiplineManager() = default;
	~GraphicsPiplineManager() = default;
	GraphicsPiplineManager(const GraphicsPiplineManager&) = delete;
	GraphicsPiplineManager& operator=(const GraphicsPiplineManager&) = delete;

private:

	SpriteGraphicsPipeline* spriteGPS_ = nullptr;
	ModelGraphicsPipline* modelGPS_ = nullptr;
	ParticleGraphicsPipeline* particleGPS_ = nullptr;
	BlockGraphicsPipeline* blocksGPS_ = nullptr;

	PiplineType currentPiplineType_ = PiplineType::SPRITE;

};

