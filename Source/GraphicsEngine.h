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
	/// <returns>trueで初期化に成功:falseで初期化に失敗</returns>
	bool Init();

private:
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

public:

	ID3D12Device5* GetD3DDevice()const { return this->device_.Get(); }

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
	ComPtr<ID3D12Fence> fence_;

	std::unique_ptr< RenderContext> render_Conext_;

	//現在のバックバッファの番号。
	unsigned int current_Back_Buffer_Index_;

	//フレームバッファのディスクリプタのサイズ。
	UINT rtv_Descriptor_Size_;
	//深度ステンシルバッファのディスクリプタのサイズ。
	UINT dsv_Descriptor_Size_;
	UINT64 fence_Value_;
	HANDLE fence_Event_;
};
