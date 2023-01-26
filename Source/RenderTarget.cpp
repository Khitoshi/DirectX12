#include "RenderTarget.h"
#include "GraphicsEngine.h"

//デフォルト コンストラクタ
RenderTarget::RenderTarget() :
    render_Target_Texture_(),
    render_Target_Texture_Dx12_(nullptr),
    depth_Stencil_Texture_(nullptr),

    rtv_Heap_(nullptr),
    dsv_Heap_(nullptr),

    rtv_Descriptor_Size_(0),
    dsv_Descriptor_Size_(0),

    width_(0),
    height_(0),

    rtv_Clear_Color_(),
    dsv_Clear_Value_(1.0f)
{
}

//デフォルト デストラクタ
RenderTarget::~RenderTarget()
{
    if (render_Target_Texture_Dx12_ != nullptr)
    {
        render_Target_Texture_Dx12_->Release();
    }

    if (depth_Stencil_Texture_ != nullptr)
    {
        depth_Stencil_Texture_->Release();
    }

    if (rtv_Heap_ != nullptr)
    {
        rtv_Heap_->Release();
    }

    if (dsv_Heap_ != nullptr)
    {
        dsv_Heap_->Release();
    }
}

// レンダリングターゲットの作成
bool RenderTarget::Create(GraphicsEngine* graphicsEngine, int renderTargetWidth, int renderTargetHeight, int mipLevel, int arraySize, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat, float clearColor[4])
{
    //デバイス取得
    auto deivce = graphicsEngine->GetD3DDevice();

    //レンダーターゲットの幅・高さ 設定
    this->width_ = renderTargetWidth;
    this->height_ = renderTargetHeight;
    
    //レンダリングターゲットとなるテクスチャ 作成
    CreateRenderTargetTexture(
        *graphicsEngine,
        renderTargetWidth,
        renderTargetHeight,
        mipLevel,
        arraySize,
        colorFormat,
        clearColor);

    //深度ステンシルバッファとなるテクスチャ 作成
    if (depthStencilFormat != DXGI_FORMAT_UNKNOWN) {
        CreateDepthStencilTexture(*graphicsEngine, renderTargetWidth, renderTargetHeight, depthStencilFormat);
    }

    //ディスクリプタヒープ作成
    CreateDescriptorHeap(*graphicsEngine);

    //ディスクリプタを作成する
    auto device = graphicsEngine->GetD3DDevice();
    CreateDescriptor(deivce);
    if (clearColor) {
        memcpy(this->rtv_Clear_Color_, clearColor, sizeof(this->rtv_Clear_Color_));
    }

    return true;
}

//ディスクリプタヒープを作成
void RenderTarget::CreateDescriptorHeap(GraphicsEngine& graphicsEngine)
{
    auto device = graphicsEngine.GetD3DDevice();

    //RTV用のディスクリプタヒープ 作成
    //設定
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = GraphicsEngine::GetFrameBufferCount();
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    //生成
    device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->rtv_Heap_));

    //生成 確認
    if (this->rtv_Heap_ == nullptr) {
        //RTV用のディスクリプタヒープの作成に失敗した
        MessageBoxA(nullptr, "RTV用のディスクリプタヒープの作成に失敗", "エラー", MB_OK);
        //異常終了
        std::abort();
    }

    //ディスクリプタのサイズを取得。
    this->rtv_Descriptor_Size_ = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    //深度ステンシルテクスチャが存在する時
    if (this->depth_Stencil_Texture_) {
        //DSV用のディスクリプタヒープを作成
        desc.NumDescriptors = 1;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

        //生成
        device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));

        //生成 確認
        if (this->dsv_Heap_ == nullptr) {
            //DSV用のディスクリプタヒープの作成に失敗した
            MessageBoxA(nullptr, "DSV用のディスクリプタヒープの作成に失敗", "エラー", MB_OK);
            //異常終了
            std::abort();
        }
        //ディスクリプタのサイズを取得。
        this->dsv_Descriptor_Size_ = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }
}

//レンダリングターゲットとなるテクスチャを作成
void RenderTarget::CreateRenderTargetTexture(
    GraphicsEngine& graphicsEngine, 
    int textureWidth, 
    int textureHeight, 
    int mipLevel, 
    int arraySize, 
    DXGI_FORMAT format, 
    float clearColor[4]
)
{
    //デバイス 取得
    auto device = graphicsEngine.GetD3DDevice();

    //リソースを設定
    CD3DX12_RESOURCE_DESC desc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        0,
        static_cast<UINT>(textureWidth),
        static_cast<UINT>(textureHeight),
        arraySize,
        mipLevel,
        format,
        1,
        0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
    );

    D3D12_CLEAR_VALUE clear_value;
    clear_value.Format = format;
    if (clearColor != nullptr) {
        clear_value.Color[0] = clearColor[0];
        clear_value.Color[1] = clearColor[1];
        clear_value.Color[2] = clearColor[2];
        clear_value.Color[3] = clearColor[3];
    }
    else {
        clear_value.Color[0] = 0.0f;
        clear_value.Color[1] = 0.0f;
        clear_value.Color[2] = 0.0f;
        clear_value.Color[3] = 1.0f;
    }
    //リソース 作成
    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto hr = device.CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COMMON,
        &clear_value,
        IID_PPV_ARGS(&this->render_Target_Texture_Dx12_)
    );

    //作成 確認
    if (FAILED(hr)) {
        //DSV用のディスクリプタヒープの作成に失敗した
        MessageBoxA(nullptr, "リソースの作成に失敗", "エラー", MB_OK);
        //異常終了
        std::abort();
    }

    this->render_Target_Texture_.InitFromD3DResource(this->render_Target_Texture_Dx12_);
}

//深度ステンシルバッファとなるテクスチャを作成
void RenderTarget::CreateDepthStencilTexture(
    GraphicsEngine& graphicsEngine, 
    int textureWidth, 
    int textureHeight, 
    DXGI_FORMAT format)
{
    //デバイス取得 
    auto device = graphicsEngine.GetD3DDevice();

    //深度ステンシルの値 設定
    D3D12_CLEAR_VALUE dsv_clear_value;
    dsv_clear_value.Format = format;
    dsv_clear_value.DepthStencil.Depth = 1.0f;
    dsv_clear_value.DepthStencil.Stencil = 0;

    //リソースの設定
    CD3DX12_RESOURCE_DESC desc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        0,
        static_cast<UINT>(textureWidth),
        static_cast<UINT>(textureHeight),
        1,
        1,
        format,
        1,
        0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    //リソース生成
    auto hr = device.CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsv_clear_value,
        IID_PPV_ARGS(&this->depth_Stencil_Texture_)
    );

    //生成 確認
    if (FAILED(hr)) {
        //深度ステンシルバッファの作成に失敗
        MessageBoxA(nullptr, "深度ステンシルバッファの作成に失敗", "エラー", MB_OK);
        //異常終了
        std::abort();
    }
}

//ディスクリプタの作成
void RenderTarget::CreateDescriptor(ID3D12Device5*& d3dDevice)
{
    //カラーテクスチャのディスクリプタを作成。
    auto rtv_handle = this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();
    d3dDevice->CreateRenderTargetView(this->render_Target_Texture_.Get(), nullptr, rtv_handle);
    if (this->depth_Stencil_Texture_) {
        //深度テクスチャのディスクリプタを作成
        auto dsvHandle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
        d3dDevice->CreateDepthStencilView(this->depth_Stencil_Texture_, nullptr, dsvHandle);
    }
}
