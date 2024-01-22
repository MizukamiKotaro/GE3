#pragma once
#include <list>
#include <unordered_map>
#include "Matrix4x4.h"
#include "Vector4.h"
#include <memory>
#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include "Utils/SoUtils/Graphics/Color.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"
#include "Light/DirectionalLight/DirectionalLight.h"
#include "DescriptorHeapManager/DescriptorHeap/DescriptorHeap.h"

class Camera;

class IBlock {
public: // コンストラクタ


public: // パブリックなメンバ変数

	// トランスフォーム
	Matrix4x4 transformMat_;

	//　色
	SoLib::Color::RGB4 color_ = { 1.0f,1.0f,1.0f,1.0f };

};

class Blocks {
public:

	static const uint32_t kNumInstance = 10000;

	Blocks(const std::string& fileName);
	Blocks(uint32_t modelHundle);
	~Blocks();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Draw(const Camera& camera, std::list<IBlock>& blocks, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDrow() { GraphicsPiplineManager::GetInstance()->PreDraw(piplineType); }

	void SetMesh(uint32_t mesh);

	void SetDirectionalLight(const DirectionalLight* light) { directionalLight_ = light; }

private:

	void CreateSRV();

private:
	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> instancingResource_;
	ParticleForGPU* instancingData_;

	const DirectionalLight* directionalLight_ = nullptr;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::BLOCKS;

	uint32_t meshHundle_;

	uint32_t textureHundle_;

	const DescriptorHandles* srvHandles_;
};

/// <summary>
/// ブロックリスト
/// </summary>
class BlockList {
public: // メンバ関数

	BlockList(uint32_t meshHundle);

	void Draw(const Camera& camera);

public: // アクセッサ等

	void clear() { blocks_.clear(); }
	/// <summary>
	/// 粒子配列に粒子を追加する関数
	/// </summary>
	/// <param name="particle">粒子</param>
	/// <returns>配列に追加された粒子</returns>
	IBlock* const push_back(IBlock&& particle);

	/// <summary>
	/// 粒子配列のリストゲッター
	/// </summary>
	/// <returns>粒子配列</returns>
	const auto& GetBlockList() { return blocks_; }

	/// <summary>
	/// 粒子配列サイズゲッター
	/// </summary>
	/// <returns></returns>
	uint32_t size() const { return static_cast<uint32_t>(blocks_.size()); }

	/// <summary>
	/// 粒子配列の始端情報ゲッター
	/// </summary>
	/// <returns>粒子配列始端情報</returns>
	auto begin() { return blocks_.begin(); }
	/// <summary>
	/// 粒子配列始端情報ゲッター（定数値）
	/// </summary>
	/// <returns>粒子配列始端情報（定数）</returns>
	auto begin() const { return blocks_.cbegin(); }

	/// <summary>
	/// 粒子配列終端情報ゲッター
	/// </summary>
	/// <returns>粒子配列終端情報</returns>
	auto end() { return blocks_.end(); }
	/// <summary>
	/// 粒子配列終端情報ゲッター(定数値)
	/// </summary>
	/// <returns>粒子配列終端情報(定数)</returns>
	auto end() const { return blocks_.cend(); }

	/// <summary>
	/// パーティクルに使用するモデルセッター
	/// </summary>
	/// <param name="model">パーティクルに使用するモデル</param>
	void SetModel(uint32_t meshHundle) { blocksResource_->SetMesh(meshHundle); }
	//void SetModel(const Model* const model) { model_ = model; }

	/*/// <summary>
	/// 描画インデックス位置セッター
	/// </summary>
	/// <param name="location">描画インデックス位置</param>
	void SetLocation(const uint32_t location) { indexLocation_ = location; }
	/// <summary>
	/// 描画インデックス位置ゲッター
	/// </summary>
	/// <returns>描画インデックス位置</returns>
	const auto& GetLoaction() const { return indexLocation_; }*/

	void SetDirectionalLight(const DirectionalLight* light) { blocksResource_->SetDirectionalLight(light); }

private: // メンバ変数

	/*==================================
		ユーザー変更禁止（マネージャーで設定）
	==================================*/
	//// 描画インデックスの場所
	//CBuffer<uint32_t> indexLocation_;

	//// ブロックモデル
	//const Model* model_;

	std::unique_ptr<Blocks> blocksResource_;

	// ブロック配列
	std::list<IBlock> blocks_;
};

/// <summary>
/// ステージ上のブロックをインスタンシングで描画するマネージャ
/// </summary>
class BlockManager
{
private: // コンストラクタ等

	// シングルトンパターンの設定
	BlockManager() = default;
	~BlockManager() = default;
	BlockManager(const BlockManager&) = delete;
	const BlockManager& operator=(const BlockManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static BlockManager* GetInstance() {
		static BlockManager instance;
		return &instance;
	};

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="maxCount">最大数</param>
	//void Init(uint32_t maxCount);

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	IBlock* const AddBox(uint32_t modelhundole, IBlock&& block) {
		decltype(blockMap_)::iterator blockListItr = blockMap_.find(modelhundole);
		if (blockListItr == blockMap_.end()) {
			blockMap_[modelhundole] = std::make_unique<BlockList>(modelhundole);
			blockMap_[modelhundole]->SetModel(modelhundole);
		}
		return blockMap_[modelhundole]->push_back(std::move(block));
	}

	/// @brief 破棄関数
	void clear() {
		for (auto& blockList : blockMap_) {
			blockList.second->clear();
		}
	}

	void SetDirectionalLight(uint32_t modelhundole, const DirectionalLight* light) { 
		decltype(blockMap_)::iterator blockListItr = blockMap_.find(modelhundole);
		blockMap_[modelhundole]->SetDirectionalLight(light);
	}

private: // メンバ変数

	// ブロック配列
	//ArrayBuffer<Particle::ParticleData> blocks_;

	//// ヒープレンジ
	//DescHeap<D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV>::HeapRange heapRange_;

	// ブロックリスト配列
	std::unordered_map<uint32_t, std::unique_ptr<BlockList>> blockMap_;

};