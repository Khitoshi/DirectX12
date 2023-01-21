#pragma once
#include "d3dx12.h"

class GraphicsEngine;
class IShaderResource {
public:
	virtual ~IShaderResource()
	{
	}
	/// <summary>
	/// SRVに登録。
	/// </summary>
	/// <param name="descriptorHandle">CPU ディスクリプタ　ハンドル</param>
	/// <param name="bufferNumber">バッファの番号</param>
	/// <param name="graphicsEngine">デバイス取得用</param>
	virtual void RegistShaderResourceView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber) = 0;
};