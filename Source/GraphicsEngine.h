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
	/// コンストラクタ
	/// </summary>
	/// <param name="frameBufferWidth">フレームバッファの幅</param>
	/// <param name="frameBufferHeight">フレームバッファの高さ</param>
	/// <param name="hwnd">Windows ハンドル</param>
	GraphicsEngine(const HWND& hwnd,const UINT frameBufferWidth,const UINT frameBufferHeight);

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~GraphicsEngine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ情報</param>
	/// <returns>trueで初期化に成功:falseで初期化に失敗</returns>
	bool Init(Camera& camera);

	/// <summary>
	/// レンダリング開始
	/// </summary>
	/// <remarks>
	/// 1フレームのレンダリングの開始時に呼び出してください
	/// </remarks>
	void BeginRender(Camera& camera);

	/// <summary>
	/// レンダリング終了
	/// </summary>
	/// <remarks>
	/// 1フレームのレンダリングの終了時に呼び出してください
	/// </remarks>
	void EndRender();

#pragma region Device Method

	/// <summary>
	/// ディスクリプタヒープ 作成
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="descriptorHeap"></param>
	/// <param name="errorMessage">MessageBoxで表示する文字</param>
	void CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ID3D12DescriptorHeap*& descriptorHeap, const LPCWSTR errorMessage = L"CreateDescriptorHeapの作成に失敗")
	{
		//ディスクリプタヒープ 生成
		HRESULT hr = this->device_->CreateDescriptorHeap(
			&desc, 
			IID_PPV_ARGS(&descriptorHeap)
		);

		//生成　確認
		HRESULTCheck(hr, L"CreateDescriptorHeapで失敗");
	}
	void CreateDescriptorHeap(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ComPtr<ID3D12DescriptorHeap>& descriptorHeap, const LPCWSTR errorMessage = L"CreateDescriptorHeapの作成に失敗")
	{
		//ディスクリプタヒープ 生成
		HRESULT hr = this->device_->CreateDescriptorHeap(
			&desc, 
			IID_PPV_ARGS(&descriptorHeap)
		);

		//生成　確認
		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// デバイス(GPU)にリソースを作成するためのID3D12Resource 作成
	/// </summary>
	/// <param name="prop">heapの設定</param>
	/// <param name="heapFlag">ヒープ オプション</param>
	/// <param name="desc">リソースの設定</param>
	/// <param name="state">リソースの使用方法に関するリソースの状態を指定する定数</param>
	/// <param name="value">クリア カラーの既定値</param>
	/// <param name="resouce">生成したID3D12Resource</param>
	/// <param name="errorMessage">MessageBoxで表示する文字</param>
	void CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES& prop,
		const D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES state,
		const D3D12_CLEAR_VALUE* value,
		ID3D12Resource*& resouce,
		const LPCWSTR errorMessage = L"CreateCommittedResourceに失敗"
	)
	{
		//リソース作成
		HRESULT hr = this->device_->CreateCommittedResource(
			&prop,
			heapFlag,
			&desc,
			state,
			value,
			IID_PPV_ARGS(&resouce)
		);

		//生成 確認
		HRESULTCheck(hr, errorMessage);
	}
	void CreateCommittedResource(
		const D3D12_HEAP_PROPERTIES& prop,
		const D3D12_HEAP_FLAGS heapFlag,
		const D3D12_RESOURCE_DESC& desc,
		const D3D12_RESOURCE_STATES state,
		const D3D12_CLEAR_VALUE* value,
		ComPtr<ID3D12Resource>& resouce,
		const LPCWSTR errorMessage = L"CreateCommittedResourceに失敗"
	)
	{
		//リソース作成
		HRESULT hr = this->device_->CreateCommittedResource(
			&prop,
			heapFlag,
			&desc,
			state,
			value,
			IID_PPV_ARGS(&resouce)
		);

		//生成 確認
		HRESULTCheck(hr, errorMessage);
	}


	/// <summary>
	/// デスクリプタのバイト数 取得
	/// </summary>
	/// <param name="type">ディスクリプターヒープの種類</param>
	/// <returns>デスクリプタヒープの先頭アドレス+(要素のバイト数*N)</returns>
	const UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type)const
	{
		//ディスクリプターハンドルのサイズを取得
		return this->device_->GetDescriptorHandleIncrementSize(type);
	}


	/// <summary>
	/// レンダーターゲットビュー　作成
	/// </summary>
	/// <param name="resouce">レンダーターゲットを表すリソース</param>
	/// <param name="desc">nullの場合，ミップ0番のサブリソースにアクセス</param>
	/// <param name="handle">ディスクリプタヒープの開始位置のハンドル</param>
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
	/// 深度ステンシルビュー　作成
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
	/// 定数バッファービュー　作成
	/// </summary>
	/// <param name="desc">点数バッファビューの設定</param>
	/// <param name="handle">CPUディスクリプタハンドル</param>
	void CreateConstantBufferView(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateConstantBufferView(&desc, handle);
	}

	/// <summary>
	/// サンプラー 作成
	/// </summary>
	/// <param name="desc">サンプラの設定</param>
	/// <param name="handle">handle</param>
	void CreateSampler(const D3D12_SAMPLER_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateSampler(&desc, handle);
	}


	/// <summary>
	/// グラフィックパイプラインステート 作成
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="pipelineState"></param>
	/// <param name="errorMessage"></param>
	void CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ID3D12PipelineState*& pipelineState, const LPCWSTR errorMessage = L"CreateGraphicsPipelineStateに失敗")
	{
		HRESULT hr = this->device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}
	void CreateGraphicsPipelineState(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, ComPtr<ID3D12PipelineState>& pipelineState, const LPCWSTR errorMessage = L"CreateGraphicsPipelineStateに失敗")
	{
		HRESULT hr = this->device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// コンピュートパイプラインステート 作成
	/// </summary>
	/// <param name="desc"></param>
	/// <param name="pipelineState"></param>
	void CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ID3D12PipelineState*& pipelineState, const LPCWSTR errorMessage = L"CreateComputePipelineStateに失敗")
	{
		HRESULT hr = this->device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState));
		HRESULTCheck(hr, errorMessage);
	}
	void CreateComputePipelineState(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, ComPtr<ID3D12PipelineState>& pipelineState, const LPCWSTR errorMessage = L"CreateComputePipelineStateに失敗")
	{
		HRESULT hr = this->device_->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pipelineState));

		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// ルートシグネイチャ 作成
	/// </summary>
	/// <param name="nodeMask"></param>
	/// <param name="bufferPoint"></param>
	/// <param name="bufferSize"></param>
	/// <param name="rootSignature"></param>
	/// <param name="errorMessage"></param>
	void CreateRootSignature(UINT nodeMask, const LPVOID bufferPoint, SIZE_T bufferSize, ID3D12RootSignature*& rootSignature, const LPCWSTR errorMessage = L"CreateRootSignatureに失敗")
	{
		HRESULT hr = this->device_->CreateRootSignature(nodeMask, bufferPoint, bufferSize, IID_PPV_ARGS(&rootSignature));
		HRESULTCheck(hr, errorMessage);
	}
	void CreateRootSignature(UINT nodeMask, const LPVOID bufferPoint, SIZE_T bufferSize, ComPtr<ID3D12RootSignature>& rootSignature, const LPCWSTR errorMessage = L"CreateRootSignatureに失敗")
	{
		HRESULT hr = this->device_->CreateRootSignature(nodeMask, bufferPoint, bufferSize, IID_PPV_ARGS(&rootSignature));
		HRESULTCheck(hr, errorMessage);
	}

	/// <summary>
	/// シェーダーリソースビュー 作成
	/// </summary>
	/// <param name="resouce">シェーダーリソース</param>
	/// <param name="desc">シェーダーリソースの設定</param>
	/// <param name="handle">ハンドル</param>
	void CreateShaderResourceView(ID3D12Resource*& resouce, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateShaderResourceView(resouce, &desc, handle);
	}
	void CreateShaderResourceView(ComPtr<ID3D12Resource>& resouce, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc, D3D12_CPU_DESCRIPTOR_HANDLE& handle)
	{
		this->device_->CreateShaderResourceView(*resouce.GetAddressOf(), &desc, handle);
	}
	
	///// <summary>
	///// 現在使用しているDeviceでResourceUploadBatch 取得
	///// </summary>
	///// <returns></returns>
	//ResourceUploadBatch GetResourceUploadBatch()
	//{
	//	ResourceUploadBatch batch(device_.Get());
	//	return batch;
	//}

	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns></returns>
	ID3D12Device5* GetDevice() { return this->device_.Get(); }
	ID3D12Device5** GetDeviceAdd() { return this->device_.GetAddressOf(); }
