#pragma once

//#include <Windows.h>

#include "Texture.h"
#include "GamePad.h"
#include "TResourceBank.h"
#include "TkmFile.h"
#include "Shader.h"

class tkEngine
{
public:
    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    tkEngine();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~tkEngine();

	/// <summary>
	/// フレームの開始時に呼ばれる処理
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// フレームの終了時に呼ばれる処理
	/// </summary>
	void EndFrame();

	/// <summary>
	/// ゲームエンジンの初期化
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

public:
#pragma region Register
	/// <summary>
	/// テクスチャをバンクに登録
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="texture">テクスチャ</param>
	void RegistTextureToBank(const char* filePath, Texture* texture)
	{
		this->texture_Bank_.Regist(filePath, texture);
	}

	/// <summary>
	/// tkmファイルをバンクに登録
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		this->tkm_File_Bank_.Regist(filePath, tkmFile);
	}

	/// <summary>
	/// シェーダーバンクからシェーダーを登録
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		this->shader_Bank_.Regist(programName.c_str(), shader);
	}

#pragma endregion

#pragma region Get Method

	/// <summary>
	/// バンクからテクスチャを取得
	/// </summary>
	/// <param name="filePath">file path</param>
	/// <returns>テクスチャ</returns>
	Texture* GetTextureFromBank(const char* filePath)
	{
		return this->texture_Bank_.Get(filePath);
	}

	/// <summary>
	/// .tkm fileをバンクから取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>tkmファイル nullptrが返ってきたらバンクに登録されていない</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return this->tkm_File_Bank_.Get(filePath);
	}

	/// <summary>
	/// シェーダーファイルバンクからシェーダーを取得
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="entryPointFuncName">シェーダエントリー関数名</param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return this->shader_Bank_.Get(programName.c_str());
	}

#pragma endregion

private:

	//グラフィックエンジン。
	GraphicsEngine* graphics_Engine_;
	//.tkm fileバンク。
	TResourceBank<TkmFile> tkm_File_Bank_;
	//シェーダーバンク
	TResourceBank<Shader> shader_Bank_;
	//テクスチャバンク。
	TResourceBank<Texture> texture_Bank_;
	//ゲームパッド。
	GamePad pad_[GamePad::CONNECT_PAD_MAX];
};

