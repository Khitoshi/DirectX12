#include "GraphicsEngine.h"
#include <d3dx12.h>
#include "RenderContext.h"
#include "NullTextureMaps.h"
#include "Camera.h"

//�f�t�H���g �R���X�g���N�^
GraphicsEngine::GraphicsEngine(const HWND& hwnd, const UINT frameBufferWidth, const UINT frameBufferHeight):
	hWnd(hwnd),
    frame_Buffer_Width_(frameBufferWidth),
    frame_Buffer_Height_(frameBufferHeight),

    factory_(),
	device_(),
	command_Queue_(),
	swap_Chain_(),
	rtv_Heap_(),
	dsv_Heap_(),
	render_Targets_(),
	depth_Stencil_Buffer_(),
	command_Allocator_(),
	command_List_(),
	fence_(),
	view_Port_(),
	scissor_Rect_(),
	current_Back_Buffer_Index_(0),

	rtv_Descriptor_Size_(0),
	dsv_Descriptor_Size_(0),
	cbr_Srv_Descriptor_Size_(0),
	sampler_Descriptor_Size_(0),
	fence_Value_(0),
	frame_Index(0),
	fence_Event_()
{}

//�f�t�H���g �f�X�g���N�^
GraphicsEngine::~GraphicsEngine()
{}

//������
bool GraphicsEngine::Init(Camera& camera)
{
    //DXGI�I�u�W�F�N�g�̐���
    this->CreateDXGIFactory();
	
	//�g�p����f�o�C�X�̐���
	this->CreateD3DDevice();

	//�R�}���h�L���[����
	this->CreateCommandQueue();

	//�X���b�v�`�F�[��(�_�u���o�b�t�@)����
	this->CreateSwapChain();

	//�t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v�𐶐�
	this->CreateDescriptorHeapForFrameBuffer();

	//�t���[���o�b�t�@�p��RTV�̐���
	this->CreateRTVForFameBuffer();

	//�t���[���o�b�t�@�p��DSV�̐���
	this->CreateDSVForFrameBuffer();

	//�R�}���h�A���P�[�^�̐���
	this->CreateCommandAllocator();

	//�R�}���h���X�g�̐���
	this->CreateCommandList();

	//GPU�Ɠ����I�u�W�F�N�g����
	this->CreateSynchronizationWithGPUObject();

	this->render_Conext_ = std::make_unique<RenderContext>();
	this->render_Conext_->Init(this->command_List_.Get());

	//�r���[�|�[�g������
	//TODO: �֐��ɂ���
	this->view_Port_.TopLeftX = 0;
	this->view_Port_.TopLeftY = 0;
	this->view_Port_.Width = static_cast<FLOAT>(this->frame_Buffer_Width_);
	this->view_Port_.Height = static_cast<FLOAT>(this->frame_Buffer_Height_);
	this->view_Port_.MaxDepth = D3D12_MAX_DEPTH;
	this->view_Port_.MinDepth = D3D12_MIN_DEPTH;

	//�V�U�����O������
	//TODO: �֐��ɂ���
	this->scissor_Rect_.left = 0;
	this->scissor_Rect_.top = 0;
	this->scissor_Rect_.right = this->frame_Buffer_Width_;
	this->scissor_Rect_.bottom = this->frame_Buffer_Height_;

	//CBR_SVR�̃f�B�X�N���v�^�̃T�C�Y���擾
	this->cbr_Srv_Descriptor_Size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Sampler�̃f�B�X�N���v�^�̃T�C�Y���擾
	this->sampler_Descriptor_Size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	this->null_Texture_Maps_ = std::make_unique<NullTextureMaps>();
	this->null_Texture_Maps_->Init();

	//std::unique_ptr<Camera> camera_2d;
	std::unique_ptr<Camera> camera_3d;

	//camera_2d = std::make_unique<Camera>();
	//camera_2d->set
	camera_3d = std::make_unique<Camera>();
	camera_3d->GetPosition().Set(0.0f, 50.0f, 200.0f);
	camera_3d->GetTarget().Set(0.0f, 50.0f, 0.0f);
	
	camera = *camera_3d;

    return true;
}

