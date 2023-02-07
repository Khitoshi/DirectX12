#include "Shader.h"
#include <d3dcompiler.h>
#include <stierr.h>
#include <fstream>
#include <sstream>
#include <atlcomcli.h>

#pragma warning(disable : 4996)
//デフォルト コンストラクタ 
Shader::Shader():
    blob_(nullptr),
    dxc_Blob_(nullptr),

    is_Inited_(false)
{
}

//デフォルト デストラクタ
Shader::~Shader()
{
}

//ピクセルシェーダーをロード
void Shader::LoadPS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetPixelShaderModelName().c_str());
}
//頂点シェーダーをロード
void Shader::LoadVS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetVertexShaderModelName().c_str());
}

//コンピュートシェーダーをロード
void Shader::LoadCS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetComputeShaderModelName().c_str());
}

//レイトレーシング用のシェーダーをロード
/*
void Shader::LoadRaytracing(const wchar_t* filePath)
{
    //ファイルを読み込む
    std::ifstream shader_file(filePath);
    if (!shader_file.good())
    {
        std::wstring error_message = L"シェーダーファイルのオープンに失敗しました\n";
        error_message += filePath;
        MessageBoxW(nullptr, error_message.c_str(), L"エラー", MB_OK);
        std::abort();
    }

    std::stringstream str_stream;
    str_stream << shader_file.rdbuf();
    std::string shader = str_stream.str();

    //生成　結果確認用フラグ
    HRESULT hr;

    //シェーダーのテキストファイルから、BLOBを作成する。
    CComPtr<IDxcLibrary> dxc_lib;
    hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxc_lib));
    //生成 確認
    if (FAILED(hr))
    {
        //失敗したのでMessageBoxを作り異常終了する
        MessageBox(nullptr, L"DXCLIBの作成に失敗しました。", L"エラー", MB_OK);
        std::abort();
    }

    //インクルードハンドル 生成
    CComPtr<IDxcIncludeHandler> includer_handler;
    hr = dxc_lib->CreateIncludeHandler(&includer_handler);
    CreateCheck(hr, L"CreateIncludeHandlerの生成に失敗");

    //dxcコンパイラ 生成
    CComPtr<IDxcCompiler> dxc_compiler;
    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler));
    CreateCheck(hr, L"Dxcコンパイラの生成に失敗");

    //ソースコードのBLOBを作成する。
    uint32_t code_page = CP_UTF8;
    CComPtr<IDxcBlobEncoding> source_blob;
    hr = dxc_lib->CreateBlobFromFile(filePath, &code_page, &source_blob);
    CreateCheck(hr, L"シェーダーソースのBlobの生成に失敗");

    // dxcインクルードハンドル 生成
    CComPtr<IDxcIncludeHandler> dxcIncludeHandler;
    hr = dxc_lib->CreateIncludeHandler(&dxcIncludeHandler);
    CreateCheck(hr, L"Dxc CreateIncludeHandlerの生成に失敗");

    const wchar_t* args[] = {
        L"-I Assets\\shader",
    };


    //コンパイル
    CComPtr<IDxcOperationResult> result;
    hr = dxc_compiler->Compile(
        source_blob, // pSource
        filePath, // pSourceName
        L"",		// pEntryPoint
        L"lib_6_3", // pTargetProfile
        args, 1, // pArguments, argCount
        nullptr, 0, // pDefines, defineCount
        dxcIncludeHandler, // pIncludeHandler
        &result); // ppResult
    //成功
    if (SUCCEEDED(hr))
    {
        result->GetStatus(&hr);
    }
        
    //失敗
    if (FAILED(hr))
    {
        if (result)
        {
            CComPtr<IDxcBlobEncoding> errors_blob;
            hr = result->GetErrorBuffer(&errors_blob);
            if (SUCCEEDED(hr) && errors_blob)
            {
                std::string error_message = "Compilation failed with errors:\n%hs\n";
                error_message += (const char*)errors_blob->GetBufferPointer();
                MessageBoxA(nullptr, error_message.c_str(), "エラー", MB_OK);
                std::abort();
            }
        }
        else
        {
            MessageBox(nullptr, TEXT("コンパイルに失敗"), L"エラー", MB_OK);
            std::abort();
        }
    }
    else
    {
        result->GetResult(&this->dxc_Blob_);
    }

}*/

//シェーダーをロード
void Shader::Load(const char* filePath, const char* entryFuncName, const char* shaderModel)
{
    ComPtr<ID3DBlob> error_blob;
    //ID3DBlob* error_blob;
#ifdef _DEBUG
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif // _DEBUG
    //ワイド文字へ変換
    wchar_t wfx_file_path[256] = { L"" };
    mbstowcs(wfx_file_path, filePath, 256);

    //HLSLコードをコンパイル
    HRESULT hr = D3DCompileFromFile(
        wfx_file_path,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryFuncName,
        shaderModel,
        compileFlags,
        0,
        &this->blob_,
        &error_blob
    );

    //コンパイルチェック
    if (FAILED(hr))
    {
        //失敗したのでエラー原因を表示
        if (hr == STIERR_OBJECTNOTFOUND) {
            std::wstring errorMessage;
            errorMessage = L"指定されたfxファイルが開けませんでした。";
            errorMessage += wfx_file_path;
            MessageBoxW(nullptr, errorMessage.c_str(), L"エラー", MB_OK);

            //異常終了
            std::abort();
        }
        if (error_blob) {
            static char errorMessage[10 * 1024];
            sprintf_s(errorMessage, "filePath : %ws, %s", wfx_file_path, (char*)error_blob->GetBufferPointer());
            MessageBoxA(NULL, errorMessage, "シェーダーエラー", MB_OK);
            //異常終了
            std::abort();
        }
        else
        {
            MessageBox(nullptr, TEXT("Shader::Loadに失敗しました"), L"エラー", MB_OK);
            //異常終了
            std::abort();
        }
    }

    //初期化完了したので初期化済みフラグを建てる
    this->is_Inited_ = true;
}

void Shader::CreateCheck(HRESULT hr, std::wstring message)const
{
    //確認
    if (FAILED(hr))
    {
        //失敗したのでメッセージボックスを表示
        MessageBoxW(nullptr, message.c_str(), L"エラー", MB_OK);
        //異常終了する
        std::abort();
    }
}
