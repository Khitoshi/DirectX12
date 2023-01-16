#pragma once
#include<d3dx12.h>
#include<wrl.h>

#include "GraphicsEngine.h"

using namespace Microsoft::WRL;

/// <summary>
/// インデックス バッファ
/// </summary>
class IndexBuffer
{
public:
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    IndexBuffer();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~IndexBuffer();

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="graphicsEngine">使用しているデバイスを格納しているクラス</param>
    /// <param name="size">インデックスバッファのサイズ</param>
    /// <param name="stride">ストライド</param>
    void Init(GraphicsEngine* graphicsEngine,int size, int stride);

    /// <summary>
    /// インデックスデータをインデックスバッファにコピー
    /// </summary>
    /// <param name="srcIndecies">コピー元のインデックスデータ</param>
    void Copy(uint16_t* srcIndecies);


    /// <summary>
    /// インデックスデータをインデックスバッファにコピー
    /// </summary>
    /// <param name="srcIndecies">コピー元のインデックスデータ</param>
    void Copy(uint32_t* srcIndecies);


public:
    /// <summary>
    /// インデックスバッファビューを取得
    /// </summary>
    /// <returns>this index buffer view</returns>
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView()const { return this->index_Buffer_View_; }

    /// <summary>
    /// インデックスの数を取得
    /// </summary>
    /// <returns></returns>
    const int GetIndexCount()const { return this->index_Count_; }

    /// <summary>
    /// インデックスバッファのストライドを取得
    /// </summary>
    /// <returns>this indexbuffer stride in bytes size</returns>
    const UINT GetStrideInBytes() const { return static_cast<UINT>(this->stride_In_Bytes_); }

    /// <summary>
    /// インデックスバッファのサイズ(単位:byte)を取得
    /// </summary>
    /// <returns>this indexbuffer size in bytes</returns>
    const UINT GetSizeInBytes()const { return static_cast<UINT>(this->size_In_Bytes_); }

    /// <summary>
    /// ID3D12Resouceのアドレスを所t句
    /// </summary>
    /// <returns>return this Resouce</returns>
    ID3D12Resource* GetResouceAddress()const { return this->index_Buffer_.Get(); }

private:
    //インデックスバッファ
    ComPtr<ID3D12Resource> index_Buffer_;
    //インデックスバッファビュー
    D3D12_INDEX_BUFFER_VIEW index_Buffer_View_;

    //インデックスの数
    int index_Count_;
    //ストライド
    int stride_In_Bytes_;
    //サイズ
    int size_In_Bytes_;
};