//DXGI�I�u�W�F�N�g�̐���
void GraphicsEngine::CreateDXGIFactory()
{
    UINT dxgi_factory_flags = 0;

#ifdef _DEBUG
    //�f�o�b�O�R���g���[���[������΁C�Ńo�O���C���[������DXGI���쐬����
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers.
		dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		debugController->Release();
	}
#endif // _DEBUG
	//DXGI�I�u�W�F�N�g����
    HRESULT hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&this->factory_));

	//�����m�F
    if (FAILED(hr))
    {
        MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDXGIFactory�Ő����Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
        exit(-1);
    }
}

//�g�p����f�o�C�X�̐���
void GraphicsEngine::CreateD3DDevice()
{
	//�g�p���Ă���\���̂���FEATURE_LEVEL���
    const D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1�̋@�\���g���B
        D3D_FEATURE_LEVEL_12_0,	//Direct3D 12.0�̋@�\���g���B
        D3D_FEATURE_LEVEL_11_1, //Direct3D 11.1�̋@�\���g���B
        D3D_FEATURE_LEVEL_11_0, //Direct3D 11.0�̋@�\���g���B
    };

	//GPU�x���_�[��`�B
	enum GPU_Vender {
		GPU_VENDER_NVIDIA,	//NVIDIA
		GPU_VENDER_AMD,		//Intel
		GPU_VENDER_INTEL,	//AMD

		NUM_GPU_VENDER,		//Vender��
	};

	//TODO: �X�}�[�g�|�C���^�ɂ���ƃR���p�C���G���[�ɂȂ�̂ŗv����
	//ComPtr<IDXGIAdapter> adapter_temp;
	IDXGIAdapter* adapter_temp;
    //�e�x���_�[�̃A�_�v�^�[
	IDXGIAdapter* adapter_vender[NUM_GPU_VENDER] = { nullptr };
    //�ő�r�f�I�������̃A�_�v�^�B
	IDXGIAdapter* adapter_max_video_memory = nullptr;
    //�ŏI�I�Ɏg�p����A�_�v�^�B
	IDXGIAdapter* use_adapter = nullptr;
	//�r�f�I�������[��r�p
    SIZE_T video_memory_size = 0;

    //�����̓O���t�B�b�N�X�J�[�h���������h�����Ă���ꍇ�ɂǂꂪ��ԃ������e�ʂ��������𒲂׈�ԑ������̂��g�p����ׂ�loop
	for (int i = 0; this->factory_->EnumAdapters(i, &adapter_temp) != DXGI_ERROR_NOT_FOUND; i++) 
	{
		DXGI_ADAPTER_DESC desc;
		//�����ŃO���t�B�b�N�X�\�͂̂���desc���擾���Ă���
		//��(NVIDIA GeForce GTX 1080��Microsoft Basic Render Driver(�ˋ�̃r�f�I�J�[�h�̃f�o�C�X�h���C�o�[))
		adapter_temp->GetDesc(&desc);

		if (desc.DedicatedVideoMemory > video_memory_size) 
		{
			//������̃r�f�I�������̕��������̂ŁA��������g���B
			if (adapter_max_video_memory != nullptr) 
			{
				adapter_max_video_memory->Release();
			}
			adapter_max_video_memory = adapter_temp;
			//IDXGIAdapter��o�^���邽�тɃC���N�������g���Ȃ��Ƃ����Ȃ��̂�addref(�C���N�������g)���Ă���
			adapter_max_video_memory->AddRef();
			video_memory_size = desc.DedicatedVideoMemory;
		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) 
		{
			//NVIDIA��
			if (adapter_vender[GPU_VENDER_NVIDIA])
			{
				adapter_vender[GPU_VENDER_NVIDIA]->Release();
			}
			adapter_vender[GPU_VENDER_NVIDIA] = adapter_temp;
			adapter_vender[GPU_VENDER_NVIDIA]->AddRef();
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) 
		{
			//AMD��
			if (adapter_vender[GPU_VENDER_AMD]) 
			{
				adapter_vender[GPU_VENDER_AMD]->Release();
			}
			adapter_vender[GPU_VENDER_AMD] = adapter_temp;
			adapter_vender[GPU_VENDER_AMD]->AddRef();
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) 
		{
			//Intel��
			if (adapter_vender[GPU_VENDER_INTEL]) 
			{
				adapter_vender[GPU_VENDER_INTEL]->Release();
			}
			adapter_vender[GPU_VENDER_INTEL] = adapter_temp;
			adapter_vender[GPU_VENDER_INTEL]->AddRef();
		}
		adapter_temp->Release();
	}
	//�g�p����A�_�v�^�[�����߂�B
	if (adapter_vender[GPU_VENDER_NVIDIA] != nullptr) 
	{
		//NVIDIA�����ŗD��
		use_adapter = adapter_vender[GPU_VENDER_NVIDIA];
	}
	else if (adapter_vender[GPU_VENDER_AMD] != nullptr) 
	{
		//����AMD���D��B
		use_adapter = adapter_vender[GPU_VENDER_AMD];
	}
	else 
	{
		//NVIDIA��AMD��GPU���Ȃ���΃r�f�I����������ԑ�������g���B
		use_adapter = adapter_max_video_memory;
	}
	//PC��featureLevel���������Ĉ�ԃ��x���̍������̂�ݒ肷��
	for (auto featureLevel : feature_levels)
	{
		auto hr = D3D12CreateDevice(
			use_adapter,
			featureLevel,
			IID_PPV_ARGS(&this->device_)
		);
		if (SUCCEEDED(hr)) 
		{
			//D3D�f�o�C�X�̍쐬�ɐ��������̂�
			break;
		}
	}
	//�擾�����S�ẴO���t�B�b�N�J�[�h���������
	for (auto& adapter : adapter_vender) 
	{
		if (adapter) {
			adapter->Release();
		}
	}
	if (adapter_max_video_memory) 
	{
		adapter_max_video_memory->Release();
	}

	//�����m�F
	if (!this->device_) {
		//D3D�f�o�C�X�̍쐬�Ɏ��s�����B
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateD3DDevice�Ő����Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
}

