#pragma once

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include <string>
#include <vector>
#include <d3d12.h>
#include "Utils/Math/Vector2.h"
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Vector4.h"

// スプライト共通部分
class ParticleCommon
{
public:

	struct VertexData
	{
		Vector4 vertexPos;
		Vector2 texcoord;
		Vector3 normal;
	};

	struct MaterialData
	{
		std::string textureFilePath;
	};

	struct MeshData 
	{
		std::vector<VertexData> verteces;
		MaterialData material;
		std::string directoryPath;
		std::string fileName;
		uint32_t textureHundle_;
	};

	static ParticleCommon* GetInstance();

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void PreDraw();

	uint32_t LoadObj(const std::string& directoryPath, const std::string& fileName);

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);

	uint32_t GetTextureHundle(uint32_t hundle) { return meshs_[hundle].textureHundle_; }

	MeshData* GetModelData(uint32_t hundle) { return &meshs_[hundle]; }

private:

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& fileName);

	MeshData LoadObjeFile(const std::string& directoryPath, const std::string& fileName);

private:

	ID3D12Device* device_;
	ID3D12GraphicsCommandList* commandList_;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3DBlob> signatureBlob_;
	ComPtr<ID3DBlob> errorBlob_;
	ComPtr<ID3D12RootSignature> rootSignature_;

	ComPtr<IDxcBlob> vertexShaderBlob_;
	ComPtr<IDxcBlob> pixelShaderBlob_;

	ComPtr<ID3D12PipelineState> graphicsPipelineState_;

private:
	ParticleCommon() = default;
	~ParticleCommon() = default;
	ParticleCommon(const ParticleCommon&) = delete;
	ParticleCommon& operator=(const ParticleCommon&) = delete;

	void InitializeDXC();

	void InitializePSO();

	//コンパイル用関数
	IDxcBlob* CompileShader(
		//compilerするShaderファイルへパス
		const std::wstring& filePath,
		//compilerに使用するprofile
		const wchar_t* profile,
		//初期化で生成したもの3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

private:

	std::vector<MeshData> meshs_;

};

