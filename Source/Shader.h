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
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    Shader();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~Shader();


	/// <summary>
	/// �s�N�Z���V�F�[�_�[�����[�h
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
	void LoadPS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// ���_�V�F�[�_�[�����[�h
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
	void LoadVS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// �R���s���[�g�V�F�[�_�[�����[�h
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="entryFuncName"></param>
	void LoadCS(const char* filePath, const char* entryFuncName);
	
	/// <summary>
	/// ���C�g���[�V���O�p�̃V�F�[�_�[�����[�h
	/// </summary>
	//void LoadRaytracing(const wchar_t* filePath);

private:
	/// <summary>
	/// �V�F�[�_�[�����[�h
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="entryFuncName">�G���g���[�|�C���g�̊֐����B</param>
	/// <param name="shaderModel">�V�F�[�_�[���f��</param>
	void Load(const char* filePath, const char* entryFuncName, const char* shaderModel);

	//�����m�F�p
	void CreateCheck(HRESULT hr, std::wstring message)const;


public:
#pragma region Get Method
	/// <summary>
	/// �R���p�C���ς݃V�F�[�_�[�f�[�^���擾�B
	/// </summary>
	/// <returns>this blob</returns>
	ID3DBlob* GetCompiledBlob() const
	{
		return this->blob_.Get();
	}

	/// <summary>
	/// //DXC�R���p�C�����g�p�����Ƃ��̃V�F�[�_�[�f�[�^�B
	/// </summary>
	/// <returns>this dxc Blob</returns>
	IDxcBlob* GetCompiledDxcBlob() const
	{
		return this->dxc_Blob_.Get();
	}

	/// <summary>
	/// �������ςݔ��ʗp�t���O�擾
	/// </summary>
	/// <returns>true = �������ς�</returns>
	bool IsInited()const
	{
		return this->is_Inited_;
	}

	/// <summary>
	/// �s�N�Z���V�F�[�_�[���f���� �擾
	/// </summary>
	/// <returns></returns>
	static const std::string GetPixelShaderModelName() { return "ps_5_0"; }

	/// <summary>
	/// ���_�V�F�[�_�[���f���� �擾
	/// </summary>
	/// <returns></returns>
	static const std::string GetVertexShaderModelName() { return "vs_5_0"; }

	/// <summary>
	/// �R���s���[�g�V�F�[�_�[���f���� �擾
	/// </summary>
	/// <returns></returns>
	static const std::string GetComputeShaderModelName() { return "cs_5_0"; }

#pragma endregion


private:
	//�R���p�C���ς݂̃V�F�[�_�[�f�[�^
	ComPtr<ID3DBlob> blob_;

	//DXC�R���p�C�����g�p�����Ƃ��̃V�F�[�_�[�f�[�^
	ComPtr<IDxcBlob> dxc_Blob_;

	bool is_Inited_;

};