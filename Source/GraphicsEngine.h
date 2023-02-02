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
#include <GraphicsMemory.h>
//#include <ResourceUploadBatch.h>
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

	/// <summary>
	/// �����_�����O�J�n
	/// </summary>
	/// <remarks>
	/// 1�t���[���̃����_�����O�̊J�n���ɌĂяo���Ă�������
	/// </remarks>
	void BeginRender(Camera& camera);

	/// <summary>
	/// �����_�����O�I��
	/// </summary>
	/// <remarks>
	/// 1�t���[���̃����_�����O�̏I�����ɌĂяo���Ă�������
	/// </remarks>
	void EndRender();

#pragma region Device Method

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v �쐬
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="descriptorHeap"></param>
	/// <param name="errorMessage">MessageBox�ŕ\�����镶��</param>
	void CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ID3D12DescriptorHeap*& descriptorHeap, const LPCWSTR errorMessage = L"CreateDescriptorHeap�̍쐬�Ɏ��s")
	{
		//�f�B�X�N���v�^�q�[�v ����
		HRESULT hr = this->device_->CreateDescriptorHeap(
			&desc, 
			IID_PPV_ARGS(&descriptorHeap)
		);

		//�����@�m�F
		HRESULTCheck(hr, L"CreateDescriptorHeap�Ŏ��s");
	}
	void CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, const LPCWSTR errorMessage = L"CreateDescriptorHeap�̍쐬�Ɏ��s")
	{
		//�f�B�X�N���v�^�q�[�v ����
		HRESULT hr = this->device_->CreateDescriptorHeap(
			&desc, 
			IID_PPV_ARGS(&descriptorHeap)
		);

		//�����@�m�F
		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// �f�o�C�X(GPU)�Ƀ��\�[�X���쐬���邽�߂�ID3D12Resource �쐬
	/// </summary>
	/// <param name="prop">heap�̐ݒ�</param>
	/// <param name="heapFlag">�q�[�v �I�v�V����</param>
	/// <param name="desc">���\�[�X�̐ݒ�</param>
	/// <param name="state">���\�[�X�̎g�p���@�Ɋւ��郊�\�[�X�̏�Ԃ��w�肷��萔</param>
	/// <param name="value">�N���A �J���[�̊���l</param>
	/// <param name="resouce">��������ID3D12Resource</param>
	/// <param name="errorMessage">MessageBox�ŕ\�����镶��</param>
	void CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES& prop,
		const D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES state,
		const D3D12_CLEAR_VALUE* value,
		ID3D12Resource*& resouce,
		const LPCWSTR errorMessage = L"CreateCommittedResource�Ɏ��s"
	)
	{
		//���\�[�X�쐬
		HRESULT hr = this->device_->CreateCommittedResource(
			&prop,
			heapFlag,
			&desc,
			state,
			value,
			IID_PPV_ARGS(&resouce)
		);

		//���� �m�F
		HRESULTCheck(hr, errorMessage);
	}
	void CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES& prop,
		const D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES state,
		const D3D12_CLEAR_VALUE* value,
		ComPtr<ID3D12Resource>& resouce,
		const LPCWSTR errorMessage = L"CreateCommittedResource�Ɏ��s"
	)
	{
		//���\�[�X�쐬
		HRESULT hr = this->device_->CreateCommittedResource(
			&prop,
			heapFlag,
			&desc,
			state,
			value,
			IID_PPV_ARGS(&resouce)
		);

		//���� �m�F
		HRESULTCheck(hr, errorMessage);
	}


	/// <summary>
	/// �f�X�N���v�^�̃o�C�g�� �擾
	/// </summary>
	/// <param name="type">�f�B�X�N���v�^�[�q�[�v�̎��</param>
	/// <returns>�f�X�N���v�^�q�[�v�̐擪�A�h���X+(�v�f�̃o�C�g��*N)</returns>
	const UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type)const
	{
		//�f�B�X�N���v�^�[�n���h���̃T�C�Y���擾
		return this->device_->GetDescriptorHandleIncrementSize(type);
	}


	/// <summary>
	/// �����_�[�^�[�Q�b�g�r���[�@�쐬
	/// </summary>
	/// <param name="resouce">�����_�[�^�[�Q�b�g��\�����\�[�X</param>
	/// <param name="desc">null�̏ꍇ�C�~�b�v0�Ԃ̃T�u���\�[�X�ɃA�N�Z�X</param>
	/// <param name="handle">�f�B�X�N���v�^�q�[�v�̊J�n�ʒu�̃n���h��</param>
	/// <param name=""></param>
	void CreateRenderTargetView(
		ID3D12Resource*& resouce,
		const D3D12_RENDER_TARGET_VIEW_DESC* desc,
		D3D12_CPU_DESCRIPTOR_HANDLE handle
	)
	{
		this->device_->CreateRenderTargetView(
			resouce,
			desc,
			handle
		);
	}

	/// <summary>
	/// �[�x�X�e���V���r���[�@�쐬
	/// </summary>
	/// <param name="resouce"></param>
	/// <param name="desc"></param>
	/// <param name="handle"></param>
	void CreateDepthStencilView(
		ID3D12Resource*& resouce,
		const D3D12_DEPTH_STENCIL_VIEW_DESC* desc,
		D3D12_CPU_DESCRIPTOR_HANDLE handle
	)
	{
		this->device_->CreateDepthStencilView(
			resouce,
			desc,
			handle
		);
	};

	/// <summary>
	/// �萔�o�b�t�@�[�r���[�@�쐬
	/// </summary>
	/// <param name="desc">�_���o�b�t�@�r���[�̐ݒ�</param>
	/// <param name="handle">CPU�f�B�X�N���v�^�n���h��</param>
	void CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateConstantBufferView(&desc, handle);
	}

	/// <summary>
	/// �T���v���[ �쐬
	/// </summary>
	/// <param name="desc">�T���v���̐ݒ�</param>
	/// <param name="handle">handle</param>
	void CreateSampler(const D3D12_SAMPLER_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateSampler(&desc, handle);
	}


	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���X�e�[�g �쐬
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="pipelineState"></param>
	/// <param name="errorMessage"></param>
	void CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3D12PipelineState*& pipelineState, const LPCWSTR errorMessage = L"CreateGraphicsPipelineState�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}
	void CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ComPtr<ID3D12PipelineState>& pipelineState, const LPCWSTR errorMessage = L"CreateGraphicsPipelineState�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// �R���s���[�g�p�C�v���C���X�e�[�g �쐬
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="pipelineState"></param>
	void CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ID3D12PipelineState*& pipelineState, const LPCWSTR errorMessage = L"CreateComputePipelineState�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState));
		HRESULTCheck(hr, errorMessage);
	}
	void CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ComPtr<ID3D12PipelineState>& pipelineState, const LPCWSTR errorMessage = L"CreateComputePipelineState�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// ���[�g�V�O�l�C�`�� �쐬
	/// </summary>
	/// <param name="nodeMask"></param>
	/// <param name="bufferPoint"></param>
	/// <param name="bufferSize"></param>
	/// <param name="rootSignature"></param>
	/// <param name="errorMessage"></param>
	void CreateRootSignature(UINT nodeMask, const LPVOID bufferPoint, SIZE_T bufferSize, ID3D12RootSignature*& rootSignature, const LPCWSTR errorMessage = L"CreateRootSignature�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateRootSignature(nodeMask, bufferPoint, bufferSize, IID_PPV_ARGS(&rootSignature));
		HRESULTCheck(hr, errorMessage);
	}
	void CreateRootSignature(UINT nodeMask, const LPVOID bufferPoint, SIZE_T bufferSize, ComPtr<ID3D12RootSignature>& rootSignature, const LPCWSTR errorMessage = L"CreateRootSignature�Ɏ��s")
	{
		HRESULT hr = this->device_->CreateRootSignature(nodeMask, bufferPoint, bufferSize, IID_PPV_ARGS(&rootSignature));
		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[ �쐬
	/// </summary>
	/// <param name="resouce">�V�F�[�_�[���\�[�X</param>
	/// <param name="desc">�V�F�[�_�[���\�[�X�̐ݒ�</param>
	/// <param name="handle">�n���h��</param>
	void CreateShaderResourceView(ID3D12Resource*& resouce, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateShaderResourceView(resouce, &desc, handle);
	}
	void CreateShaderResourceView(ComPtr<ID3D12Resource>& resouce, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateShaderResourceView(*resouce.GetAddressOf(), &desc, handle);
	}
	
	///// <summary>
	///// ���ݎg�p���Ă���Device��ResourceUploadBatch �擾
	///// </summary>
	///// <returns></returns>
	//ResourceUploadBatch GetResourceUploadBatch()
	//{
	//	ResourceUploadBatch batch(device_.Get());
	//	return batch;
	//}

	/// <summary>
	/// �f�o�C�X�擾
	/// </summary>
	/// <returns></returns>
	ID3D12Device5* GetDevice() { return this->device_.Get(); }
	ID3D12Device5** GetDeviceAdd() { return this->device_.GetAddressOf(); }
#pragma endregion


private:
	void WaitDraw();

	/// <summary>
	/// HRESULT�̃G���[�m�F�p �֐�
	/// ���s�����ꍇMessageBox���\�������@
	/// </summary>
	/// <param name="hr"></param>
	const void HRESULTCheck(const HRESULT hr,const LPCWSTR errorMessage)const
	{
		//�����̏ꍇ �������^�[��
		if (SUCCEEDED(hr))return;

		//RTV�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s����
		MessageBox(nullptr, errorMessage, L"�G���[", MB_OK);

		//�ُ�I��
		std::abort();
	}

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
	//ID3D12Device5& GetD3DDevice()const { return *this->device_.Get(); }

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

	const NullTextureMaps& GetNullTextureMaps()const { return *this->null_Texture_Maps_; }

	//�t���[���o�b�t�@�̕� �擾
	UINT GetFrameBufferWidth() const { return this->frame_Buffer_Width_; }
	//�t���[���o�b�t�@�̍��� �擾
	UINT GetFrameBufferHeight() const { return this->frame_Buffer_Height_; }

	//�t���[���o�b�t�@�̐� �擾
	const static UINT GetFrameBufferCount() { return FRAME_BUFFER_COUNT; }

	//�����_�[�R���e�L�X�g �擾
	RenderContext& GetRenderContext()
	{
		return *this->render_Conext_;
	}

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
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipeline_State_;
	//�t�F���X
	ComPtr<ID3D12Fence> fence_;
	//�����_�[�R���e�L�X�g
	std::unique_ptr<RenderContext> render_Conext_;
	//�r���[�|�[�g�B
	D3D12_VIEWPORT view_Port_;
	//�V�U�����O��`�B
	D3D12_RECT scissor_Rect_;

	//�k���e�N�X�`��
	std::unique_ptr <NullTextureMaps> null_Texture_Maps_;

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

	D3D12_CPU_DESCRIPTOR_HANDLE current_Frame_Buffer_RTV_Handle_;
	D3D12_CPU_DESCRIPTOR_HANDLE current_Frame_Buffer_DSV_Handle_;

	//std::unique_ptr<Camera> camera_2d;
	//std::unique_ptr<Camera> camera_3d;
	//DirectXTK�̃O���t�B�b�N�������V�X�e��
	std::unique_ptr<DirectX::GraphicsMemory> directXTKG_Fx_Memroy_;	

};
