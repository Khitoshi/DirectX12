#include "GraphicsEngine.h"
#include <d3dx12.h>
#include "RenderContext.h"
#include "NullTextureMaps.h"
#include "Camera.h"

//デフォルト コンストラクタ
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

//デフォルト デストラクタ
GraphicsEngine::~GraphicsEngine()
{}

//初期化
bool GraphicsEngine::Init(Camera& camera)
{
    //DXGIオブジェクトの生成
    this->CreateDXGIFactory();
	
	//使用するデバイスの生成
	this->CreateD3DDevice();

	//コマンドキュー生成
	this->CreateCommandQueue();

	//スワップチェーン(ダブルバッファ)生成
	this->CreateSwapChain();

	//フレームバッファ用のディスクリプタヒープを生成
	this->CreateDescriptorHeapForFrameBuffer();

	//フレームバッファ用のRTVの生成
	this->CreateRTVForFameBuffer();

	//フレームバッファ用のDSVの生成
	this->CreateDSVForFrameBuffer();

	//コマンドアロケータの生成
	this->CreateCommandAllocator();

	//コマンドリストの生成
	this->CreateCommandList();

	//GPUと同期オブジェクト生成
	this->CreateSynchronizationWithGPUObject();

	this->render_Conext_ = std::make_unique<RenderContext>();
	this->render_Conext_->Init(this->command_List_.Get());

	//ビューポート初期化
	//TODO: 関数にする
	this->view_Port_.TopLeftX = 0;
	this->view_Port_.TopLeftY = 0;
	this->view_Port_.Width = static_cast<FLOAT>(this->frame_Buffer_Width_);
	this->view_Port_.Height = static_cast<FLOAT>(this->frame_Buffer_Height_);
	this->view_Port_.MaxDepth = D3D12_MAX_DEPTH;
	this->view_Port_.MinDepth = D3D12_MIN_DEPTH;

	//シザリング初期化
	//TODO: 関数にする
	this->scissor_Rect_.left = 0;
	this->scissor_Rect_.top = 0;
	this->scissor_Rect_.right = this->frame_Buffer_Width_;
	this->scissor_Rect_.bottom = this->frame_Buffer_Height_;

	//CBR_SVRのディスクリプタのサイズを取得
	this->cbr_Srv_Descriptor_Size_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//Samplerのディスクリプタのサイズを取得
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

//DXGIオブジェクトの生成
void GraphicsEngine::CreateDXGIFactory()
{
    UINT dxgi_factory_flags = 0;

#ifdef _DEBUG
    //デバッグコントローラーがあれば，でバグレイヤーがあるDXGIを作成する
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();

		// Enable additional debug layers.
		dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		debugController->Release();
	}
#endif // _DEBUG
	//DXGIオブジェクト生成
    HRESULT hr = CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&this->factory_));

	//生成確認
    if (FAILED(hr))
    {
        MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDXGIFactoryで生成に失敗しました"), TEXT("エラー"), MB_OK);
        exit(-1);
    }
}