void GraphicsEngine::CreateCommandQueue()
{
	//�R�}���h�L���[��ݒ�
	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	
	//�R�}���h���X�g�𐶐�
	HRESULT hr = this->device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&this->command_Queue_));

	//�������`�F�b�N
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("�R�}���h�L���[�̍쐬�Ɏ��s���܂����B"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
}

//�X���b�v�`�F�[���̐���
void GraphicsEngine::CreateSwapChain()
{
	

	//�X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	swap_chain_desc.BufferCount = FRAME_BUFFER_COUNT;
	swap_chain_desc.Width = frame_Buffer_Width_;
	swap_chain_desc.Height = frame_Buffer_Height_;
	swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swap_chain;
	//�X���b�v�`�F�[������
	HRESULT hr = this->factory_->CreateSwapChainForHwnd(
		this->command_Queue_.Get(), 
		this->hWnd, 
		&swap_chain_desc, 
		nullptr, 
		nullptr, 
		&swap_chain
	);

	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSwapChain�̐����Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
		exit(-1);
	}

	//IDXGISwapChain4�̃C���^�[�t�F�X���擾
	swap_chain->QueryInterface(IID_PPV_ARGS(&this->swap_Chain_));
	//swap_chain->Release();

	//���݂̃o�b�N�o�b�t�@�̔ԍ����擾
	this->current_Back_Buffer_Index_ = this->swap_Chain_->GetCurrentBackBufferIndex();
}

