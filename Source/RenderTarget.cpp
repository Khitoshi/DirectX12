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
bool RenderTarget::Create(GraphicsEngine*& graphicsEngine, int renderTargetWidth, int renderTargetHeight, int mipLevel, int arraySize, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat, float clearColor[4])
{
    //レンダーターゲットの幅・高さ 設定
    this->width_ = renderTargetWidth;
    this->height_ = renderTargetHeight;
    
    //レンダリングターゲットとなるテクスチャ 作成
    CreateRenderTargetTexture(
        graphicsEngine,
        renderTargetWidth,
        renderTargetHeight,
        mipLevel,
        arraySize,
        colorFormat,
        clearColor);

    //深度ステンシルバッファとなるテクスチャ 作成
    if (depthStencilFormat != DXGI_FORMAT_UNKNOWN) {
        CreateDepthStencilTexture(graphicsEngine, renderTargetWidth, renderTargetHeight, depthStencilFormat);
    }

    //RTV ディスクリプタヒープ作成
    CreateDescriptorRTVHeap(graphicsEngine);
    //DSV ディスクリプタヒープ作成
    CreateDescriptorDSVHeap(graphicsEngine);

    //レンダーターゲット　ディスクリプタ　作成
    CreateDescriptorRTV(graphicsEngine);
    //深度ステンシル　ディスクリプタ　作成
    CreateDescriptorDSV(graphicsEngine);

    //クリアカラーが存在する場合メモリのコピーを行う
    if (clearColor) {
        memcpy(this->rtv_Clear_Color_, clearColor, sizeof(this->rtv_Clear_Color_));
    }

    return true;
}

//レンダーターゲットビュー用 ディスクリプタヒープ 作成
void RenderTarget::CreateDescriptorRTVHeap(GraphicsEngine*& graphicsEngine)
{
    //RTV用のディスクリプタヒープ 作成
    //設定
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = GraphicsEngine::GetFrameBufferCount();
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    //生成
    graphicsEngine->CreateDescriptorHeap(desc, this->rtv_Heap_);

    //ディスクリプタのサイズを設定
    this->rtv_Descriptor_Size_ = graphicsEngine->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

//深度ステンシルバッファ用 ディスクリプタヒープ
void RenderTarget::CreateDescriptorDSVHeap(GraphicsEngine*& graphicsEngine)
{
    //深度ステンシルテクスチャが存在しない場合 早期リターン
    if (!this->depth_Stencil_Texture_) return;

    //DSV用のディスクリプタヒープを作成
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


    //生成
    //device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));
    graphicsEngine->CreateDescriptorHeap(desc, this->dsv_Heap_);

    //ディスクリプタのサイズを設定
    this->dsv_Descriptor_Size_ = graphicsEngine->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    
}

//レンダリングターゲットとなるテクスチャを作成
void RenderTarget::CreateRenderTargetTexture(
    GraphicsEngine*& graphicsEngine, 
    int textureWidth, 
    int textureHeight, 
    int mipLevel, 
    int arraySize, 
    DXGI_FORMAT format, 
    float clearColor[4]
)
{
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

    //ヒープの設定
    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    //リソース 作成
    graphicsEngine->CreateCommittedResource(
        prop,
        D3D12_HEAP_FLAG_NONE,
        desc,
        D3D12_RESOURCE_STATE_COMMON,
        &clear_value,
        this->render_Target_Texture_Dx12_
    );

    this->render_Target_Texture_.InitFromD3DResource(this->render_Target_Texture_Dx12_);
}

//深度ステンシルバッファとなるテクスチャを作成
void RenderTarget::CreateDepthStencilTexture(
    GraphicsEngine*& graphicsEngine,
    int textureWidth, 
    int textureHeight, 
    DXGI_FORMAT format)
{
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
    graphicsEngine->CreateCommittedResource(
        prop,
        D3D12_HEAP_FLAG_NONE,
        desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsv_clear_value,
        this->depth_Stencil_Texture_
    );
}

//レンダーターゲットビューのディスクリプタ 作成
void RenderTarget::CreateDescriptorRTV(GraphicsEngine*& graphicsEngine)
{
    //カラーテクスチャのディスクリプタを作成。
    auto rtv_handle = this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();
    graphicsEngine->CreateRenderTargetView(
        this->render_Target_Texture_.GetTexture(),
        nullptr, 
        rtv_handle
    );

    //ID3D12Device* device = nullptr;
    //device->CreateRenderTargetView(
    //    this->render_Target_Texture_.GetTexture(),
    //    nullptr,
    //    rtv_handle)

}

// テクスチャがある場合
// 深度ステンシルビューのディスクリプタ 作成
void RenderTarget::CreateDescriptorDSV(GraphicsEngine*& graphicsEngine)
{
    //深度ステンシルテクスチャが存在しない場合　早期リターン
    if (this->depth_Stencil_Texture_ == nullptr)return;
    
    //深度テクスチャのディスクリプタを作成
    auto dsvHandle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
    graphicsEngine->CreateDepthStencilView(
        this->depth_Stencil_Texture_,
        nullptr,
        dsvHandle
    );
    
}
