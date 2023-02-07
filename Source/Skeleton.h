#pragma once

#include <memory>
#include <vector>

#include "Skeleton.h"
#include "TksFile.h"
#include "Bone.h"
#include "Matrix.h"

class Skeleton
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	Skeleton();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~Skeleton();

	/// <summary>
	/// 
	/// </s������ummary>
	/// <param name="tksFilePath">.tks file path</param>
	void Init(const char* tksFilePath);

	/// <summary>
	/// �{�[���s��̍\�z�B
	/// </summary>
	void BuildBoneMatrices();

	/// <summary>
	/// �{�[���̖��O����ԍ�������
	/// </summary>
	/// <param name="boneName">bone name</param>
	/// <returns>�{�[���̔ԍ��C������Ȃ������ꍇ��-1</returns>
	int FindBoneID(const wchar_t* boneName)const;

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="matrix_World"></param>
	void Update(const Matrix& matrix_World);

	/// <summary>
	/// �{�[���̃��[���h�s��̍X�V�֐�
	/// </summary>
	/// <param name="bone">�X�V����{�[��</param>
	/// <param name="parentMatrix">�e�{�[���̃��[���h�s��</param>
	static 	void UpdateBoneWorldMatrix(Bone& bone, const Matrix& parentMatrix);

private:
	/// <summary>
	/// �A�j���[�V�����X�V����
	/// </summary>
	/// <param name="matrix_world">�}�g���b�N�X���[���h�s��</param>
	void AnimationUpdate(const Matrix& matrix_world);

public:
#pragma region Set method
	/// <summary>
	/// �{�[�������[�J���s��ɐݒ�
	/// </summary>
	/// <param name="boneNo">�{�[���ԍ�</param>
	/// <param name="matrix">�s��</param>
	//void SetBoneLocalMatrix(int boneNumber, const Matrix& matrix);
	
	/// <summary>
	/// �A�j���[�V�������Đ�����Ă���t���O��t����
	/// </summary>
	void SetMarkPlayAnimation() { this->is_Play_Animation_ = true; }

#pragma endregion

#pragma region Get method
	/// <summary>
	/// �{�[���̐����擾
	/// </summary>
	/// <returns>this bone number </returns>
	int GetNumBones()const { return static_cast<int>(this->bones_.size()); }

	/// <summary>
	/// �������ς� ���� �擾
	/// </summary>
	/// <returns></returns>
	bool IsInited()const { this->is_Inited_; };

	/// <summary>
	/// �{�[�����擾
	/// </summary>
	/// <param name="boneNumber">�{�[���̔ԍ�</param>
	/// <returns></returns>
	Bone* GetBone(const int boneNumber)const { return this->bones_[boneNumber].get(); }

	/// <summary>
	/// �{�[���s��̐擪�A�h���X�擾
	/// </summary>
	/// <returns></returns>
	Matrix* GetBoneMatricesTopAddress()const { return this->bone_Matrixs_.get(); };

#pragma endregion

private:
	//�{�[���̍ő吔
	enum { BONE_MAX = 512 };
private:
	//.tks�t�@�C��
	TksFile tks_file_;
	
	//�{�[���̔z��
	std::vector<std::unique_ptr<Bone>> bones_;

	//�{�[���s��
	std::unique_ptr<Matrix[]> bone_Matrixs_;

	//���������f�p�t���O
	//false = ���������Ă��Ȃ�
	bool is_Inited_;

	//�A�j���[�V�����������ݔ��f�p
	//false = �A�j���[�V�������������܂�Ă��Ȃ�
	bool is_Play_Animation_;
};
