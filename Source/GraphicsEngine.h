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
#include "NullTextureMaps.h"
using namespace Microsoft::WRL;

class RenderContext;
class Camera;

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
	/// <param name="camera">�J�������</param>
	/// <returns>true�ŏ������ɐ���:false�ŏ������Ɏ��s</returns>
	bool Init(Camera& camera);

private:
#pragma region Create Method
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

#pragma endregion


public:
#pragma region Get Method
	/// <summary>
	/// �f�o�C�X�擾
	/// </summary>
	/// <returns>this device</returns>
	ID3D12Device5* GetD3DDevice()const { return this->device_.Get(); }

	/// <summary>
	/// �o�b�N�o�b�t�@�̔ԍ����擾�B
	/// </summary>
	/// <returns>�o�b�N�o�b�t�@�̔ԍ��B</returns>
	UINT GetBackBufferIndex() const { return this->frame_Index; }

	/// <summary>
	/// �T���v���̃f�B�X�N���v�^�q�[�v�T�C�Y���擾�B
	/// </summary>
	/// <returns>this sapmer descriptor size</returns>
	UINT GetSapmerDescriptorSize() const { return this->sampler_Descriptor_Size_; }

	/// <summary>
	/// CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y���擾�B
	/// </summary>
	/// <returns>this cbr srv descriptor size</returns>
	UINT GetCbrSrvDescriptorSize() const { return this->cbr_Srv_Descriptor_Size_; }

	ID3D12CommandQueue* GetCommandQueue()const { return this->command_Queue_.Get(); }

	const NullTextureMaps& GetNullTextureMaps()const { return this->null_Texture_Maps_; }

	//�t���[���o�b�t�@�̕� �擾
	const UINT GetFrameBufferWidth() { return this->frame_Buffer_Width_; }
	//�t���[���o�b�t�@�̍��� �擾
	const UINT GetFrameBufferHeight() { return this->frame_Buffer_Width_; }

#pragma endregion


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
	//�r���[�|�[�g�B
	D3D12_VIEWPORT view_Port_;
	//�V�U�����O��`�B
	D3D12_RECT scissor_Rect_;

	//�k���e�N�X�`��
	NullTextureMaps null_Texture_Maps_;

	//���݂̃o�b�N�o�b�t�@�̔ԍ��B
	unsigned int current_Back_Buffer_Index_;

	//�t���[���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT rtv_Descriptor_Size_;
	//�[�x�X�e���V���o�b�t�@�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT dsv_Descriptor_Size_;
	//CBR_SRV�̃f�B�X�N���v�^�̃T�C�Y�B
	UINT cbr_Srv_Descriptor_Size_;
	//�T���v���̃f�B�X�N���v�^�̃T�C�Y�B			
	UINT sampler_Descriptor_Size_;
	UINT64 fence_Value_;
	// GPU�Ƃ̓����Ŏg�p����ϐ��B
	UINT frame_Index;

	HANDLE fence_Event_;

	//std::unique_ptr<Camera> camera_2d;
	//std::unique_ptr<Camera> camera_3d;
};