//使用するデバイスの生成
void GraphicsEngine::CreateD3DDevice()
{
	//使用している可能性のあるFEATURE_LEVELを列挙
    const D3D_FEATURE_LEVEL feature_levels[] = {
        D3D_FEATURE_LEVEL_12_1,	//Direct3D 12.1の機能を使う。
        D3D_FEATURE_LEVEL_12_0,	//Direct3D 12.0の機能を使う。
        D3D_FEATURE_LEVEL_11_1, //Direct3D 11.1の機能を使う。
        D3D_FEATURE_LEVEL_11_0, //Direct3D 11.0の機能を使う。
    };

	//GPUベンダー定義。
	enum GPU_Vender {
		GPU_VENDER_NVIDIA,	//NVIDIA
		GPU_VENDER_AMD,		//Intel
		GPU_VENDER_INTEL,	//AMD

		NUM_GPU_VENDER,		//Vender数
	};

	//TODO: スマートポインタにするとコンパイルエラーになるので要注意
	//ComPtr<IDXGIAdapter> adapter_temp;
	IDXGIAdapter* adapter_temp;
    //各ベンダーのアダプター
	IDXGIAdapter* adapter_vender[NUM_GPU_VENDER] = { nullptr };
    //最大ビデオメモリのアダプタ。
	IDXGIAdapter* adapter_max_video_memory = nullptr;
    //最終的に使用するアダプタ。
	IDXGIAdapter* use_adapter = nullptr;
	//ビデオメモリー比較用
    SIZE_T video_memory_size = 0;

    //ここはグラフィックスカードが複数枚刺さっている場合にどれが一番メモリ容量が多いかを調べ一番多いものを使用する為のloop
	for (int i = 0; this->factory_->EnumAdapters(i, &adapter_temp) != DXGI_ERROR_NOT_FOUND; i++) 
	{
		DXGI_ADAPTER_DESC desc;
		//ここでグラフィックス能力のあるdescを取得している
		//例(NVIDIA GeForce GTX 1080やMicrosoft Basic Render Driver(架空のビデオカードのデバイスドライバー))
		adapter_temp->GetDesc(&desc);

		if (desc.DedicatedVideoMemory > video_memory_size) 
		{
			//こちらのビデオメモリの方が多いので、こちらを使う。
			if (adapter_max_video_memory != nullptr) 
			{
				adapter_max_video_memory->Release();
			}
			adapter_max_video_memory = adapter_temp;
			//IDXGIAdapterを登録するたびにインクリメントしないといけないのでaddref(インクリメント)している
			adapter_max_video_memory->AddRef();
			video_memory_size = desc.DedicatedVideoMemory;
		}
		if (wcsstr(desc.Description, L"NVIDIA") != nullptr) 
		{
			//NVIDIA製
			if (adapter_vender[GPU_VENDER_NVIDIA])
			{
				adapter_vender[GPU_VENDER_NVIDIA]->Release();
			}
			adapter_vender[GPU_VENDER_NVIDIA] = adapter_temp;
			adapter_vender[GPU_VENDER_NVIDIA]->AddRef();
		}
		else if (wcsstr(desc.Description, L"AMD") != nullptr) 
		{
			//AMD製
			if (adapter_vender[GPU_VENDER_AMD]) 
			{
				adapter_vender[GPU_VENDER_AMD]->Release();
			}
			adapter_vender[GPU_VENDER_AMD] = adapter_temp;
			adapter_vender[GPU_VENDER_AMD]->AddRef();
		}
		else if (wcsstr(desc.Description, L"Intel") != nullptr) 
		{
			//Intel製
			if (adapter_vender[GPU_VENDER_INTEL]) 
			{
				adapter_vender[GPU_VENDER_INTEL]->Release();
			}
			adapter_vender[GPU_VENDER_INTEL] = adapter_temp;
			adapter_vender[GPU_VENDER_INTEL]->AddRef();
		}
		adapter_temp->Release();
	}
	//使用するアダプターを決める。
	if (adapter_vender[GPU_VENDER_NVIDIA] != nullptr) 
	{
		//NVIDIA製が最優先
		use_adapter = adapter_vender[GPU_VENDER_NVIDIA];
	}
	else if (adapter_vender[GPU_VENDER_AMD] != nullptr) 
	{
		//次はAMDが優先。
		use_adapter = adapter_vender[GPU_VENDER_AMD];
	}
	else 
	{
		//NVIDIAとAMDのGPUがなければビデオメモリが一番多いやつを使う。
		use_adapter = adapter_max_video_memory;
	}
	//PCのfeatureLevelを検索して一番レベルの高いものを設定する
	for (auto featureLevel : feature_levels)
	{
		auto hr = D3D12CreateDevice(
			use_adapter,
			featureLevel,
			IID_PPV_ARGS(&this->device_)
		);
		if (SUCCEEDED(hr)) 
		{
			//D3Dデバイスの作成に成功したので
			break;
		}
	}
	//取得した全てのグラフィックカードを解放する
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

	//生成確認
	if (!this->device_) {
		//D3Dデバイスの作成に失敗した。
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateD3DDeviceで生成に失敗しました"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
}

void GraphicsEngine::CreateCommandQueue()
{
	//コマンドキューを設定
	D3D12_COMMAND_QUEUE_DESC queue_desc = {};
	queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	
	//コマンドリストを生成
	HRESULT hr = this->device_->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&this->command_Queue_));

	//生成をチェック
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("コマンドキューの作成に失敗しました。"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
}

//スワップチェーンの生成
void GraphicsEngine::CreateSwapChain()
{
	

	//スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc = {};
	swap_chain_desc.BufferCount = FRAME_BUFFER_COUNT;
	swap_chain_desc.Width = frame_Buffer_Width_;
	swap_chain_desc.Height = frame_Buffer_Height_;
	swap_chain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swap_chain;
	//スワップチェーン生成
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
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSwapChainの生成に失敗しました"), TEXT("エラー"), MB_OK);
		exit(-1);
	}

	//IDXGISwapChain4のインターフェスを取得
	swap_chain->QueryInterface(IID_PPV_ARGS(&this->swap_Chain_));
	//swap_chain->Release();

	//現在のバックバッファの番号を取得
	this->current_Back_Buffer_Index_ = this->swap_Chain_->GetCurrentBackBufferIndex();
}