#pragma endregion


private:
	void WaitDraw();

	/// <summary>
	/// HRESULTのエラー確認用 関数
	/// 失敗した場合MessageBoxが表示される　
	/// </summary>
	/// <param name="hr"></param>
	const void HRESULTCheck(const HRESULT hr,const LPCWSTR errorMessage)const
	{
		//成功の場合 早期リターン
		if (SUCCEEDED(hr))return;

		//RTV用のディスクリプタヒープの作成に失敗した
		MessageBox(nullptr, errorMessage, L"エラー", MB_OK);

		//異常終了
		std::abort();
	}

#pragma region Create Method
	/// <summary>
	/// DXGIオブジェクトの生成
	/// </summary>
	void CreateDXGIFactory();

	/// <summary>
	/// 使用するデバイスの生成
	/// </summary>
	void CreateD3DDevice();

	/// <summary>
	/// コマンドキューの生成
	/// </summary>
	void CreateCommandQueue();

	/// <summary>
	/// スワップチェーン(ダブルバッファ)の生成
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// フレームバッファ用ディスクリプタヒープの生成
	/// </summary>
	void CreateDescriptorHeapForFrameBuffer();

	/// <summary>
	/// フレームバッファ用のレンダーターゲットビューの生成
	/// </summary>
	void CreateRTVForFameBuffer();

	/// <summary>
	/// フレームバッファ用の深度ステンシルバッファの生成
	/// </summary>
	void CreateDSVForFrameBuffer();

	/// <summary>
	/// コマンドアロケータの生成
	/// </summary>
	void CreateCommandAllocator();

	/// <summary>
	/// コマンドリストの生成
	/// </summary>
	void CreateCommandList();

	/// <summary>
	/// GPUと同期オブジェクト生成
	/// </summary>
	void CreateSynchronizationWithGPUObject();

