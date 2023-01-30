#pragma once

#include "IShaderResource.h"
#include <wrl.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class StructuredBuffer :public IShaderResource
{
public:
    /// <summary>
    /// デフォルト コンストラクタ 
    /// </summary>
    StructuredBuffer();

    /// <summary>
    /// デフォルト デストラクタ 
    /// </summary>
    ~StructuredBuffer()override;

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <param name="elementSize">エレメントのサイズ</param>
    /// <param name="elementNumber">エレメントの数</param>
    /// <param name="initData">初期化データ</param>
    void Init(GraphicsEngine*& graphicsEngine, int elementSize, int elementNumber, void* initData);

    /// <summary>
    /// SRVに登録。
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <param name="descriptorHandle">CPU ディスクリプタ　ハンドル</param>
    /// <param name="buffernumber">バッファ 番号</param>
    void RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber)override;

    /// <summary>
    /// 構造化バッファの内容更新
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <param name="data"></param>
    void Update(GraphicsEngine*& graphicsEngine,void* data);

    
private:

public:
#pragma region Get method

    /// <summary>
    /// 初期化判別用フラグ取得
    /// </summary>
    /// <returns>true = 初期化済み</returns>
    bool IsInited() const { return this->is_Inited_; }

    //GPUからアクセスできるバッファーの取得
    ID3D12Resource* GetResouce(GraphicsEngine*& graphicsEngine);

#pragma endregion


private:
    //GPUからアクセスできるバッファー
    ComPtr<ID3D12Resource> buffers_On_GPU_[2];
    //CPUからアクセスできるバッファー
    void* buffers_On_CPU_[2];

    //エレメント 要素数
    int element_Number_;
    //エレメントサイズ
    int element_Size_;

    //初期化済み 判断 フラグ
    bool is_Inited_;

};

