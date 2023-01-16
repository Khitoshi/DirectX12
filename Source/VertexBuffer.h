#pragma once

#include <d3dx12.h>
#include <wrl.h>
#include "GraphicsEngine.h"
using namespace Microsoft::WRL;

class VertexBuffer
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	VertexBuffer();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~VertexBuffer();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="graphicsEngine">使用しているデバイスを格納しているクラス</param>
	/// <param name="size">頂点サイズ</param>
	/// <param name="stride">頂点バッファのストライド、1個の頂点のサイズ</param>
	/// <param name="graphicsEngine">デバイス取得用</param>
	void Init(GraphicsEngine* graphicsEngine,int size, int stride);

	/// <summary>
	/// 頂点データを頂点バッファにコピー
	/// </summary>
	/// <param name="strVertices">コピー元の頂点データ(void*に変換)</param>
	void Copy(void* strVertices);

public:
	/// <summary>
	/// 頂点バッファービューを取得
	/// </summary>
	/// <returns>this vertex buffer view</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView()const { return this->vertex_Buffer_View_; }

	/// <summary>
	/// 頂点バッファのストライド(単位:byte)を取得
	/// </summary>
	/// <returns>this vertex buffer view strideInBytes</returns>
	UINT GetStrideInByte()const { return this->vertex_Buffer_View_.StrideInBytes; }

	/// <summary>
	/// 頂点バッファのサイズ(単位:byte)
	/// </summary>
	/// <returns>this vertex buffer view size(byte)</returns>
	UINT GetSizeInBytes()const { return this->vertex_Buffer_View_.SizeInBytes; }

	/// <summary>
	/// ID3D12Resouceのアドレスを取得
	/// </summary>
	/// <returns>this vertex buffer</returns>
	ID3D12Resource* GetResourceAddress()const { return this->vertex_Buffer_.Get(); }

private:
	//頂点バッファ
	ComPtr<ID3D12Resource> vertex_Buffer_;

	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_Buffer_View_;
};
