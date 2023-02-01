#include "RenderTarget.h"
#include "GraphicsEngine.h"

//�f�t�H���g �R���X�g���N�^
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

//�f�t�H���g �f�X�g���N�^
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

// �����_�����O�^�[�Q�b�g�̍쐬
bool RenderTarget::Create(GraphicsEngine*& graphicsEngine, int renderTargetWidth, int renderTargetHeight, int mipLevel, int arraySize, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat, float clearColor[4])
{
    //�����_�[�^�[�Q�b�g�̕��E���� �ݒ�
    this->width_ = renderTargetWidth;
    this->height_ = renderTargetHeight;
    
    //�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�� �쐬
    CreateRenderTargetTexture(
        graphicsEngine,
        renderTargetWidth,
        renderTargetHeight,
        mipLevel,
        arraySize,
        colorFormat,
        clearColor);

    //�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�� �쐬
    if (depthStencilFormat != DXGI_FORMAT_UNKNOWN) {
        CreateDepthStencilTexture(graphicsEngine, renderTargetWidth, renderTargetHeight, depthStencilFormat);
    }

    //RTV �f�B�X�N���v�^�q�[�v�쐬
    CreateDescriptorRTVHeap(graphicsEngine);
    //DSV �f�B�X�N���v�^�q�[�v�쐬
    CreateDescriptorDSVHeap(graphicsEngine);

    //�����_�[�^�[�Q�b�g�@�f�B�X�N���v�^�@�쐬
    CreateDescriptorRTV(graphicsEngine);
    //�[�x�X�e���V���@�f�B�X�N���v�^�@�쐬
    CreateDescriptorDSV(graphicsEngine);

    //�N���A�J���[�����݂���ꍇ�������̃R�s�[���s��
    if (clearColor) {
        memcpy(this->rtv_Clear_Color_, clearColor, sizeof(this->rtv_Clear_Color_));
    }

    return true;
}

//�����_�[�^�[�Q�b�g�r���[�p �f�B�X�N���v�^�q�[�v �쐬
void RenderTarget::CreateDescriptorRTVHeap(GraphicsEngine*& graphicsEngine)
{
    //RTV�p�̃f�B�X�N���v�^�q�[�v �쐬
    //�ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = GraphicsEngine::GetFrameBufferCount();
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    //����
    graphicsEngine->CreateDescriptorHeap(desc, this->rtv_Heap_);

    //�f�B�X�N���v�^�̃T�C�Y��ݒ�
    this->rtv_Descriptor_Size_ = graphicsEngine->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

//�[�x�X�e���V���o�b�t�@�p �f�B�X�N���v�^�q�[�v
void RenderTarget::CreateDescriptorDSVHeap(GraphicsEngine*& graphicsEngine)
{
    //�[�x�X�e���V���e�N�X�`�������݂��Ȃ��ꍇ �������^�[��
    if (!this->depth_Stencil_Texture_) return;

    //DSV�p�̃f�B�X�N���v�^�q�[�v���쐬
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


    //����
    //device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));
    graphicsEngine->CreateDescriptorHeap(desc, this->dsv_Heap_);

    //�f�B�X�N���v�^�̃T�C�Y��ݒ�
    this->dsv_Descriptor_Size_ = graphicsEngine->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    
}

//�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬
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
    //���\�[�X��ݒ�
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

    //�q�[�v�̐ݒ�
    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    //���\�[�X �쐬
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

//�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�����쐬
void RenderTarget::CreateDepthStencilTexture(
    GraphicsEngine*& graphicsEngine,
    int textureWidth, 
    int textureHeight, 
    DXGI_FORMAT format)
{
    //�[�x�X�e���V���̒l �ݒ�
    D3D12_CLEAR_VALUE dsv_clear_value;
    dsv_clear_value.Format = format;
    dsv_clear_value.DepthStencil.Depth = 1.0f;
    dsv_clear_value.DepthStencil.Stencil = 0;

    //���\�[�X�̐ݒ�
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

    //���\�[�X����
    graphicsEngine->CreateCommittedResource(
        prop,
        D3D12_HEAP_FLAG_NONE,
        desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsv_clear_value,
        this->depth_Stencil_Texture_
    );
}

//�����_�[�^�[�Q�b�g�r���[�̃f�B�X�N���v�^ �쐬
void RenderTarget::CreateDescriptorRTV(GraphicsEngine*& graphicsEngine)
{
    //�J���[�e�N�X�`���̃f�B�X�N���v�^���쐬�B
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

// �e�N�X�`��������ꍇ
// �[�x�X�e���V���r���[�̃f�B�X�N���v�^ �쐬
void RenderTarget::CreateDescriptorDSV(GraphicsEngine*& graphicsEngine)
{
    //�[�x�X�e���V���e�N�X�`�������݂��Ȃ��ꍇ�@�������^�[��
    if (this->depth_Stencil_Texture_ == nullptr)return;
    
    //�[�x�e�N�X�`���̃f�B�X�N���v�^���쐬
    auto dsvHandle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
    graphicsEngine->CreateDepthStencilView(
        this->depth_Stencil_Texture_,
        nullptr,
        dsvHandle
    );
    
}
