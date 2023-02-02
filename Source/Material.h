#pragma once

#include <d3dx12.h>
#include <array>
#include <memory>

#include "TkmFile.h"
#include "Texture.h"

#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "Shader.h"

class tkEngine;
class RenderContext;

class Material
{
public:
	
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    Material();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
    ~Material();

	/// <summary>
	/// tkmファイルのマテリアル情報から初期化する。
	/// </summary>
	/// <param name="tkmMat">tkmマテリアル</param>
	void InitFromTkmMaterila(
		tkEngine*& tkEngine,
		GraphicsEngine*& graphicsEngine,
		const TkmFile::SMaterial& tkmMat,
		const char* fxFilePath,
		const char* vsEntryPointFuncName,
		const char* vsSkinEntriyPointFuncName,
		const char* psEntryPointFuncName,
		const std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat,
		int numSrv,
		int numCbv,
		UINT offsetInDescriptorsFromTableStartCB,
		UINT offsetInDescriptorsFromTableStartSRV,
		D3D12_FILTER samplerFilter
	);

	
	/// <summary>
	/// レンダリング開始する時に呼び出す関数
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="hasSkin"></param>
	void BeginRender(RenderContext& rc, int hasSkin);

private:
	/// <summary>
	/// パイプラインステートの初期化
	/// </summary>
	void InitPipelineState(GraphicsEngine*& graphicsEngine,const std::array < DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat);

	/// <summary>
	/// シェーダー初期化
	/// </summary>
	/// <param name="fxFilePath">.fx fileのファイルパス</param>
	/// <param name="vsEntryPointFuncName">頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="vsSkinEntriyPointFuncName">スキンありマテリアル用の頂点シェーダーのエントリーポイントの関数名</param>
	/// <param name="psEntryPointFuncName">ピクセルシェーダーのエントリーポイントの関数名</param>
	void InitShaders(
		tkEngine*& tkEngine,
		const char* fxFilePath,
		const char* vsEntryPointFuncName,
		const char* vsSkinEntriyPointFuncName,
		const char* psEntryPointFuncName
	);

	/// <summary>
	/// テクスチャを初期化。
	/// </summary>
	/// <param name="tkmMat"></param>
	void InitTexture(tkEngine*& tk,GraphicsEngine*& graphicsEngine, const TkmFile::SMaterial& tkmMat);

public:
#pragma region Get Method
	/// <summary>
	/// アルベドマップを取得
	/// </summary>
	/// <returns>this Albed Map</returns>
	Texture& GetAlbedoMap()
	{
		return *this->albedo_Map_;
	}

	/// <summary>
	/// 法線マップを取得
	/// </summary>
	/// <returns>this Normal Map</returns>
	Texture& GetNormalMap()
	{
		return *this->normal_Map_;
	}

	/// <summary>
	/// スペキュラマップを取得。
	/// </summary>
	/// <returns>this Specular Map</returns>
	Texture& GetSpecularMap()
	{
		return *this->specular_Map_;
	}

	/// <summary>
	/// 反射マップを取得
	/// </summary>
	/// <returns>this Reflection Map</returns>
	Texture& GetReflectionMap()
	{
		return *this->reflection_Map_;
	}

	/// <summary>
	/// 屈折マップを取得
	/// </summary>
	/// <returns>this Reflection Map</returns>
	Texture& GetRefractionMap()
	{
		return *this->refraction_Map_;
	}

	/// <summary>
	/// 定数バッファを取得。
	/// </summary>
	/// <returns>this Constant Buffer</returns>
	ConstantBuffer& GetConstantBuffer()
	{
		return this->constant_Buffer_;
	}

#pragma endregion

private:
	/// <summary>
	/// マテリアルパラメータ。
	/// </summary>
	struct SMaterialParam {
		int hasNormalMap;	//法線マップを保持しているかどうかのフラグ。
		int hasSpecMap;		//スペキュラマップを保持しているかどうかのフラグ。
	};

private:
#pragma region Map
	//アルベドマップ
	Texture* albedo_Map_;

	//法線マップ
	Texture* normal_Map_;

	//スペキュラマップ
	Texture* specular_Map_;

	//リフレクションマップ
	Texture* reflection_Map_;

	//屈折マップ
	Texture* refraction_Map_;

#pragma endregion

#pragma region Shader
	////スキンなしモデル用の頂点シェーダー
	//std::unique_ptr<Shader> vs_Non_Skin_Model_;
	//
	////スキンありモデル用の頂点シェーダー
	//std::unique_ptr<Shader> vs_Skin_Model_;
	//
	////モデル用のピクセルシェーダー
	//std::unique_ptr<Shader> ps_Model_;

	//スキンなしモデル用の頂点シェーダー
	Shader* vs_Non_Skin_Model_;

	//スキンありモデル用の頂点シェーダー
	Shader* vs_Skin_Model_;

	//モデル用のピクセルシェーダー
	Shader* ps_Model_;

#pragma endregion

#pragma region PipelineState
	//スキンなしモデル用のパイプラインステート
	PipelineState non_Skin_Model_Pipeline_State_;

	//スキンありモデル用のパイプラインステート
	PipelineState skin_Model_Pipeline_State_;

	//スキンありモデル用のパイプラインステート(半透明マテリアル)
	PipelineState trans_Skin_Model_Pipeline_State_;

	//スキンなしモデル用のパイプラインステート(半透明マテリアル)
	PipelineState trans_Non_Skin_Model_Pipeline_State_;

#pragma endregion

	//定数バッファ
	ConstantBuffer constant_Buffer_;

	//ルートシグネチャ
	RootSignature root_Signature_;
};

