#pragma once

#include <d3dx12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>

using namespace Microsoft::WRL;


class Shader
{
public:
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    Shader();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~Shader();


	/// <summary>
	/// ピクセルシェーダーをロード
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="entryFuncName">エントリーポイントの関数名。</param>
	void LoadPS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// 頂点シェーダーをロード
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="entryFuncName">エントリーポイントの関数名。</param>
	void LoadVS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// コンピュートシェーダーをロード
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="entryFuncName"></param>
	void LoadCS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// レイトレーシング用のシェーダーをロード
	/// </summary>
	//void LoadRaytracing(const wchar_t* filePath);

private:
	/// <summary>
	/// シェーダーをロード
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="entryFuncName">エントリーポイントの関数名。</param>
	/// <param name="shaderModel">シェーダーモデル</param>
	void Load(const char* filePath, const char* entryFuncName, const char* shaderModel);

	//生成確認用
	void CreateCheck(HRESULT hr, std::wstring message)const;


public:
#pragma region Get Method
	/// <summary>
	/// コンパイル済みシェーダーデータを取得。
	/// </summary>
	/// <returns>this blob</returns>
	ID3DBlob* GetCompiledBlob() const
	{
		return this->blob_.Get();
	}

	/// <summary>
	/// //DXCコンパイラを使用したときのシェーダーデータ。
	/// </summary>
	/// <returns>this dxc Blob</returns>
	IDxcBlob* GetCompiledDxcBlob() const
	{
		return this->dxc_Blob_.Get();
	}

	/// <summary>
	/// 初期化済み判別用フラグ取得
	/// </summary>
	/// <returns>true = 初期化済み</returns>
	bool IsInited()const
	{
		return this->is_Inited_;
	}

	/// <summary>
	/// ピクセルシェーダーモデル名 取得
	/// </summary>
	/// <returns></returns>
	static const std::string GetPixelShaderModelName() { return "ps_5_0"; }

	/// <summary>
	/// 頂点シェーダーモデル名 取得
	/// </summary>
	/// <returns></returns>
	static const std::string GetVertexShaderModelName() { return "vs_5_0"; }

	/// <summary>
	/// コンピュートシェーダーモデル名 取得
	/// </summary>
	/// <returns></returns>
	static const std::string GetComputeShaderModelName() { return "cs_5_0"; }

#pragma endregion


private:
	//コンパイル済みのシェーダーデータ
	ComPtr<ID3DBlob> blob_;

	//DXCコンパイラを使用したときのシェーダーデータ
	ComPtr<IDxcBlob> dxc_Blob_;

	bool is_Inited_;

};