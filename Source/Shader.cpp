#include "Shader.h"
#include <d3dcompiler.h>
#include <stierr.h>
#include <fstream>
#include <sstream>
#include <atlcomcli.h>

#pragma warning(disable : 4996)
//�f�t�H���g �R���X�g���N�^ 
Shader::Shader():
    blob_(nullptr),
    dxc_Blob_(nullptr),

    is_Inited_(false)
{
}

//�f�t�H���g �f�X�g���N�^
Shader::~Shader()
{
}

//�s�N�Z���V�F�[�_�[�����[�h
void Shader::LoadPS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetPixelShaderModelName().c_str());
}
//���_�V�F�[�_�[�����[�h
void Shader::LoadVS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetVertexShaderModelName().c_str());
}

//�R���s���[�g�V�F�[�_�[�����[�h
void Shader::LoadCS(const char* filePath, const char* entryFuncName)
{
    Load(filePath, entryFuncName, this->GetComputeShaderModelName().c_str());
}

//���C�g���[�V���O�p�̃V�F�[�_�[�����[�h
/*
void Shader::LoadRaytracing(const wchar_t* filePath)
{
    //�t�@�C����ǂݍ���
    std::ifstream shader_file(filePath);
    if (!shader_file.good())
    {
        std::wstring error_message = L"�V�F�[�_�[�t�@�C���̃I�[�v���Ɏ��s���܂���\n";
        error_message += filePath;
        MessageBoxW(nullptr, error_message.c_str(), L"�G���[", MB_OK);
        std::abort();
    }

    std::stringstream str_stream;
    str_stream << shader_file.rdbuf();
    std::string shader = str_stream.str();

    //�����@���ʊm�F�p�t���O
    HRESULT hr;

    //�V�F�[�_�[�̃e�L�X�g�t�@�C������ABLOB���쐬����B
    CComPtr<IDxcLibrary> dxc_lib;
    hr = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxc_lib));
    //���� �m�F
    if (FAILED(hr))
    {
        //���s�����̂�MessageBox�����ُ�I������
        MessageBox(nullptr, L"DXCLIB�̍쐬�Ɏ��s���܂����B", L"�G���[", MB_OK);
        std::abort();
    }

    //�C���N���[�h�n���h�� ����
    CComPtr<IDxcIncludeHandler> includer_handler;
    hr = dxc_lib->CreateIncludeHandler(&includer_handler);
    CreateCheck(hr, L"CreateIncludeHandler�̐����Ɏ��s");

    //dxc�R���p�C�� ����
    CComPtr<IDxcCompiler> dxc_compiler;
    hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_compiler));
    CreateCheck(hr, L"Dxc�R���p�C���̐����Ɏ��s");

    //�\�[�X�R�[�h��BLOB���쐬����B
    uint32_t code_page = CP_UTF8;
    CComPtr<IDxcBlobEncoding> source_blob;
    hr = dxc_lib->CreateBlobFromFile(filePath, &code_page, &source_blob);
    CreateCheck(hr, L"�V�F�[�_�[�\�[�X��Blob�̐����Ɏ��s");

    // dxc�C���N���[�h�n���h�� ����
    CComPtr<IDxcIncludeHandler> dxcIncludeHandler;
    hr = dxc_lib->CreateIncludeHandler(&dxcIncludeHandler);
    CreateCheck(hr, L"Dxc CreateIncludeHandler�̐����Ɏ��s");

    const wchar_t* args[] = {
        L"-I Assets\\shader",
    };


    //�R���p�C��
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
    //����
    if (SUCCEEDED(hr))
    {
        result->GetStatus(&hr);
    }
        
    //���s
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
                MessageBoxA(nullptr, error_message.c_str(), "�G���[", MB_OK);
                std::abort();
            }
        }
        else
        {
            MessageBox(nullptr, TEXT("�R���p�C���Ɏ��s"), L"�G���[", MB_OK);
            std::abort();
        }
    }
    else
    {
        result->GetResult(&this->dxc_Blob_);
    }

}*/

//�V�F�[�_�[�����[�h
void Shader::Load(const char* filePath, const char* entryFuncName, const char* shaderModel)
{
    ComPtr<ID3DBlob> error_blob;
    //ID3DBlob* error_blob;
#ifdef _DEBUG
    UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compileFlags = 0;
#endif // _DEBUG
    //���C�h�����֕ϊ�
    wchar_t wfx_file_path[256] = { L"" };
    mbstowcs(wfx_file_path, filePath, 256);

    //HLSL�R�[�h���R���p�C��
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

    //�R���p�C���`�F�b�N
    if (FAILED(hr))
    {
        //���s�����̂ŃG���[������\��
        if (hr == STIERR_OBJECTNOTFOUND) {
            std::wstring errorMessage;
            errorMessage = L"�w�肳�ꂽfx�t�@�C�����J���܂���ł����B";
            errorMessage += wfx_file_path;
            MessageBoxW(nullptr, errorMessage.c_str(), L"�G���[", MB_OK);

            //�ُ�I��
            std::abort();
        }
        if (error_blob) {
            static char errorMessage[10 * 1024];
            sprintf_s(errorMessage, "filePath : %ws, %s", wfx_file_path, (char*)error_blob->GetBufferPointer());
            MessageBoxA(NULL, errorMessage, "�V�F�[�_�[�G���[", MB_OK);
            //�ُ�I��
            std::abort();
        }
        else
        {
            MessageBox(nullptr, TEXT("Shader::Load�Ɏ��s���܂���"), L"�G���[", MB_OK);
            //�ُ�I��
            std::abort();
        }
    }

    //���������������̂ŏ������ς݃t���O�����Ă�
    this->is_Inited_ = true;
}

void Shader::CreateCheck(HRESULT hr, std::wstring message)const
{
    //�m�F
    if (FAILED(hr))
    {
        //���s�����̂Ń��b�Z�[�W�{�b�N�X��\��
        MessageBoxW(nullptr, message.c_str(), L"�G���[", MB_OK);
        //�ُ�I������
        std::abort();
    }
}
