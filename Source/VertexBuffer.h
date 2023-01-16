#pragma once

#include <d3dx12.h>
#include <wrl.h>
#include "GraphicsEngine.h"
using namespace Microsoft::WRL;

class VertexBuffer
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	VertexBuffer();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~VertexBuffer();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="graphicsEngine">�g�p���Ă���f�o�C�X���i�[���Ă���N���X</param>
	/// <param name="size">���_�T�C�Y</param>
	/// <param name="stride">���_�o�b�t�@�̃X�g���C�h�A1�̒��_�̃T�C�Y</param>
	/// <param name="graphicsEngine">�f�o�C�X�擾�p</param>
	void Init(GraphicsEngine* graphicsEngine,int size, int stride);

	/// <summary>
	/// ���_�f�[�^�𒸓_�o�b�t�@�ɃR�s�[
	/// </summary>
	/// <param name="strVertices">�R�s�[���̒��_�f�[�^(void*�ɕϊ�)</param>
	void Copy(void* strVertices);

public:
	/// <summary>
	/// ���_�o�b�t�@�[�r���[���擾
	/// </summary>
	/// <returns>this vertex buffer view</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView()const { return this->vertex_Buffer_View_; }

	/// <summary>
	/// ���_�o�b�t�@�̃X�g���C�h(�P��:byte)���擾
	/// </summary>
	/// <returns>this vertex buffer view strideInBytes</returns>
	UINT GetStrideInByte()const { return this->vertex_Buffer_View_.StrideInBytes; }

	/// <summary>
	/// ���_�o�b�t�@�̃T�C�Y(�P��:byte)
	/// </summary>
	/// <returns>this vertex buffer view size(byte)</returns>
	UINT GetSizeInBytes()const { return this->vertex_Buffer_View_.SizeInBytes; }

	/// <summary>
	/// ID3D12Resouce�̃A�h���X���擾
	/// </summary>
	/// <returns>this vertex buffer</returns>
	ID3D12Resource* GetResourceAddress()const { return this->vertex_Buffer_.Get(); }

private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertex_Buffer_;

	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vertex_Buffer_View_;
};
