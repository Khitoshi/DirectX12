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

private:
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
	ID3D12Device5* GetD3DDevice()const { return this->device_.Get(); }

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

	const NullTextureMaps& GetNullTextureMaps()const { return this->null_Texture_Maps_; }

	//フレームバッファの幅 取得
	const UINT GetFrameBufferWidth() { return this->frame_Buffer_Width_; }
	//フレームバッファの高さ 取得
	const UINT GetFrameBufferHeight() { return this->frame_Buffer_Width_; }

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
	ComPtr<ID3D12Fence> fence_;
	std::unique_ptr< RenderContext> render_Conext_;
	//ビューポート。
	D3D12_VIEWPORT view_Port_;
	//シザリング矩形。
	D3D12_RECT scissor_Rect_;

	//ヌルテクスチャ
	NullTextureMaps null_Texture_Maps_;

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

	//std::unique_ptr<Camera> camera_2d;
	//std::unique_ptr<Camera> camera_3d;
};
