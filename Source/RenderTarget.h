#pragma once
#include "Texture.h"

class GraphicsEngine;

class RenderTarget
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	RenderTarget();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// レンダリングターゲットの作成
	/// </summary>
	/// <param name="graphicsEngine">デバイス関係を保有している</param>
	/// <param name="renderTargetWidth">レンダリングターゲットの幅</param>
	/// <param name="renderTargetHeight">レンダリングターゲットの高さ</param>
	/// <param name="mipLevel">ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。</param>
	/// <param name="arraySize">テクスチャ配列のサイズ</param>
	/// <param name="colorFormat">カラーバッファのフォーマット。</param>
	/// <param name="depthStencilFormat">深度ステンシルバッファのフォーマット。</param>
	/// <returns>trueが返ってきたら作成成功</returns>
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
	/// ディスクリプタヒープを作成
	/// </summary>
	/// <param name="graphicsEngine">グラフィックエンジン</param>
	/// <returns></returns>
	void CreateDescriptorHeap(GraphicsEngine& graphicsEngine);

	/// <summary>
	/// レンダリングターゲットとなるテクスチャを作成
	/// </summary>
	/// <param name="graphicsEngine">グラフィックエンジン</param>
	/// <param name="textureWidth">テクスチャの幅</param>
	/// <param name="textureHeight">テクスチャの高さ</param>
	/// <param name="mipLevel">ミップマップレベル</param>
	/// <param name="arraySize">テクスチャ配列のサイズ</param>
	/// <param name="format">テクスチャのフォーマット</param>
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
	/// 深度ステンシルバッファとなるテクスチャを作成
	/// </summary>
	/// <param name="graphicsEngine">グラフィックエンジン</param>
	/// <param name="textureWidth">テクスチャの幅</param>
	/// <param name="TextureHeight">テクスチャの高さ</param>
	/// <param name="format">テクスチャのフォーマット</param>
	/// <returns></returns>
	void CreateDepthStencilTexture(
		GraphicsEngine& graphicsEngine,
		int textureWidth,
		int textureHeight,
		DXGI_FORMAT format);

	/// <summary>
	/// ディスクリプタの作成
	/// </summary>
	/// <param name="d3dDevice">D3Dデバイス</param>
	/// <returns></returns>
	void CreateDescriptor(ID3D12Device5*& d3dDevice);
#pragma endregion

public:
#pragma region Get Method
	/// <summary>
	/// CPU側のレンダリングターゲットのディスクリプタハンドル 取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
	{
		return this->rtv_Heap_->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// CPU側のデプスステンシルバッファのディスクリプタハンドル 取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
	{
		return this->dsv_Heap_->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// レンダリングターゲットとなるテクスチャ 取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetRenderTargetTexture()
	{
		return this->render_Target_Texture_;
	}

	/// <summary>
	/// デプスステンシルバッファが存在しているか判定
	/// </summary>
	/// <returns></returns>
	bool IsExsitDepthStencilBuffer() const
	{
		return this->depth_Stencil_Texture_;
	}

	/// <summary>
	/// レンダリングターゲットの幅 取得。
	/// </summary>
	/// <returns></returns>
	int GetWidth() const
	{
		return this->width_;
	}

	/// <summary>
	/// レンダリングターゲットの高さ 取得。
	/// </summary>
	/// <returns></returns>
	int GetHeight() const
	{
		return this->height_;
	}

	/// <summary>
	/// カラーバッファのフォーマット 取得
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetColorBufferFormat() const
	{
		return this->render_Target_Texture_.GetFormat();
	}

	/// <summary>
	/// レンダリングターゲットビューのクリアカラー 取得
	/// </summary>
	/// <returns></returns>
	const float* GetRTVClearColor() const
	{
		return this->rtv_Clear_Color_;
	}

	/// <summary>
	/// DSVのクリアカラー 取得
	/// </summary>
	/// <returns></returns>
	float GetDSVClearValue() const
	{
		return this->dsv_Clear_Value_;
	}
#pragma endregion

private:
	//テクスチャ
	Texture render_Target_Texture_;
	//レンダリングターゲットとなるテクスチャ
	ID3D12Resource* render_Target_Texture_Dx12_;
	//深度ステンシルバッファとなるテクスチャ
	ID3D12Resource* depth_Stencil_Texture_;
	//RTV用のディスクリプタヒープ
	ID3D12DescriptorHeap* rtv_Heap_;
	//深度ステンシルバッファビューのディスクリプタヒープ
	ID3D12DescriptorHeap* dsv_Heap_;

	//フレームバッファのディスクリプタのサイズ
	UINT rtv_Descriptor_Size_;
	//深度ステンシルバッファのディスクリプタのサイズ
	UINT dsv_Descriptor_Size_;

	//レンダリングターゲットの幅
	int width_;
	//レンダリングターゲットの高さ
	int height_;

	//レンダリングターゲットビューのクリアカラー
	float rtv_Clear_Color_[4];
	//DSVのクリアカラー
	float dsv_Clear_Value_;

};
