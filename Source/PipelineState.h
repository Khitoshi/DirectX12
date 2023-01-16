#pragma once
#include "d3dx12.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class PipelineState
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	PipelineState();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~PipelineState();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="desc"></param>
	void Init(GraphicsEngine* graphicsEngine, D3D12_GRAPHICS_PIPELINE_STATE_DESC desc);
	void Init(GraphicsEngine* graphicsEngine, D3D12_COMPUTE_PIPELINE_STATE_DESC desc);

public:
ID3D12PipelineState* GetPipelineState() { return this->pipeline_State_.Get(); }

private:
	//�p�C�v���C�� �X�e�[�g
	ComPtr<ID3D12PipelineState> pipeline_State_;

};