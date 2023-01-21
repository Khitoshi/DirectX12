#pragma once

#include "IShaderResource.h"
#include <wrl.h>

using namespace Microsoft::WRL;

class Texture :public IShaderResource
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Texture();

    /// <summary>
    /// 明示的 コンストラクタ
    /// </summary>
    /// <param name = "filePath">ロードするテクスチャのファイルパス</param>
    explicit Texture(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Texture();

    /// <summary>
    /// DDSファイルからテクスチャを初期化する。
    /// </summary>
    /// <param name="filePath">ロードするテクスチャのファイルパス。</param>
    void InitFromDDSFile(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// メモリからテクスチャを初期化する。
    /// </summary>
    /// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス</param>
    /// <param name="size">テクスチャのサイズ。</param>
    void InitFromMemory(GraphicsEngine* graphicsEngine,const char* memory, unsigned int size);
    
    /// <summary>
    /// D3Dリソースからテクスチャを初期化する。
    /// </summary>
    /// <param name="resrouce">D3Dリソース。</param>
    void InitFromD3DResource(ID3D12Resource* texture);
    
    /// <summary>
    /// SRVに登録。
    /// </summary>
    /// <param name="descriptorHandle"></param>
    void RegistShaderResourceView(GraphicsEngine* graphicsEngine,D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

private:

    /// <summary>
    /// DDSファイルからテクスチャをロード。
    /// </summary>
    /// <param name="filePath">ファイルパス。</param>
    void LoadTextureFromDDSFile(GraphicsEngine* graphicsEngine,const wchar_t* filePath);

    /// <summary>
    /// メモリからテクスチャをロード。
    /// </summary>
    /// <param name="memory">テクスチャデータが格納されているメモリの先頭アドレス。</param>
    /// <param name="size">テクスチャのサイズ。</param>
    /// <param name="ge12">Dx12版のグラフィックスエンジン</param>
    /// <param name="device">D3Dデバイス</param>
    void LoadTextureFromMemory(GraphicsEngine* graphicsEngine,const char* memory, unsigned int size);


public:
#pragma region Get method
    
    /// <summary>
    /// テクスチャが有効か判定。
    /// </summary>
    /// <returns>trueが返ってきたら有効。</returns>
    bool IsValid() const
    {
        return this->texture_ != nullptr;
    }

    /// <summary>
    /// テクスチャ取得
    /// </summary>
    /// <returns></returns>
    ID3D12Resource* Get()
    {
        return this->texture_.Get();
    }

    /// <summary>
    /// テクスチャの幅を取得。
    /// </summary>
    /// <returns></returns>
    int GetWeight()const
    {
        return static_cast<int>(this->texture_Desc_.Width); 
    }
    
    /// <summary>
    /// テクスチャの高さを取得。
    /// </summary>
    /// <returns></returns>
    int GetHeight() const
    {
        return static_cast<int>(this->texture_Desc_.Height);
    }

    /// <summary>
    /// テクスチャのフォーマットを取得。
    /// </summary>
    /// <returns></returns>
    DXGI_FORMAT GetFormat() const
    {
        return this->texture_Desc_.Format;
    }

#pragma endregion

private:
    //テクスチャ。
    ComPtr<ID3D12Resource> texture_ = nullptr;	
    //テクスチャ情報
    D3D12_RESOURCE_DESC texture_Desc_;	
};

