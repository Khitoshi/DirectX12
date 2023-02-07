#pragma once
#include <string>
#include <memory>
/// <summary>
/// Null�e�N�X�`���}�b�v��ێ����Ă���N���X�B
/// </summary>
/// <remarks>
/// ���̃N���X���ێ����Ă���̂́A
/// �X�y�L�����}�b�v�A�@���}�b�v�A���˗��}�b�v�Ȃǂ�
/// �}�e���A���Ŏg�p����e�N�X�`���}�b�v�ł��B
/// ���̃N���X�̃}�b�v�̓}�e���A���ɁA��L�̃}�b�v��
/// �\��t�����Ă��Ȃ��ꍇ�ł��B
/// �ł��̂ŁA���̃N���X���ێ����Ă���}�b�v�́A�X�y�L�����}�b�v�ł����
/// ���ʔ��˗��O�A�@���}�b�v�ł���Γʉ��Ȃ��A���t���N�V�����}�b�v�ł���Δ��˗��O�ƂȂ�܂��B
/// �}�e���A���Ƀ}�b�v���\��t�����Ă��Ȃ��ꍇ�ɁA���̃N���X�̃}�b�v�𗘗p���邱�ƂŁA
/// �V�F�[�_�[�ł̓��I��������炷���Ƃ��ł��A�V�F�[�_�\�v���O�������V���v���ɂ��邱�Ƃ��ł���悤�ɂȂ��Ă��܂��B
/// ���̃N���X�Ń��[�h����Ă���e�N�X�`���́AAssets/modelData/preset�̉��ɗp�ӂ���Ă��܂��B
/// </remarks>
class NullTextureMaps
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	NullTextureMaps();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~NullTextureMaps();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

public://get method
#pragma region Get this AlbedoMap Infomation
	
	/// <summary> 
	/// �A���x�h�}�b�v���擾�B
	/// </summary>
	/// <returns></returns>
	const std::unique_ptr<char[]>& GetAlbedoMap() const
	{
		return this->albedo_Map_;
	}
	/// <summary>
	/// �A���x�h�}�b�v�̃T�C�Y���擾�B
	/// </summary>
	/// <returns></returns>
	unsigned int GetAlbedoMapSize() const
	{
		return this->albedo_Map_Size_;
	}

	/// <summary>
	/// �A���x�h�}�b�v�̃t�@�C���p�X �擾
	/// </summary>
	/// <returns></returns>
	const char* GetAlbedoMapFilePath() const
	{
		return this->albedo_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this NormalMap Infomation
	/// <summary>
	/// �@���}�b�v �擾
	/// </summary>
	/// <returns>this normal map</returns>
	const std::unique_ptr<char[]>& GetNormalMap() const
	{
		return this->normal_Map_;
	}

	/// <summary>
	/// �@���}�b�v�̃T�C�Y �擾�B
	/// </summary>
	/// <returns>this normal map size</returns>
	unsigned int GetNormalMapSize() const
	{
		return this->normal_Map_Size_;
	}

	/// <summary>
	/// �@���}�b�v�̃t�@�C���p�X �擾
	/// </summary>
	/// <returns>this normal map file path</returns>
	const char* GetNormalMapFilePath() const
	{
		return this->normal_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this SpecularMap Infomation
	/// <summary>
	/// �X�y�L�����}�b�v���擾�B
	/// </summary>
	/// <returns>this scecular map</returns>
	const std::unique_ptr<char[]>& GetSpecularMap() const
	{
		return this->scecular_Map_;
	}

	/// <summary>
	/// �X�y�L�����}�b�v�̃T�C�Y���擾�B
	/// </summary>
	/// <returns>this scecular map size</returns>
	unsigned int GetSpecularMapSize() const
	{
		return this->scecular_Map_Size_;
	}

	/// <summary>
	/// �X�y�L�����}�b�v�̃t�@�C���p�X �擾
	/// </summary>
	/// <returns>this scecular map map file path </returns>
	const char* GetSpecularMapFilePath() const
	{
		return this->scecular_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this ReflectionMap Infomation
	/// <summary>
	/// ���t���N�V�����}�b�v���擾�B
	/// </summary>
	/// <returns>this ReflectionMap </returns>
	const std::unique_ptr<char[]>& GetReflectionMap() const
	{
		return this->zero_Value_Map_;
	}

	/// <summary>
	/// ���t���N�V�����}�b�v�̃T�C�Y���擾�B
	/// </summary>
	/// <returns>this ReflectionMap size</returns>
	unsigned int GetReflectionMapSize() const
	{
		return this->zero_Value_Map_Size_;
	}

	/// <summary>
	/// ���t���N�V�����}�b�v�̃t�@�C���p�X�擾
	/// </summary>
	/// <returns>this Get ReflectionMap file path</returns>
	const char* GetReflectionMapFilePath() const
	{
		return this->zero_Value_Map_File_Path_;
	}

#pragma endregion

private:
	//�A���x�h�}�b�v
	std::unique_ptr<char[]> albedo_Map_;
	//�A���x�h�}�b�v�̃t�@�C���p�X
	const char* albedo_Map_File_Path_;
	//�A���x�h�}�b�v�̃T�C�Y
	unsigned int albedo_Map_Size_;

	//�A���x�h�}�b�v
	std::unique_ptr<char[]> normal_Map_;
	//�A���x�h�}�b�v�̃t�@�C���p�X
	const char* normal_Map_File_Path_;
	//�A���x�h�}�b�v�̃T�C�Y
	unsigned int normal_Map_Size_;

	//�X�y�L�����}�b�v
	std::unique_ptr<char[]> scecular_Map_;
	//�X�y�L�����}�b�v�̃t�@�C���p�X
	const char* scecular_Map_File_Path_;
	//�X�y�L�����}�b�v�̃T�C�Y
	unsigned int scecular_Map_Size_;

	//���t���N�V�����}�b�v
	std::unique_ptr<char[]> zero_Value_Map_;
	//���t���N�V�����}�b�v�̃t�@�C���p�X
	const char* zero_Value_Map_File_Path_;
	//���t���N�V�����}�b�v�̃T�C�Y
	unsigned int zero_Value_Map_Size_;
};
