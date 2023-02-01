#include "Texture.h"
#include "GraphicsEngine.h"
#include "d3dx12.h"
#include <d3d12.h>
#include <ResourceUploadBatch.h>
#include <DDSTextureLoader.h>


using namespace DirectX;

//コンストラクタ
Texture::Texture():
    texture_(nullptr),
    texture_Desc_()
{
}

//明示的 コンストラクタ
Texture::Texture(GraphicsEngine*& graphicsEngine,const wchar_t* filePath):
    texture_(nullptr),
    texture_Desc_()
{
    //初期化
    InitFromDDSFile(graphicsEngine, filePath);
}

//デストラクタ
Texture::~Texture()
{
}

//.DDS file からテクスチャを初期化する
void Texture::InitFromDDSFile(GraphicsEngine*& graphicsEngine,const wchar_t* filePath)
{
    //DDSファイルからテクスチャをロード。
    LoadTextureFromDDSFile(graphicsEngine, filePath);
}

//メモリからテクスチャ 初期化
void Texture::InitFromMemory(GraphicsEngine*& graphicsEngine,const char* memory, unsigned int size)
{
    //DDSファイルからテクスチャをロード。
    LoadTextureFromMemory(graphicsEngine,memory, size);
}

//D3Dリソースからテクスチャを初期化する
void Texture::InitFromD3DResource(ID3D12Resource*& texture)
{
    //すでに存在している場合は解放する
    if (this->texture_)texture->Release();

    //初期化
    this->texture_ = texture;
    this->texture_->AddRef();
    this->texture_Desc_ = this->texture_->GetDesc();
}

//SRVに登録
void Texture::RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo)
{
    //テクスチャが存在しない場合
    if (!this->texture_) return;

    //シェーダーリソースビュー　設定
    D3D12_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
    shader_resource_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    shader_resource_view_desc.Format = this->texture_Desc_.Format;
    shader_resource_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = this->texture_Desc_.MipLevels;
    
    ////シェーダーリソースビュー 生成
    auto device = graphicsEngine->GetDevice();
    device->CreateShaderResourceView(
        this->texture_, 
        &shader_resource_view_desc, 
        descriptorHandle);
    
    /*
    graphicsEngine->GetDevice()->CreateShaderResourceView(
        this->texture_,
        &shader_resource_view_desc,
        descriptorHandle);

    
    graphicsEngine->CreateShaderResourceView(
        this->texture_,
        shader_resource_view_desc,
        descriptorHandle);
        */
    return;
}

//.DDS file からテクスチャをロード
void Texture::LoadTextureFromDDSFile(GraphicsEngine*& graphicsEngine, const wchar_t* filePath)
{
    //ResourceUploadBatch resouce_upload_batch(device);
    ResourceUploadBatch resouce_upload_batch(graphicsEngine->GetDevice());
    resouce_upload_batch.Begin();

    ComPtr<ID3D12Resource> texture;
    
    //DDSテクスチャを生成する
    HRESULT hr = DirectX::CreateDDSTextureFromFileEx(
        //device,
        graphicsEngine->GetDevice(),
        resouce_upload_batch,
        filePath,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        0,
        &texture
    );

    //ResourceUploadBatchを終了する
    resouce_upload_batch.End(graphicsEngine->GetCommandQueue());
    
    //生成確認
    if (FAILED(hr))
    {
        //失敗したので異常終了する
        MessageBoxA(nullptr, "CreateDDSTextureFromFileExに失敗しました", "エラー", MB_OK);
        std::abort();
    }

    this->texture_ = texture.Get();
    this->texture_Desc_ = this->texture_->GetDesc();

    //texture->Release();
}

//メモリからテクスチャをロード
void Texture::LoadTextureFromMemory(GraphicsEngine*& graphicsEngine, const char* memory, unsigned int size)
{
    
    ResourceUploadBatch resouce_upload_batch(graphicsEngine->GetDevice());
    resouce_upload_batch.Begin();
    ComPtr<ID3D12Resource> texture;
    
    //DDSテクスチャメモリ 生成
    HRESULT hr = CreateDDSTextureFromMemoryEx(
        graphicsEngine->GetDevice(),
        resouce_upload_batch,
        (const uint8_t*)memory,
        size,
        0,
        D3D12_RESOURCE_FLAG_NONE,
        0,
        &texture
    );

    //ResourceUploadBatchを終了する
    resouce_upload_batch.End(graphicsEngine->GetCommandQueue());

    //生成確認
    if (FAILED(hr))
    {
        //失敗したので異常終了する
        MessageBoxA(nullptr, "CreateDDSTextureFromMemoryExに失敗しました", "エラー", MB_OK);
        std::abort();
    }

    this->texture_ = texture.Get();
    this->texture_Desc_ = this->texture_->GetDesc();
}
