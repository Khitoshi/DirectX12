#include "PipelineState.h"

#include "GraphicsEngine.h"

PipelineState::PipelineState():
    pipeline_State_()
{
}

PipelineState::~PipelineState()
{
}

//初期化
void PipelineState::Init(GraphicsEngine*& graphicsEngine, D3D12_GRAPHICS_PIPELINE_STATE_DESC desc)
{
    //パイプライン ステート 生成
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
    //    MessageBoxA(nullptr, "パイプラインステートの作成に失敗しました。\n", "エラー", MB_OK);
    //    std::abort();
    //    std::abort();
    //}
}

//初期化
void PipelineState::Init(GraphicsEngine*& graphicsEngine, D3D12_COMPUTE_PIPELINE_STATE_DESC desc)
{
    //パイプライン ステート 生成
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
    //    MessageBoxA(nullptr, "パイプラインステートの作成に失敗しました。\n", "エラー", MB_OK);
    //    std::abort();
    //    std::abort();
    //}
}
