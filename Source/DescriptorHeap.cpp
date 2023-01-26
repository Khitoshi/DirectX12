#include "DescriptorHeap.h"
#include "GraphicsEngine.h"
#include "ConstantBuffer.h"
#include "IShaderResource.h"
#include "IUnorderAccessResrouce.h"

//デフォルト コンストラクタ
DescriptorHeap::DescriptorHeap():
    descriptor_Heap_(),
    shader_Resources_(MAX_SHADER_RESOURCE),
    uav_Resources_(MAX_SHADER_RESOURCE),
    constant_Buffers_(MAX_CONSTANT_BUFFER),
    sampler_Descs_(),
    const_Buffer_Gpu_Descriptor_Start_(),
    shader_Resource_Gpu_Descriptor_Start_(),
    uav_Gpu_Descriptor_Start_(),
    sampler_Gpu_Descriptor_Start_(),

    num_Shader_Resource_(0),
    num_Constant_Buffer_(0),
    num_Uav_Resource_(0),
    num_Sampler_Desc_(0),
    num_Descriptor_Heap_(0)
{
}

//デフォルト デストラクタ
DescriptorHeap::~DescriptorHeap()
{
}

//シェーダーリソースをディスクリプタヒープに登録
void DescriptorHeap::RegistShaderResource(int registerNo, IShaderResource& shaderResource)
{
    RegistResource(
        registerNo,
        &shaderResource,
        &this->shader_Resources_.front(),
        this->num_Shader_Resource_,
        MAX_SHADER_RESOURCE,
        L"DescriptorHeap::RegistShaderResource() レジスタ番号が範囲外です。"
    );
}

//アンオーダーアクセスリソースを登録
void DescriptorHeap::RegistUnorderAccessResource(int registerIndex, IUnorderAccessResrouce& unorderAccessResrouce)
{
    RegistResource(
        registerIndex,
        &unorderAccessResrouce,
        &this->uav_Resources_.front(),
        this->num_Uav_Resource_,
        MAX_SHADER_RESOURCE,
        L"DescriptorHeap::RegistUnorderAccessResource() レジスタ番号が範囲外です。"
    );
}

//定数バッファをディスクリプタヒープに登録
void DescriptorHeap::RegistConstantBuffer(int registerIndex, ConstantBuffer& constantBuffer)
{
    RegistResource(
        registerIndex,
        &constantBuffer,
        &this->constant_Buffers_.front(),
        this->num_Constant_Buffer_,
        MAX_CONSTANT_BUFFER,
        L"DescriptorHeap::RegistConstantBuffer() レジスタ番号が範囲外です。"
    );
}

//サンプラ定義をディスクリプタヒープに追加
void DescriptorHeap::RegistSamplerDesc(int registerIndex, const D3D12_SAMPLER_DESC& samplerDesc)
{
    RegistResource(
        registerIndex,
        samplerDesc,
        this->sampler_Descs_,
        this->num_Sampler_Desc_,
        MAX_SAMPLER_STATE,
        L"DescriptorHeap::RegistSamplerDesc() レジスタ番号が範囲外です。"
    );
}

