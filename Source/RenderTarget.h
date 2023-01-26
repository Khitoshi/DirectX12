#pragma once
#include "Texture.h"

class GraphicsEngine;

class RenderTarget
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	RenderTarget();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̍쐬
	/// </summary>
	/// <param name="graphicsEngine">�f�o�C�X�֌W��ۗL���Ă���</param>
	/// <param name="renderTargetWidth">�����_�����O�^�[�Q�b�g�̕�</param>
	/// <param name="renderTargetHeight">�����_�����O�^�[�Q�b�g�̍���</param>
	/// <param name="mipLevel">�~�b�v�}�b�v���x���B0���w�肵���ꍇ�̓~�b�v�}�b�v���T�|�[�g����Ă���GPU�ł�1*1�s�N�Z���܂ł̃~�b�v�}�b�v���쐬�����B</param>
	/// <param name="arraySize">�e�N�X�`���z��̃T�C�Y</param>
	/// <param name="colorFormat">�J���[�o�b�t�@�̃t�H�[�}�b�g�B</param>
	/// <param name="depthStencilFormat">�[�x�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B</param>
	/// <returns>true���Ԃ��Ă�����쐬����</returns>
	bool Create(
		GraphicsEngine* graphicsEngine,
		int renderTargetWidth,
		int renderTargetHeight,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT colorFormat,
		DXGI_FORMAT depthStencilFormat,
		float clearColor[4] = nullptr
	);

private:
#pragma region Create Method
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v���쐬
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�b�N�G���W��</param>
	/// <returns></returns>
	void CreateDescriptorHeap(GraphicsEngine& graphicsEngine);

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�b�N�G���W��</param>
	/// <param name="textureWidth">�e�N�X�`���̕�</param>
	/// <param name="textureHeight">�e�N�X�`���̍���</param>
	/// <param name="mipLevel">�~�b�v�}�b�v���x��</param>
	/// <param name="arraySize">�e�N�X�`���z��̃T�C�Y</param>
	/// <param name="format">�e�N�X�`���̃t�H�[�}�b�g</param>
	/// <returns></returns>
	void CreateRenderTargetTexture(
		GraphicsEngine& graphicsEngine,
		int textureWidth,
		int textureHeight,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT format,
		float clearColor[4]
	);

	/// <summary>
	/// �[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`�����쐬
	/// </summary>
	/// <param name="graphicsEngine">�O���t�B�b�N�G���W��</param>
	/// <param name="textureWidth">�e�N�X�`���̕�</param>
	/// <param name="TextureHeight">�e�N�X�`���̍���</param>
	/// <param name="format">�e�N�X�`���̃t�H�[�}�b�g</param>
	/// <returns></returns>
	void CreateDepthStencilTexture(
		GraphicsEngine& graphicsEngine,
		int textureWidth,
		int textureHeight,
		DXGI_FORMAT format);

	/// <summary>
	/// �f�B�X�N���v�^�̍쐬
	/// </summary>
	/// <param name="d3dDevice">D3D�f�o�C�X</param>
	/// <returns></returns>
	void CreateDescriptor(ID3D12Device5*& d3dDevice);
#pragma endregion

public:
#pragma region Get Method
	/// <summary>
	/// CPU���̃����_�����O�^�[�Q�b�g�̃f�B�X�N���v�^�n���h�� �擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
	{
		return this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// CPU���̃f�v�X�X�e���V���o�b�t�@�̃f�B�X�N���v�^�n���h�� �擾�B
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
	{
		return this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�� �擾�B
	/// </summary>
	/// <returns></returns>
	Texture& GetRenderTargetTexture()
	{
		return this->render_Target_Texture_;
	}

	/// <summary>
	/// �f�v�X�X�e���V���o�b�t�@�����݂��Ă��邩����
	/// </summary>
	/// <returns></returns>
	bool IsExsitDepthStencilBuffer() const
	{
		return this->depth_Stencil_Texture_;
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̕� �擾�B
	/// </summary>
	/// <returns></returns>
	int GetWidth() const
	{
		return this->width_;
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̍��� �擾�B
	/// </summary>
	/// <returns></returns>
	int GetHeight() const
	{
		return this->height_;
	}

	/// <summary>
	/// �J���[�o�b�t�@�̃t�H�[�}�b�g �擾
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetColorBufferFormat() const
	{
		return this->render_Target_Texture_.GetFormat();
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�r���[�̃N���A�J���[ �擾
	/// </summary>
	/// <returns></returns>
	const float* GetRTVClearColor() const
	{
		return this->rtv_Clear_Color_;
	}

	/// <summary>
	/// DSV�̃N���A�J���[ �擾
	/// </summary>
	/// <returns></returns>
	float GetDSVClearValue() const
	{
		return this->dsv_Clear_Value_;
	}
#pragma endregion

private:
	//�e�N�X�`��
	Texture render_Target_Texture_;
	//�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`��
	ID3D12Resource* render_Target_Texture_Dx12_;
	//�[�x�X�e���V���o�b�t�@�ƂȂ�e�N�X�`��
	ID3D12Resource* depth_Stencil_Texture_;
	//RTV�p�̃f�B�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* rtv_Heap_;
	//�[�x�X�e���V���o�b�t�@�r���[�̃f�B�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* dsv_Heap_;

	//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y
	UINT rtv_Descriptor_Size_;
	//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y
	UINT dsv_Descriptor_Size_;

	//�����_�����O�^�[�Q�b�g�̕�
	int width_;
	//�����_�����O�^�[�Q�b�g�̍���
	int height_;

	//�����_�����O�^�[�Q�b�g�r���[�̃N���A�J���[
	float rtv_Clear_Color_[4];
	//DSV�̃N���A�J���[
	float dsv_Clear_Value_;

};
