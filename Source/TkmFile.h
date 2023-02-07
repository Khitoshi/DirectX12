#pragma once
#include <string>
#include <functional>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
class TkmFile
{
public:
	/// <summary>
	/// �}�e���A��
	/// </summary>
	struct SMaterial {
		std::string albedoMapFileName;			//�A���x�h�}�b�v�̃t�@�C����
		std::string normalMapFileName;			//�@���}�b�v�̃t�@�C����
		std::string specularMapFileName;		//�X�y�L�����}�b�v�̃t�@�C����
		std::string reflectionMapFileName;		//���t���N�V�����}�b�v�̃t�@�C����
		std::string refractionMapFileName;		//���܃}�b�v�̃t�@�C����
		std::unique_ptr<char[]>	albedoMap;		//���[�h���ꂽ�A���x�h�}�b�v(dds�t�@�C��)
		unsigned int albedoMapSize;				//�A���x�h�}�b�v�̃T�C�Y(dds�t�@�C��)
		std::unique_ptr<char[]>	normalMap;		//���[�h���ꂽ�@���}�b�v(dds�t�@�C��)
		unsigned int normalMapSize;				//�@���}�b�v�̃T�C�Y
		std::unique_ptr<char[]>	specularMap;	//���[�h���ꂽ�X�y�L�����}�b�v(dds�t�@�C��)
		unsigned int specularMapSize;			//�X�y�L�����}�b�v�̃T�C�Y(dds�t�@�C��)
		std::unique_ptr<char[]>	reflectionMap;	//���[�h���ꂽ���t���N�V�����}�b�v(dds�t�@�C��)
		unsigned int reflectionMapSize;			//���t���N�V�����}�b�v�̃T�C�Y(dds�t�@�C��)
		std::unique_ptr<char[]>	refractionMap;	//���[�h���ꂽ���܃}�b�v(dds�t�@�C��)
		unsigned int refractionMapSize;			//���܃}�b�v�̃T�C�Y(dds�t�@�C��)
		std::string albedoMapFilePath;			//�A���x�h�}�b�v�̃t�@�C���p�X
		std::string normalMapFilePath;			//�@���}�b�v�̃t�@�C���p�X
		std::string specularMapFilePath;		//�X�y�L�����}�b�v�̃t�@�C���p�X
		std::string reflectionMapFilePath;		//���t���N�V�����}�b�v�̃t�@�C���p�X
		std::string refractionMapFilePath;		//���܃}�b�v�̃t�@�C���p�X
	};

	/// <summary>
	/// ���_�B
	/// </summary>
	/// <remarks>
	/// ������ҏW������A���C�g���̃V�F�[�_�[�Œ�`����Ă��钸�_�\���̂��ύX����K�v������
	/// </remarks>
	struct SVertex {
		Vector3 pos;			//���W�B
		Vector3 normal;			//�@���B
		Vector3 tangent;		//�ڃx�N�g���B
		Vector3 binormal;		//�]�x�N�g���B
		Vector2 uv;				//UV���W�B
		int indices[4];			//�X�L���C���f�b�N�X�B
		Vector4 skinWeights;	//�X�L���E�F�C�g�B
	};

	/// <summary>
	/// 32�r�b�g�̃C���f�b�N�X�o�b�t�@
	/// </summary>
	struct SIndexBuffer32 {
		//�C���f�b�N�X
		std::vector<uint32_t> indices;
	};

	/// <summary>
	/// 16�r�b�g�̃C���f�b�N�X�o�b�t�@
	/// </summary>
	struct SIndexbuffer16 {
		//�C���f�b�N�X
		std::vector<uint16_t> indices;
	};

	/// <summary>
	/// ���b�V���p�[�c
	/// </summary>
	struct SMesh {
		//�t���b�g�V�F�[�f�B���O�H
		bool isFlatShading;									
		//�}�e���A���̔z��B
		std::vector< SMaterial > materials;					
		//���_�o�b�t�@�B
		std::vector< SVertex >	vertexBuffer;				
		//�C���f�b�N�X�o�b�t�@(32�r�b�g)�̔z��B�}�e���A���̐��������C���f�b�N�X�o�b�t�@�͂����B
		std::vector<SIndexBuffer32> indexBuffer32Array;		
		//�C���f�b�N�X�o�b�t�@(16�r�b�g)�̔z��B
		std::vector< SIndexbuffer16> indexBuffer16Array;	
	};

public:

	TkmFile() :
		mesh_Parts_()
	{}
	~TkmFile() {};

	/// <summary>
	/// .tkm file �����[�h
	/// </summary>
	/// <param name="filePath">.tkm file path</param>
	void Load(const char* filePath);

	/// <summary>
	/// ���b�V���p�[�c�ɑ΂��ăN�G�����s��
	/// </summary>
	/// <param name="func">�N�G���֐�</param>
	void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
	{
		for (auto& mesh : this->mesh_Parts_) {
			func(mesh);
		}
	}

private:
	/// <summary>
	/// �e�N�X�`���������[�h
	/// </summary>
	/// <param name="fp">�t�@�C���|�C���^</param>
	/// <returns>�e�N�X�`����</returns>
	std::string LoadTextureFileName(FILE* fp);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�����[�h
	/// </summary>
	template<class T>
	void LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp);

	/// <summary>
	/// �}�e���A�����\�z
	/// </summary>
	/// <param name="tkmMat">tkm�}�e���A���̍\�z��</param>
	/// <param name="fp">�t�@�C���|�C���^</param>
	/// <param name="filePath">���[�h����tkm�t�@�C���̃t�@�C���p�X�B</param>
	void BuildMaterial(SMaterial& tkmMaterial, FILE* fp, const char* filePath);

	/// <summary>
	/// �ڃx�N�g���Ə]�x�N�g�����v�Z
	/// </summary>
	/// <remarks>
	/// 3dsMaxScript�ł��ׂ��Ȃ񂾂낤���ǁA�f�o�b�O�������̂ō��͂�����ł��B
	/// </remarks>
	void BuildTangentAndBiNormal();

public://get method
#pragma region Get Method
	/// <summary>
	/// ���b�V���p�[�c���擾
	/// </summary>
	/// <returns>���b�V���p�[�c�̉ϒ��z��</returns>
	const std::vector< SMesh>& GetMeshParts() const
	{
		return this->mesh_Parts_;
	}

	/// <summary>
	/// ���b�V���̐����擾
	/// </summary>
	/// <returns>���b�V���̐�</returns>
	int GetNumMesh() const
	{
		return (int)(this->mesh_Parts_.size());
	}

#pragma endregion

private:
	//���b�V���p�[�c
	std::vector<SMesh> mesh_Parts_;

};