//フレームバッファ用のディスクリプタヒープの生成
void GraphicsEngine::CreateDescriptorHeapForFrameBuffer()
{
	//レンダリングターゲットビューのディスクリプタヒープ用のディスクリプタヒープを作成する。
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FRAME_BUFFER_COUNT;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	//生成
	HRESULT hr = this->device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->rtv_Heap_));
	if (FAILED(hr)) {
		//レンダリングターゲットビューのディスクリプタヒープ用のディスクリプタヒープの作成に失敗した。
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDescriptorHeapForFrameBufferの生成に失敗しました"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
	//ディスクリプタのサイズを取得。
	rtv_Descriptor_Size_ = this->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


	//深度ステンシルビューのディスクリプタヒープ用のディスクリプタヒープを作成する。
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	hr = this->device_->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&this->dsv_Heap_));
	if (FAILED(hr)) {
		//深度ステンシルビューのディスクリプタヒープ用のディスクリプタヒープの作成に失敗した。
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDescriptorHeapForFrameBufferの生成に失敗しました"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
	//ディスクリプタのサイズを取得。
	this->dsv_Descriptor_Size_ = this->device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
}

//フレームバッファ用のレンダーターゲットビューの生成
void GraphicsEngine::CreateRTVForFameBuffer()
{
	//ヒープの先頭を表すCPUディスクリプタハンドルを取得する。
	D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle = this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();

	//フロントバッファをバックバッファ用のRTVを作成。
	for (UINT n = 0; n < FRAME_BUFFER_COUNT; n++) {
		this->swap_Chain_->GetBuffer(n, IID_PPV_ARGS(&this->render_Targets_[n]));
		//レンダーターゲットビューの生成
		this->device_->CreateRenderTargetView(this->render_Targets_[n].Get(), nullptr, rtv_handle);
		//ポインタを渡したのでずらす
		rtv_handle.ptr += this->rtv_Descriptor_Size_;
	}

	//生成チェック
	for (const auto& rt : this->render_Targets_)
	{
		if (!rt)
		{
			//中身がnullの場合
			MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateRTVForFameBufferの生成に失敗"), TEXT("エラー"), MB_OK);
			exit(-1);
		}
	}
}

// フレームバッファ用の深度ステンシルバッファの生成
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

	//深度ステンシルバッファの生成
	auto heap_prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr = this->device_->CreateCommittedResource(
		&heap_prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&dsv_clear_value,
		IID_PPV_ARGS(&this->depth_Stencil_Buffer_)
	);

	//生成チェック
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateDSVForFrameBufferの生成に失敗"), TEXT("エラー"), MB_OK);
		exit(-1);
	}

	//ディスクリプタヒープの生成
	D3D12_CPU_DESCRIPTOR_HANDLE dsv_handle = this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
	
	//リソース データにアクセスするための深度ステンシル ビューを作成
	this->device_->CreateDepthStencilView(
		this->depth_Stencil_Buffer_.Get(), nullptr, dsv_handle
	);
}

//コマンドアロケータの生成
void GraphicsEngine::CreateCommandAllocator()
{
	//コマンドアロケータの作成。
	HRESULT hr = this->device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&this->command_Allocator_));

	//生成チェック
	if (FAILED(hr)) {
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateCommandAllocatorの生成に失敗"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
}

//コマンドリストの生成
void GraphicsEngine::CreateCommandList()
{
	//コマンドリスト生成
	HRESULT hr = this->device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, this->command_Allocator_.Get(), nullptr, IID_PPV_ARGS(&this->command_List_));

	//生成チェック
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateCommandListの生成に失敗"), TEXT("エラー"), MB_OK);
		exit(-1);
	}

	//コマンドリストは開かれている状態で生成されるので，一度閉じる
	this->command_List_->Close();
}

//GPUと同期オブジェクト生成
void GraphicsEngine::CreateSynchronizationWithGPUObject()
{
	//GPUと同期オブジェクト(fence)生成
	HRESULT  hr = this->device_->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->fence_));

	//生成チェック
	if (FAILED(hr))
	{
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObjectの生成に失敗"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
	this->fence_Value_ = 1;

	//同期を行う時のイベントハンドラを作成する
	this->fence_Event_ = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (this->fence_Event_ == nullptr) {
		MessageBox(this->hWnd, TEXT("GraphicsEngine::CreateSynchronizationWithGPUObjectの生成に失敗"), TEXT("エラー"), MB_OK);
		exit(-1);
	}
}
