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
bool RenderTarget::Create(GraphicsEngine* graphicsEngine, int renderTargetWidth, int renderTargetHeight, int mipLevel, int arraySize, DXGI_FORMAT colorFormat, DXGI_FORMAT depthStencilFormat, float clearColor[4])
{
    //�f�o�C�X�擾
    auto deivce = graphicsEngine->GetD3DDevice();

    //�����_�[�^�[�Q�b�g�̕��E���� �ݒ�
    this->width_ = renderTargetWidth;
    this->height_ = renderTargetHeight;
    
    //�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�� �쐬
    CreateRenderTargetTexture(
        *graphicsEngine,
        renderTargetWidth,
        renderTargetHeight,
        mipLevel,
        arraySize,
        colorFormat,
        clearColor);

    //�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�� �쐬
    if (depthStencilFormat != DXGI_FORMAT_UNKNOWN) {
        CreateDepthStencilTexture(*graphicsEngine, renderTargetWidth, renderTargetHeight, depthStencilFormat);
    }

    //�f�B�X�N���v�^�q�[�v�쐬
    CreateDescriptorHeap(*graphicsEngine);

    //�f�B�X�N���v�^���쐬����
    auto device = graphicsEngine->GetD3DDevice();
    CreateDescriptor(deivce);
    if (clearColor) {
        memcpy(this->rtv_Clear_Color_, clearColor, sizeof(this->rtv_Clear_Color_));
    }

    return true;
}

//�f�B�X�N���v�^�q�[�v���쐬
void RenderTarget::CreateDescriptorHeap(GraphicsEngine& graphicsEngine)
{
    auto device = graphicsEngine.GetD3DDevice();

    //RTV�p�̃f�B�X�N���v�^�q�[�v �쐬
    //�ݒ�
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = GraphicsEngine::GetFrameBufferCount();
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    //����
    device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->rtv_Heap_));

    //���� �m�F
    if (this->rtv_Heap_ == nullptr) {
        //RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s����
        MessageBoxA(nullptr, "RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s", "�G���[", MB_OK);
        //�ُ�I��
        std::abort();
    }

    //�f�B�X�N���v�^�̃T�C�Y���擾�B
    this->rtv_Descriptor_Size_ = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    //�[�x�X�e���V���e�N�X�`�������݂��鎞
    if (this->depth_Stencil_Texture_) {
        //DSV�p�̃f�B�X�N���v�^�q�[�v���쐬
        desc.NumDescriptors = 1;
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

        //����
        device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));

        //���� �m�F
        if (this->dsv_Heap_ == nullptr) {
            //DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s����
            MessageBoxA(nullptr, "DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s", "�G���[", MB_OK);
            //�ُ�I��
            std::abort();
        }
        //�f�B�X�N���v�^�̃T�C�Y���擾�B
        this->dsv_Descriptor_Size_ = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    }
}

//�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬
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
    //�f�o�C�X �擾
    auto device = graphicsEngine.GetD3DDevice();

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
    //���\�[�X �쐬
    auto prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    auto hr = device.CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_COMMON,
        &clear_value,
        IID_PPV_ARGS(&this->render_Target_Texture_Dx12_)
    );

    //�쐬 �m�F
    if (FAILED(hr)) {
        //DSV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s����
        MessageBoxA(nullptr, "���\�[�X�̍쐬�Ɏ��s", "�G���[", MB_OK);
        //�ُ�I��
        std::abort();
    }

    this->render_Target_Texture_.InitFromD3DResource(this->render_Target_Texture_Dx12_);
}

//�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�����쐬
void RenderTarget::CreateDepthStencilTexture(
    GraphicsEngine& graphicsEngine, 
    int textureWidth, 
    int textureHeight, 
    DXGI_FORMAT format)
{
    //�f�o�C�X�擾 
    auto device = graphicsEngine.GetD3DDevice();

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
    auto hr = device.CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsv_clear_value,
        IID_PPV_ARGS(&this->depth_Stencil_Texture_)
    );

    //���� �m�F
    if (FAILED(hr)) {
        //�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s
        MessageBoxA(nullptr, "�[�x�X�e���V���o�b�t�@�̍쐬�Ɏ��s", "�G���[", MB_OK);
        //�ُ�I��
        std::abort();
    }
}

//�f�B�X�N���v�^�̍쐬
void RenderTarget::CreateDescriptor(ID3D12Device5*& d3dDevice)
{
    //�J���[�e�N�X�`���̃f�B�X�N���v�^���쐬�B
    auto rtv_handle = this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();
    d3dDevice->CreateRenderTargetView(this->render_Target_Texture_.Get(), nullptr, rtv_handle);
    if (this->depth_Stencil_Texture_) {
        //�[�x�e�N�X�`���̃f�B�X�N���v�^���쐬
        auto dsvHandle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
        d3dDevice->CreateDepthStencilView(this->depth_Stencil_Texture_, nullptr, dsvHandle);
    }
}
