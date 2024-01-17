#pragma once

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
#include <string>
#include <d3d12.h>
#include <array>

class ContrastGraphicsPipeline
{
public:

	enum BlendMode {
		kBlendModeNone, // ブレンドなし
		kBlendModeNormal, // デフォルト
		kBlendModeAdd, // 加算
		kBlendModeSubtract, // 減算
		kBlendModeMultiply, // 乗算
		kBlendModeScreen, // スクリーン
		kCountOfBlendMode, // 使わない。配列用。
	};

	static ContrastGraphicsPipeline* GetInstance();

	// namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	void Initialize();

	void PreDraw();

	void SetBlendMode(uint32_t blendMode);

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

	std::array<ComPtr<ID3D12PipelineState>, kCountOfBlendMode> graphicsPipelineStates_;

	BlendMode blendMode_ = BlendMode::kBlendModeNormal;

private:
	ContrastGraphicsPipeline() = default;
	~ContrastGraphicsPipeline() = default;
	ContrastGraphicsPipeline(const ContrastGraphicsPipeline&) = delete;
	ContrastGraphicsPipeline& operator=(const ContrastGraphicsPipeline&) = delete;

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

};

