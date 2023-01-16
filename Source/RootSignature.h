#pragma once
#include <d3dx12.h>
#include<wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class RootSignature
{
public:
	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	RootSignature();

	/// <summary>
	/// デフォルト デストラク
	/// </summary>
	~RootSignature();

	bool Init(
		GraphicsEngine* graphicsEngine,
		D3D12_FILTER samplerFilter,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeU,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeV,
		D3D12_TEXTURE_ADDRESS_MODE textureAdressModeW,
		UINT maxCbvDescriptor = 8,
		UINT maxSrvDescriptor = 32,
		UINT maxUavDescritor = 8
	);
	
	bool Init(
		GraphicsEngine* graphicsEngine,
		D3D12_STATIC_SAMPLER_DESC* samplerDescArray,
		int numSampler,
		UINT maxCbvDescriptor = 8,
		UINT maxSrvDescriptor = 32,
		UINT maxUavDescritor = 8,
		UINT offsetInDescriptorsFromTableStartCB = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		UINT offsetInDescriptorsFromTableStartSRV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		UINT offsetInDescriptorsFromTableStartUAV = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
	);

public://get method

	ID3D12RootSignature* GetRootSignature() { return this->root_Signature_.Get(); }

private:
	ComPtr<ID3D12RootSignature> root_Signature_;
};