//�t���[���o�b�t�@�p�̃f�B�X�N���v�^�q�[�v�̐���
void GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	//�����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v�p�̃f�B�X�N���v�^�q�[�v���쐬����B
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//����
	HRESULT hr = this->device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->rtv_Heap_));
	if (FAILED(hr)) {
		//�����_�����O�^�[�Q�b�g�r���[�̃f�B�X�N���v�^�q�[�v�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDescriptorHeapForFrameBuffer�̐����Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
	//�f�B�X�N���v�^�̃T�C�Y���擾�B
	rtv_Descriptor_Size_ = this->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	//�[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v�p�̃f�B�X�N���v�^�q�[�v���쐬����B
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = this->device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));
	if (FAILED(hr)) {
		//�[�x�X�e���V���r���[�̃f�B�X�N���v�^�q�[�v�p�̃f�B�X�N���v�^�q�[�v�̍쐬�Ɏ��s�����B
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDescriptorHeapForFrameBuffer�̐����Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
	//�f�B�X�N���v�^�̃T�C�Y���擾�B
	this->dsv_Descriptor_Size_ = this->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

//�t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g�r���[�̐���
void GraphicsEngine::CreateRTVForFameBuffer()
{
	//�q�[�v�̐擪��\��CPU�f�B�X�N���v�^�n���h�����擾����B
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();

	//�t�����g�o�b�t�@���o�b�N�o�b�t�@�p��RTV���쐬�B
	for (UINT n = 0; n < FRAME_BUFFER_COUNT; n++) {
		this->swap_Chain_->GetBuffer(n, IID_PPV_ARGS(&this->render_Targets_[n]));
		//�����_�[�^�[�Q�b�g�r���[�̐���
		this->device_->CreateRenderTargetView(this->render_Targets_[n].Get(), nullptr, rtv_handle);
		//�|�C���^��n�����̂ł��炷
		rtv_handle.ptr += this->rtv_Descriptor_Size_;
	}

	//�����`�F�b�N
	for (const auto& rt : this->render_Targets_)
	{
		if (!rt)
		{
			//���g��null�̏ꍇ
			MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateRTVForFameBuffer�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
			exit(-1);
		}
	}
}

// �t���[���o�b�t�@�p�̐[�x�X�e���V���o�b�t�@�̐���
void GraphicsEngine::CreateDSVForFrameBuffer()
{
	D3D12_CLEAR_VALUE dsv_clear_value;
	dsv_clear_value.Format = DXGI_FORMAT_D32_FLOAT;
	dsv_clear_value.DepthStencil.Depth = 1.0f;
	dsv_clear_value.DepthStencil.Stencil = 0;

	CD3DX12_RESOURCE_DESC desc;		
	desc.Dimension			=	D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment			=	0;
	desc.Width				=	frame_Buffer_Width_;
	desc.Height				=	frame_Buffer_Height_;
	desc.DepthOrArraySize	=	1;
	desc.MipLevels			=	1;
	desc.Format				=	DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count	=	1;
	desc.SampleDesc.Quality =	0;
	desc.Layout				=	D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags				=	D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

	//�[�x�X�e���V���o�b�t�@�̐���
	auto heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr = this->device_->CreateCommittedResource(
		&heap_prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsv_clear_value,
		IID_PPV_ARGS(&this->depth_Stencil_Buffer_)
	);

	//�����`�F�b�N
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDSVForFrameBuffer�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
		exit(-1);
	}

	//�f�B�X�N���v�^�q�[�v�̐���
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
	
	//���\�[�X �f�[�^�ɃA�N�Z�X���邽�߂̐[�x�X�e���V�� �r���[���쐬
	this->device_->CreateDepthStencilView(
		this->depth_Stencil_Buffer_.Get(), nullptr, dsv_handle
	);
}

//�R�}���h�A���P�[�^�̐���
void GraphicsEngine::CreateCommandAllocator()
{
	//�R�}���h�A���P�[�^�̍쐬�B
	HRESULT hr = this->device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->command_Allocator_));

	//�����`�F�b�N
	if (FAILED(hr)) {
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateCommandAllocator�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
}

//�R�}���h���X�g�̐���
void GraphicsEngine::CreateCommandList()
{
	//�R�}���h���X�g����
	HRESULT hr = this->device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->command_Allocator_.Get(), nullptr, IID_PPV_ARGS(&this->command_List_));

	//�����`�F�b�N
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateCommandList�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
		exit(-1);
	}

	//�R�}���h���X�g�͊J����Ă����ԂŐ��������̂ŁC��x����
	this->command_List_->Close();
}

//GPU�Ɠ����I�u�W�F�N�g����
void GraphicsEngine::CreateSynchronizationWithGPUObject()
{
	//GPU�Ɠ����I�u�W�F�N�g(fence)����
	HRESULT  hr = this->device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fence_));

	//�����`�F�b�N
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObject�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
	this->fence_Value_ = 1;

	//�������s�����̃C�x���g�n���h�����쐬����
	this->fence_Event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (this->fence_Event_ == nullptr) {
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObject�̐����Ɏ��s"), TEXT("�G���["), MB_OK);
		exit(-1);
	}
}
