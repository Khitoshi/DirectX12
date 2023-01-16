#pragma once
#include "d3dx12.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class PipelineState
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	PipelineState();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~PipelineState();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="desc"></param>
	void Init(GraphicsEngine* graphicsEngine, D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);
	void Init(GraphicsEngine* graphicsEngine, D3D12_COMPUTE_PIPELINE_STATE_DESC desc);

public:
ID3D12PipelineState* GetPipelineState() { return this->pipeline_State_.Get(); }

private:
	//パイプライン ステート
	ComPtr<ID3D12PipelineState> pipeline_State_;

};