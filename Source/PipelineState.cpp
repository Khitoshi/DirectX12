#include "PipelineState.h"

#include "GraphicsEngine.h"

PipelineState::PipelineState():
    pipeline_State_()
{
}

PipelineState::~PipelineState()
{
}

//������
void PipelineState::Init(GraphicsEngine*& graphicsEngine, D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
    //�p�C�v���C�� �X�e�[�g ����
    //HRESULT hr = device.CreateGraphicsPipelineState(
    //    &desc,
    //    IID_PPV_ARGS(&this->pipeline_State_)
    //);

    graphicsEngine->CreateGraphicsPipelineState(
        desc,
        this->pipeline_State_
    );

    //if (FAILED(hr))
    //{
    //    MessageBoxA(nullptr, "�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂����B\n", "�G���[", MB_OK);
    //    std::abort();
    //    std::abort();
    //}
}

//������
void PipelineState::Init(GraphicsEngine*& graphicsEngine, D3D12_COMPUTE_PIPELINE_STATE_DESC desc)
{
    //�p�C�v���C�� �X�e�[�g ����
    //HRESULT hr = device.CreateComputePipelineState(
    //    &desc,
    //    IID_PPV_ARGS(&this->pipeline_State_)
    //);

    graphicsEngine->CreateComputePipelineState(
        desc, 
        this->pipeline_State_
    );

    //if (FAILED(hr))
    //{
    //    MessageBoxA(nullptr, "�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂����B\n", "�G���[", MB_OK);
    //    std::abort();
    //    std::abort();
    //}
}