#pragma endregion

public:
#pragma region Get Method
	/// <summary>
	/// デバイス取得
	/// </summary>
	/// <returns>this device</returns>
	//ID3D12Device5& GetD3DDevice()const { return *this->device_.Get(); }

	/// <summary>
	/// バックバッファの番号を取得。
	/// </summary>
	/// <returns>バックバッファの番号。</returns>
	UINT GetBackBufferIndex() const { return this->frame_Index; }

	/// <summary>
	/// サンプラのディスクリプタヒープサイズを取得。
	/// </summary>
	/// <returns>this sapmer descriptor size</returns>
	UINT GetSapmerDescriptorSize() const { return this->sampler_Descriptor_Size_; }

	/// <summary>
	/// CBR_SRVのディスクリプタのサイズを取得。
	/// </summary>
	/// <returns>this cbr srv descriptor size</returns>
	UINT GetCbrSrvDescriptorSize() const { return this->cbr_Srv_Descriptor_Size_; }

	ID3D12CommandQueue* GetCommandQueue()const { return this->command_Queue_.Get(); }

	const NullTextureMaps& GetNullTextureMaps()const { return *this->null_Texture_Maps_; }

	//フレームバッファの幅 取得
	UINT GetFrameBufferWidth() const { return this->frame_Buffer_Width_; }
	//フレームバッファの高さ 取得
	UINT GetFrameBufferHeight() const { return this->frame_Buffer_Height_; }

	//フレームバッファの数 取得
	const static UINT GetFrameBufferCount() { return FRAME_BUFFER_COUNT; }

	//レンダーコンテキスト 取得
	RenderContext& GetRenderContext()
	{
		return *this->render_Conext_;
	}

#pragma endregion

private:
	//フレームバッファの数(表と裏で2枚)
	enum { FRAME_BUFFER_COUNT = 2 };
private:
	//Windows ハンドル
	HWND hWnd;
	//フレームバッファの幅
	UINT frame_Buffer_Width_;
	//フレームバッファの高さ
	UINT frame_Buffer_Height_;

	//DXGIデバッグ用オブジェクト
	ComPtr<IDXGIFactory4>factory_;
	//D3Dデバイス
	ComPtr<ID3D12Device5>device_;
	//コマンドキュー
	ComPtr<ID3D12CommandQueue> command_Queue_;
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swap_Chain_;
	//レンダリングターゲットビューのディスクリプタヒープ。
	ComPtr<ID3D12DescriptorHeap> rtv_Heap_;
	//深度ステンシルビューのディスクリプタヒープ。
	ComPtr<ID3D12DescriptorHeap> dsv_Heap_;
	//フレームバッファ用のレンダリングターゲット。
	ComPtr<ID3D12Resource> render_Targets_[FRAME_BUFFER_COUNT];
	//深度ステンシルバッファ。
	ComPtr<ID3D12Resource> depth_Stencil_Buffer_;	
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> command_Allocator_;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList4> command_List_;
	//パイプラインステート
	ComPtr<ID3D12PipelineState> pipeline_State_;
	//フェンス
	ComPtr<ID3D12Fence> fence_;
	//レンダーコンテキスト
	std::unique_ptr<RenderContext> render_Conext_;
	//ビューポート。
	D3D12_VIEWPORT view_Port_;
	//シザリング矩形。
	D3D12_RECT scissor_Rect_;

	//ヌルテクスチャ
	std::unique_ptr <NullTextureMaps> null_Texture_Maps_;

	//現在のバックバッファの番号。
	unsigned int current_Back_Buffer_Index_;

	//フレームバッファのディスクリプタのサイズ。
	UINT rtv_Descriptor_Size_;
	//深度ステンシルバッファのディスクリプタのサイズ。
	UINT dsv_Descriptor_Size_;
	//CBR_SRVのディスクリプタのサイズ。
	UINT cbr_Srv_Descriptor_Size_;
	//サンプラのディスクリプタのサイズ。			
	UINT sampler_Descriptor_Size_;
	UINT64 fence_Value_;
	// GPUとの同期で使用する変数。
	UINT frame_Index;

	HANDLE fence_Event_;

	D3D12_CPU_DESCRIPTOR_HANDLE current_Frame_Buffer_RTV_Handle_;
	D3D12_CPU_DESCRIPTOR_HANDLE current_Frame_Buffer_DSV_Handle_;

	//std::unique_ptr<Camera> camera_2d;
	//std::unique_ptr<Camera> camera_3d;
	//DirectXTKのグラフィックメモリシステム
	std::unique_ptr<DirectX::GraphicsMemory> directXTKG_Fx_Memroy_;	

};
