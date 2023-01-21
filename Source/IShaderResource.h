#pragma once
#include "d3dx12.h"

class GraphicsEngine;
class IShaderResource {
public:
	virtual ~IShaderResource()
	{
	}
	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descriptorHandle">CPU �f�B�X�N���v�^�@�n���h��</param>
	/// <param name="bufferNumber">�o�b�t�@�̔ԍ�</param>
	/// <param name="graphicsEngine">�f�o�C�X�擾�p</param>
	virtual void RegistShaderResourceView(GraphicsEngine* graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber) = 0;
};