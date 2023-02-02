#pragma once

#include <d3dx12.h>
#include<wrl.h>

using namespace Microsoft::WRL;

class IShaderResource;
class IUnorderAccessResrouce;
class ConstantBuffer;
class GraphicsEngine;

class DescriptorHeap
{
public:
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    DescriptorHeap();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~DescriptorHeap();

    /// <summary>
    /// シェーダーリソースをディスクリプタヒープに登録
    /// </summary>
    /// <param name="registerNo">レジスタ番号 -1が指令されたら，現在登録されているリソースの次にレジスタを登録する</param>
    /// <param name="shaderResource">シェーダーリソース</param>
    void RegistShaderResource(int registerNo, IShaderResource& shaderResource);

    /// <summary>
    /// アンオーダーアクセスリソースを登録
    /// </summary>
    /// <param name="registerNo">レジスタ番号，-1が登録されたら現在登録されているリソースの一番後ろに登録</param>
    /// <param name="sr"></param>
    void RegistUnorderAccessResource(int registerIndex, IUnorderAccessResrouce& unorderAccessResrouce);
    
    /// <summary>
    /// 定数バッファをディスクリプタヒープに登録
    /// </summary>
    /// <param name="registerNo">レジスタ番号 -1が指令されたら，現在登録されているリソースの次にレジスタを登録する</param>
    /// <param name="cb">定数バッファ</param>
    void RegistConstantBuffer(int registerIndex, ConstantBuffer& constantBuffer);

    /// <summary>
    /// サンプラ定義をディスクリプタヒープに追加
    /// </summary>
    /// <param name="registerNo">レジスタ番号 -1が指令されたら，現在登録されているリソースの次にレジスタを登録する</param>
    /// <param name="desc">サンプラ定義</param>
    void RegistSamplerDesc(int registerIndex, const D3D12_SAMPLER_DESC& samplerDesc);

    /// <summary>
    /// ディスクリプタヒープへの登録を確定
    /// </summary>
    /// <param name="graphicsEngine">デバイスが格納されている</param>
    void Commit(GraphicsEngine*& graphicsEngine);

    /// <summary>
    /// サンプラステート用のディスクリプタヒープへの登録
    /// </summary>
    /// <param name="graphicsEngine">デバイスが格納されている</param>
    void CommitSamplerHeap(GraphicsEngine*& graphicsEngine);

private:
    /// <summary>
    /// リソースをディスクリプタヒープに登録
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="registerIndex">登録番号</param>
    /// <param name="res">登録するリソース</param>
    /// <param name="resTable">リソーステーブル。このテーブルにリソースが追加されます。</param>
    /// <param name="numRes">登録されているリソースの数。本関数を呼び出すと、この数が１インクリメントされます。</param>
    /// <param name="MAX_RESOURCE">リソースの最大値</param>
    /// <param name="errorMessage">エラーが発生した際に表示するTEXT</param>
    template<class T>
    void RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage);

public://get method

    /// <summary>
    /// ディスクリプタヒープ 取得
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <returns>this Descriptor Heap</returns>
    ID3D12DescriptorHeap* GetDescriptorHeap(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// 定数バッファの開始ハンドル 取得
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <returns>this constant Buffer GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// シェーダーリソースのディスクリプタの開始ハンドルを取得
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <returns>this shader resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// Unorder Access リソースのディスクリプタの開始ハンドルを取得
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <returns>this uav resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetUavResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const;

    /// <summary>
    /// Samplerのディスクリプタの開始ハンドルを取得
    /// </summary>
    /// <param name="graphicsEngine">デバイスを格納している</param>
    /// <returns>this sampler resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const;

    /// <summary>
    /// シェーダーリソースが一つでも登録されているか判定
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistShaderResource() const { return this->num_Shader_Resource_ != 0; }

    /// <summary>
    /// 定数バッファが一つでも登録されているか判定
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistConstantResource() const { return this->num_Constant_Buffer_ != 0; }

    /// <summary>
    /// UAVリソースが一つでも登録されているか判定
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistUavResource() const { return this->num_Uav_Resource_ != 0; }

    /// <summary>
    /// 定数バッファが一つでも登録されているか判定。
    /// </summary>
    /// <returns>tru = not zero</returns>
    bool IsRegistConstantBuffer() const { return this->num_Constant_Buffer_ != 0; }

    /// <summary>
    /// UAVディスクリプタが始まる配列番号を取得する。
    /// </summary>
    /// <remarks>
    /// UAVディスクリプタはSRVディスクリプタの次から登録されている。
    /// なので、SRVディスクリプタが10個登録されていれば、
    /// UAVディスクリプタは配列の10番目から登録されていることになる。
    /// この関数は現在レイトレエンジンで使用されている。
    /// </remarks>
    /// <returns></returns>
    int GetOffsetUAVDescriptorFromTableStart() const { return this->num_Shader_Resource_ + this->num_Constant_Buffer_; }

    /// <summary>
    /// SRVディスクリプタが始まる配列番号を取得する。
    /// </summary>
    /// <returns></returns>
    int GetOffsetSRVDescriptorFromTableStart() const { return this->num_Constant_Buffer_; }

    /// <summary>
    /// 定数バッファディスクリプタが始まる配列番号を取得する。
    /// </summary>
    /// /// <remarks>
    /// 定数バッファディスクリプタはSRVディスクリプタとUAVディスクリプタの次から登録されている。
    /// なので、SRVディスクリプタが10個、UVAディスクリプタが5個登録されていれば、
    /// 定数バッファディスクリプタは配列の15番目から登録されていることになる。
    /// この関数は現在レイトレエンジンで使用されている。
    /// <returns></returns>
    int GetOffsetConstantBufferDescriptorFromTableStart() const{return this->num_Shader_Resource_ + this->num_Uav_Resource_;}

private:
    enum {
        MAX_SHADER_RESOURCE = 1024 * 10,	//シェーダーリソースの最大数。
        MAX_CONSTANT_BUFFER = 1024 * 10,	//定数バッファの最大数。
        MAX_SAMPLER_STATE = 16,	            //サンプラステートの最大数。
    };

private:
    //ディスクリプタヒープ
    //ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[2];
    ID3D12DescriptorHeap* descriptor_Heap_[2];
    //シェーダーリソース。
    std::vector<IShaderResource*> shader_Resources_;
    //UAVリソース。
    std::vector <IUnorderAccessResrouce*> uav_Resources_;
    //定数バッファ。
    std::vector <ConstantBuffer*> constant_Buffers_;
    //サンプラステート。
    D3D12_SAMPLER_DESC sampler_Descs_[MAX_SAMPLER_STATE];
    //定数バッファのディスクリプタヒープの開始ハンドル。
    D3D12_GPU_DESCRIPTOR_HANDLE const_Buffer_Gpu_Descriptor_Start_[2];
    //シェーダーリソースのディスクリプタヒープの開始ハンドル。
    D3D12_GPU_DESCRIPTOR_HANDLE shader_Resource_Gpu_Descriptor_Start_[2];
    //UAVリソースのディスクリプタヒープの開始ハンドル。
    D3D12_GPU_DESCRIPTOR_HANDLE uav_Gpu_Descriptor_Start_[2];
    //サンプラーステートのディスクリプタの開始ハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE sampler_Gpu_Descriptor_Start_[2];

    //シェーダーリソースの数
    int num_Shader_Resource_;
    //定数バッファの数
    int num_Constant_Buffer_;
    //アンオーダーアクセスリソースの数
    int num_Uav_Resource_;
    //サンプラの数
    int num_Sampler_Desc_;
    int num_Descriptor_Heap_;
};

