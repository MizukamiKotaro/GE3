#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <stdint.h>
#include <string>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector4.h"
#include "Utils/Math/Matrix4x4.h"
#include "Utils/Transform/Transform.h"
#include "GraphicsPipelines/GraphicsPiplineManager/GraphicsPiplineManager.h"

class Camera;

class Model
{
public:

	// Model(const std::string& fileName);
	// 例)
	// Cubeフォルダ
	//   Cube.obj
	//   Cube.mtl
	//   hoge.png
	// の状態

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="fileName">.objが入っているフォルダの名前( "Cube" 等)</param>
	Model(const std::string& fileName);

	/// <summary>
	/// モデルの生成
	/// </summary>
	/// <param name="meshHundle">モデルデータのハンドル( ModelDataManager::GetInstance()->LoadObj("Cube") 等)</param>
	Model(uint32_t meshHundle);
	~Model();

	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};

	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	struct DirectionalLight
	{
		Vector4 color; // ライトの色
		Vector3 direction; // ライトの向き
		float intensity; // 輝度
	};

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void Update();

	void Draw(const Camera& camera, BlendMode blendMode = BlendMode::kBlendModeNormal);

	static void PreDrow() { GraphicsPiplineManager::GetInstance()->PreDraw(piplineType); }

public:

	void SetTex(uint32_t hundle) { textureHundle_ = hundle; }

	void SetMesh(uint32_t hundle);

private:

	ComPtr<ID3D12Resource> materialResource_;
	Material* materialData_;

	ComPtr<ID3D12Resource> instancingResource_;
	TransformationMatrix* instancingData_;

	ComPtr<ID3D12Resource> directionalLightResource_;
	DirectionalLight* directionalLightData_;


public:

	Transform transform_;

private:

	static const GraphicsPiplineManager::PiplineType piplineType = GraphicsPiplineManager::PiplineType::MODEL;

	Matrix4x4 uvMatrix_;

	Vector3 uvScale_;
	Vector3 uvRotate_;
	Vector3 uvPos_;

	uint32_t meshHundle_;

	uint32_t textureHundle_;

};
