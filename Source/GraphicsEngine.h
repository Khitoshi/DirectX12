#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <wrl.h>
#include <memory>
using namespace Microsoft::WRL;

class RenderContext;

class GraphicsEngine
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="frameBufferWidth">�t���[���o�b�t�@�̕�</param>
	/// <param name="frameBufferHeight">�t���[���o�b�t�@�̍���</param>
	/// <param name="hwnd">Windows �n���h��</param>
	GraphicsEngine(const HWND& hwnd,const UINT frameBufferWidth,const UINT frameBufferHeight);

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~GraphicsEngine();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>true�ŏ������ɐ���:false�ŏ������Ɏ��s</returns>
	bool Init();

private:
	/// <summary>
	/// DXGI�I�u�W�F�N�g�̐���
	/// </summary>
	void CreateDXGIFactory();

	/// <summary>
	/// �g�p����f�o�C�X�̐���
	/// </summary>
	void CreateD3DDevice();

	/// <summary>
	/// �R�}���h�L���[�̐���
	/// </summary>
	void CreateCommandQueue();

	/// <summary>
	/// �X���b�v�`�F�[��(�_�u���o�b�t�@)�̐���
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// �t���[���o�b�t�@�p�f�B�X�N���v�^�q�[�v�̐���
	/// </summary>
	void CreateDescriptorHeapForFrameBuffer();

	/// <summary>
	/// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g�r���[�̐���
	/// </summary>
	void CreateRTVForFameBuffer();

	/// <summary>
	/// �t���[���o�b�t�@�p�̐[�x�X�e���V���o�b�t�@�̐���
	/// </summary>
	void CreateDSVForFrameBuffer();

	/// <summary>
	/// �R�}���h�A���P�[�^�̐���
	/// </summary>
	void CreateCommandAllocator();

	/// <summary>
	/// �R�}���h���X�g�̐���
	/// </summary>
	void CreateCommandList();

	/// <summary>
	/// GPU�Ɠ����I�u�W�F�N�g����
	/// </summary>
	void CreateSynchronizationWithGPUObject();

public:

	ID3D12Device5* GetD3DDevice()const { return this->device_.Get(); }

private:
	//�t���[���o�b�t�@�̐�(�\�Ɨ���2��)
	enum { FRAME_BUFFER_COUNT = 2 };
private:
	//Windows �n���h��
	HWND hWnd;
	//�t���[���o�b�t�@�̕�
	UINT frame_Buffer_Width_;
	//�t���[���o�b�t�@�̍���
	UINT frame_Buffer_Height_;

	//DXGI�f�o�b�O�p�I�u�W�F�N�g
	ComPtr<IDXGIFactory4>factory_;
	//D3D�f�o�C�X
	ComPtr<ID3D12Device5>device_;
	//�R�}���h�L���[
	ComPtr<ID3D12CommandQueue> command_Queue_;
	//�X���b�v�`�F�[��
	ComPtr<IDXGISwapChain4> swap_Chain_;
	//�����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v�B
	ComPtr<ID3D12DescriptorHeap> rtv_Heap_;
	//�[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v�B
	ComPtr<ID3D12DescriptorHeap> dsv_Heap_;
	//�t���[���o�b�t�@�p�̃����_�����O�^�[�Q�b�g�B
	ComPtr<ID3D12Resource> render_Targets_[FRAME_BUFFER_COUNT];
	//�[�x�X�e���V���o�b�t�@�B
	ComPtr<ID3D12Resource> depth_Stencil_Buffer_;	
	//�R�}���h�A���P�[�^
	ComPtr<ID3D12CommandAllocator> command_Allocator_;
	//�R�}���h���X�g
	ComPtr<ID3D12GraphicsCommandList4> command_List_;
	ComPtr<ID3D12Fence> fence_;

	std::unique_ptr< RenderContext> render_Conext_;

	//���݂̃o�b�N�o�b�t�@�̔ԍ��B
	unsigned int current_Back_Buffer_Index_;

	//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT rtv_Descriptor_Size_;
	//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT dsv_Descriptor_Size_;
	UINT64 fence_Value_;
	HANDLE fence_Event_;
};
