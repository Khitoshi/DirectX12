#pragma once

#include <d3dx12.h>

#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class ConstantBuffer
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	ConstantBuffer();

	/// <summary>
	/// ムーブコンストラクタ
	/// *コピーではなく譲渡している*
	/// </summary>
	/// <param name="constantBuffer"></param>
	ConstantBuffer(ConstantBuffer&& cb);

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~ConstantBuffer();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="size">定数バッファのサイズ</param>
	/// <param name="srcData">ソースデータ，nullを指定することも可能</param>
	/// <param name="graphicsEngine">デバイスを格納している</param>
	void Init(GraphicsEngine*& graphicsEngine, int constantBufferSize, void* srcData);

	/// <summary>
	/// データをVRAMにコピー
	/// </summary>
	/// <param name="data">コピー元のデータ</param>
	void CopyToVRAM(GraphicsEngine*& graphicsEngine, void* data);
	template<class T>
	void CopyToVRAM(GraphicsEngine*& graphicsEngine, T& data)
	{
		CopyToVRAM(graphicsEngine, &data);
	}
	
	/// <summary>
	/// ディスクリプタヒープに定数バッファビューを登録
	/// </summary>
	/// <param name="descriptorHandle">ディスクリプタハンドル</param>
	/// <param name="graphicsEngine">デバイス取得用</param>
	void RegistConstantBufferView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
	void RegistConstantBufferView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

public:
	/// <summary>
	/// 定数バッファのGPU仮想アドレスを取得
	/// </summary>
	/// <param name="graphicsEngine"></param>
	/// <returns></returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(GraphicsEngine*& graphicsEngine);

public:
#pragma region Get Method
	/// <summary>
	/// 利用可能なバッファかどうかを判定。
	/// </summary>
	/// <returns></returns>
	bool IsValid() const
	{
		return this->is_Valid_;
	}
#pragma endregion


private:
	//定数バッファ。
	ComPtr<ID3D12Resource> constant_Buffer_[2];
	//ID3D12Resource* constant_Buffer_[2];

	//CPU側からアクセスできるする定数バッファのアドレス。
	void* const_Buffer_CPU_[2];

	//定数バッファのサイズ。
	int constant_Buffer_Size_;
	//aoolcのサイズ
	int alloc_Size_;

	//利用可能フラグ
	bool is_Valid_;
};