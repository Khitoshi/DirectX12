#pragma once

#include <d3dx12.h>

#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class ConstantBuffer
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	ConstantBuffer();

	/// <summary>
	/// ���[�u�R���X�g���N�^
	/// *�R�s�[�ł͂Ȃ����n���Ă���*
	/// </summary>
	/// <param name="constantBuffer"></param>
	ConstantBuffer(ConstantBuffer&& cb);

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~ConstantBuffer();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="size">�萔�o�b�t�@�̃T�C�Y</param>
	/// <param name="srcData">�\�[�X�f�[�^�Cnull���w�肷�邱�Ƃ��\</param>
	/// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
	void Init(GraphicsEngine*& graphicsEngine, int constantBufferSize, void* srcData);

	/// <summary>
	/// �f�[�^��VRAM�ɃR�s�[
	/// </summary>
	/// <param name="data">�R�s�[���̃f�[�^</param>
	void CopyToVRAM(GraphicsEngine*& graphicsEngine, void* data);
	template<class T>
	void CopyToVRAM(GraphicsEngine*& graphicsEngine, T& data)
	{
		CopyToVRAM(graphicsEngine, &data);
	}
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�r���[��o�^
	/// </summary>
	/// <param name="descriptorHandle">�f�B�X�N���v�^�n���h��</param>
	/// <param name="graphicsEngine">�f�o�C�X�擾�p</param>
	void RegistConstantBufferView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle);
	void RegistConstantBufferView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNo);

public:
	/// <summary>
	/// �萔�o�b�t�@��GPU���z�A�h���X���擾
	/// </summary>
	/// <param name="graphicsEngine"></param>
	/// <returns></returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(GraphicsEngine*& graphicsEngine);

public:
#pragma region Get Method
	/// <summary>
	/// ���p�\�ȃo�b�t�@���ǂ����𔻒�B
	/// </summary>
	/// <returns></returns>
	bool IsValid() const
	{
		return this->is_Valid_;
	}
#pragma endregion


private:
	//�萔�o�b�t�@�B
	ComPtr<ID3D12Resource> constant_Buffer_[2];
	//ID3D12Resource* constant_Buffer_[2];

	//CPU������A�N�Z�X�ł��邷��萔�o�b�t�@�̃A�h���X�B
	void* const_Buffer_CPU_[2];

	//�萔�o�b�t�@�̃T�C�Y�B
	int constant_Buffer_Size_;
	//aoolc�̃T�C�Y
	int alloc_Size_;

	//���p�\�t���O
	bool is_Valid_;
};