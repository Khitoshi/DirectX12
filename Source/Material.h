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
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    Material();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
    ~Material();

	/// <summary>
	/// tkm�t�@�C���̃}�e���A����񂩂珉��������B
	/// </summary>
	/// <param name="tkmMat">tkm�}�e���A��</param>
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
	/// �����_�����O�J�n���鎞�ɌĂяo���֐�
	/// </summary>
	/// <param name="rc"></param>
	/// <param name="hasSkin"></param>
	void BeginRender(RenderContext& rc, int hasSkin);

private:
	/// <summary>
	/// �p�C�v���C���X�e�[�g�̏�����
	/// </summary>
	void InitPipelineState(GraphicsEngine*& graphicsEngine,const std::array < DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat);

	/// <summary>
	/// �V�F�[�_�[������
	/// </summary>
	/// <param name="fxFilePath">.fx file�̃t�@�C���p�X</param>
	/// <param name="vsEntryPointFuncName">���_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="vsSkinEntriyPointFuncName">�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	/// <param name="psEntryPointFuncName">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̊֐���</param>
	void InitShaders(
		tkEngine*& tkEngine,
		const char* fxFilePath,
		const char* vsEntryPointFuncName,
		const char* vsSkinEntriyPointFuncName,
		const char* psEntryPointFuncName
	);

	/// <summary>
	/// �e�N�X�`�����������B
	/// </summary>
	/// <param name="tkmMat"></param>
	void InitTexture(tkEngine*& tk,GraphicsEngine*& graphicsEngine, const TkmFile::SMaterial& tkmMat);

public:
#pragma region Get Method
	/// <summary>
	/// �A���x�h�}�b�v���擾
	/// </summary>
	/// <returns>this Albed Map</returns>
	Texture& GetAlbedoMap()
	{
		return *this->albedo_Map_;
	}

	/// <summary>
	/// �@���}�b�v���擾
	/// </summary>
	/// <returns>this Normal Map</returns>
	Texture& GetNormalMap()
	{
		return *this->normal_Map_;
	}

	/// <summary>
	/// �X�y�L�����}�b�v���擾�B
	/// </summary>
	/// <returns>this Specular Map</returns>
	Texture& GetSpecularMap()
	{
		return *this->specular_Map_;
	}

	/// <summary>
	/// ���˃}�b�v���擾
	/// </summary>
	/// <returns>this Reflection Map</returns>
	Texture& GetReflectionMap()
	{
		return *this->reflection_Map_;
	}

	/// <summary>
	/// ���܃}�b�v���擾
	/// </summary>
	/// <returns>this Reflection Map</returns>
	Texture& GetRefractionMap()
	{
		return *this->refraction_Map_;
	}

	/// <summary>
	/// �萔�o�b�t�@���擾�B
	/// </summary>
	/// <returns>this Constant Buffer</returns>
	ConstantBuffer& GetConstantBuffer()
	{
		return this->constant_Buffer_;
	}

#pragma endregion

private:
	/// <summary>
	/// �}�e���A���p�����[�^�B
	/// </summary>
	struct SMaterialParam {
		int hasNormalMap;	//�@���}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
		int hasSpecMap;		//�X�y�L�����}�b�v��ێ����Ă��邩�ǂ����̃t���O�B
	};

private:
#pragma region Map
	//�A���x�h�}�b�v
	Texture* albedo_Map_;

	//�@���}�b�v
	Texture* normal_Map_;

	//�X�y�L�����}�b�v
	Texture* specular_Map_;

	//���t���N�V�����}�b�v
	Texture* reflection_Map_;

	//���܃}�b�v
	Texture* refraction_Map_;

#pragma endregion

#pragma region Shader
	////�X�L���Ȃ����f���p�̒��_�V�F�[�_�[
	//std::unique_ptr<Shader> vs_Non_Skin_Model_;
	//
	////�X�L�����胂�f���p�̒��_�V�F�[�_�[
	//std::unique_ptr<Shader> vs_Skin_Model_;
	//
	////���f���p�̃s�N�Z���V�F�[�_�[
	//std::unique_ptr<Shader> ps_Model_;

	//�X�L���Ȃ����f���p�̒��_�V�F�[�_�[
	Shader* vs_Non_Skin_Model_;

	//�X�L�����胂�f���p�̒��_�V�F�[�_�[
	Shader* vs_Skin_Model_;

	//���f���p�̃s�N�Z���V�F�[�_�[
	Shader* ps_Model_;

#pragma endregion

#pragma region PipelineState
	//�X�L���Ȃ����f���p�̃p�C�v���C���X�e�[�g
	PipelineState non_Skin_Model_Pipeline_State_;

	//�X�L�����胂�f���p�̃p�C�v���C���X�e�[�g
	PipelineState skin_Model_Pipeline_State_;

	//�X�L�����胂�f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)
	PipelineState trans_Skin_Model_Pipeline_State_;

	//�X�L���Ȃ����f���p�̃p�C�v���C���X�e�[�g(�������}�e���A��)
	PipelineState trans_Non_Skin_Model_Pipeline_State_;

#pragma endregion

	//�萔�o�b�t�@
	ConstantBuffer constant_Buffer_;

	//���[�g�V�O�l�`��
	RootSignature root_Signature_;
};

