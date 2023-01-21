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
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    tkEngine();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~tkEngine();

	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈��
	/// </summary>
	void BeginFrame();

	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈��
	/// </summary>
	void EndFrame();

	/// <summary>
	/// �Q�[���G���W���̏�����
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);

public:
#pragma region Register
	/// <summary>
	/// �e�N�X�`�����o���N�ɓo�^
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="texture">�e�N�X�`��</param>
	void RegistTextureToBank(const char* filePath, Texture* texture)
	{
		this->texture_Bank_.Regist(filePath, texture);
	}

	/// <summary>
	/// tkm�t�@�C�����o���N�ɓo�^
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		this->tkm_File_Bank_.Regist(filePath, tkmFile);
	}

	/// <summary>
	/// �V�F�[�_�[�o���N����V�F�[�_�[��o�^
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
	/// �o���N����e�N�X�`�����擾
	/// </summary>
	/// <param name="filePath">file path</param>
	/// <returns>�e�N�X�`��</returns>
	Texture* GetTextureFromBank(const char* filePath)
	{
		return this->texture_Bank_.Get(filePath);
	}

	/// <summary>
	/// .tkm file���o���N����擾
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>tkm�t�@�C�� nullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ�</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return this->tkm_File_Bank_.Get(filePath);
	}

	/// <summary>
	/// �V�F�[�_�[�t�@�C���o���N����V�F�[�_�[���擾
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <param name="entryPointFuncName">�V�F�[�_�G���g���[�֐���</param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return this->shader_Bank_.Get(programName.c_str());
	}

#pragma endregion

private:

	//�O���t�B�b�N�G���W���B
	GraphicsEngine* graphics_Engine_;
	//.tkm file�o���N�B
	TResourceBank<TkmFile> tkm_File_Bank_;
	//�V�F�[�_�[�o���N
	TResourceBank<Shader> shader_Bank_;
	//�e�N�X�`���o���N�B
	TResourceBank<Texture> texture_Bank_;
	//�Q�[���p�b�h�B
	GamePad pad_[GamePad::CONNECT_PAD_MAX];
};