//ディスクリプタヒープへの登録を確定
void DescriptorHeap::Commit(GraphicsEngine* graphicsEngine)
{
    //デバイスを取得
    const auto& device = graphicsEngine->GetD3DDevice();

    //シェーダーリソースビューのディスクリプタヒープ デスク
    D3D12_DESCRIPTOR_HEAP_DESC srv_Heap_Desc = {};
    //シェーダーリソース数 + 定数バッファ数 + アンオーダーアクセスリソース数
    //をディスクリプタ数に設定
    srv_Heap_Desc.NumDescriptors = num_Shader_Resource_ + num_Constant_Buffer_+ num_Uav_Resource_;
    srv_Heap_Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srv_Heap_Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    //ディスクリプタ 生成 loop
    for (auto& descriptor_Heap : this->descriptor_Heap_) 
    {
        //ディスクリプタヒープ 生成
        HRESULT hr = device.CreateDescriptorHeap(&srv_Heap_Desc, IID_PPV_ARGS(&descriptor_Heap));
        //ディスクリプタ数 インクリメント
        this->num_Descriptor_Heap_++;

        //生成チェック
        if (FAILED(hr))
        {
            //生成失敗
            MessageBox(nullptr, L"DescriptorHeap::Commit ディスクリプタヒープの作成に失敗しました。", L"エラー", MB_OK);
            std::abort();
            //強制終了
            std::abort();
        }
    }

    //定数バッファやシェーダーリソースのディスクリプタをヒープに書き込む
    int buffer_Index = 0;
    for (auto& descriptor_heap : this->descriptor_Heap_)
    {
        //CPU ハンドル取得
        auto cpu_Handle = descriptor_heap->GetCPUDescriptorHandleForHeapStart();
        //GPU ハンドル所得
        auto gpu_Handle = descriptor_heap->GetGPUDescriptorHandleForHeapStart();


        //定数バッファを登録
        for (auto& constant_buffer:this->constant_Buffers_)
        {
            if (constant_buffer != nullptr)
            {
                constant_buffer->RegistConstantBufferView(graphicsEngine, cpu_Handle, buffer_Index);
            }
            //次に進める。
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //続いてシェーダーリソース。
        for (int i = 0; i < this->num_Shader_Resource_; i++) 
        {
            if (this->shader_Resources_[i] != nullptr) 
            {
                this->shader_Resources_[i]->RegistShaderResourceView(*graphicsEngine,cpu_Handle, buffer_Index);
            }
            //次に進める。
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //続いてUAV。
        for (int i = 0; i < this->num_Uav_Resource_; i++) {
            if (this->uav_Resources_[i] != nullptr) {
                this->uav_Resources_[i]->RegistUnorderAccessView(cpu_Handle, buffer_Index);
            }
            //次に進める。
            cpu_Handle.ptr += graphicsEngine->GetCbrSrvDescriptorSize();
        }

        //定数バッファのディスクリプタヒープの開始ハンドルを計算。
        this->const_Buffer_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        //シェーダーリソースのディスクリプタヒープの開始ハンドルを計算。
        this->shader_Resource_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        this->uav_Gpu_Descriptor_Start_[buffer_Index].ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * this->num_Constant_Buffer_;
        //UAVリソースのディスクリプタヒープの開始ハンドルを計算。
        this->uav_Gpu_Descriptor_Start_[buffer_Index] = gpu_Handle;
        this->uav_Gpu_Descriptor_Start_[buffer_Index].ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * (this->num_Shader_Resource_ + this->num_Constant_Buffer_);

        //ポインタを進める
        gpu_Handle.ptr += (UINT64)graphicsEngine->GetCbrSrvDescriptorSize() * (this->num_Shader_Resource_ + this->num_Constant_Buffer_ + this->num_Uav_Resource_);

        //バッファ数をインクリメント
        buffer_Index++;
    }
}

//サンプラステート用のディスクリプタヒープへの登録
void DescriptorHeap::CommitSamplerHeap(GraphicsEngine* graphicsEngine)
{
    //デバイス
    const auto& device = graphicsEngine->GetD3DDevice();

    //シェーダーリソースビュー　ヒープデスク
    D3D12_DESCRIPTOR_HEAP_DESC srv_Heap_Desc = {};
    
    //シェーダーリソースビュー　ヒープデスク設定
    srv_Heap_Desc.NumDescriptors = this->num_Sampler_Desc_;
    srv_Heap_Desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
    srv_Heap_Desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    //ディスクリプタヒープ　生成 loop
    for (auto& descriptor_Heap : this->descriptor_Heap_) {
        //生成
        HRESULT hr = device.CreateDescriptorHeap(&srv_Heap_Desc, IID_PPV_ARGS(&descriptor_Heap));

        //生成 チェック
        if (FAILED(hr)) {
            //失敗
            MessageBox(nullptr, L"DescriptorHeap::Commit ディスクリプタヒープの作成に失敗しました。", L"エラー", MB_OK);
            //異常終了
            std::abort();
        }
    }

    int buffer_Index = 0;
    //サンプラステートをディスクリプタヒープに登録していくloop
    for (auto& descriptorHeap : this->descriptor_Heap_) {
        //CPUのハンドルを取得
        auto cpuHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
        //GPUのハンドルを取得
        auto gpuHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

        for (int i = 0; i < this->num_Sampler_Desc_; i++) {
            //サンプラステートをディスクリプタヒープに登録
            device.CreateSampler(&this->sampler_Descs_[i], cpuHandle);
            cpuHandle.ptr += graphicsEngine->GetSapmerDescriptorSize();
        }
        this->sampler_Gpu_Descriptor_Start_[buffer_Index] = gpuHandle;
        //バッファの数をインクリメント
        buffer_Index++;
    }
}

//リソースをディスクリプタヒープに登録
template<class T>
void DescriptorHeap::RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage)
{
    if (registerIndex == -1)
    {
        //-1が指定されているので，
        //現在登録されているリソースの一番後ろに登録
        registerIndex = numRes;
    }

    //登録番号がリソース最大値以下の場合
    if (registerIndex < MAX_RESOURCE)
    {
        //登録
        resTable[registerIndex] = res;

        if(numRes < registerIndex + 1)
        {
            //登録されているリソース数を増やす
            numRes = registerIndex + 1;
        }
    }
    else
    {
        //リソースの最大値より多い場合
        //エラーメッセージを出す
        MessageBox(nullptr, errorMessage, L"DescriptorHeap::RegistResourceで失敗", MB_OK);
        std::abort();
        //強制終了
        std::abort();
    }
}
