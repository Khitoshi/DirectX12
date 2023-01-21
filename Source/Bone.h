#pragma once

#include "Quaternion.h"
#include "Matrix.h"
#include <string>
#include <list>
/// <summary>
/// �{�[���B
/// </summary>
class Bone {
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	/// <param name="boneName">�{�[���̖��O</param>
	/// <param name="bindPose">�o�C���h�|�[�Y�̍s��</param>
	/// <param name="invBindPose">�o�C���h�|�[�Y�̋t�s��</param>
	/// <param name="parentBoneNo">�e�̃{�[���ԍ�</param>
	/// <param name="boneId">�{�[���ԍ�</param>
	Bone(
		const wchar_t* boneName,
		const Matrix& bindPose,
		const Matrix& invBindPose,
		int parentBoneNo,
		int boneId
	);

	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~Bone();

	/// <summary>
	/// �q�� �ǉ�
	/// </summary>
	/// <param name="bone"></param>
	void AddChild(Bone* bone) { this->bone_children_.push_back(bone); }
	
	/// <summary>
	/// ���̃{�[���̃��[���h��Ԃł̈ʒu�Ɖ�]�ƃX�P�[�����v�Z����
	/// </summary>
	/// <param name="trans">���s�ړ��ʂ̊i�[��</param>
	/// <param name="rot">��]�ʂ̊i�[��</param>
	/// <param name="scale">�g�嗦�̊i�[��</param>
	void CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale);

public:
#pragma region get method
	/// <summary>
	/// ���[�J���s��(�e�̍��W�n�ł̍s��)���擾�B
	/// </summary>
	const Matrix& GetLocalMatrix() const{return this->local_Matrix_;}

	/// <summary>
	/// ���[���h�s����擾�B
	/// </summary>
	const Matrix& GetWorldMatrix() const{return this->world_Matrix_;}

	/// <summary>
	/// �o�C���h�|�[�Y�̍s����擾�B
	/// </summary>
	const Matrix& GetBindPoseMatrix() const{return this->bind_Pose_;}

	/// <summary>
	/// �o�C���h�|�[�Y�̋t�s����擾�B
	/// </summary>
	const Matrix& GetInvBindPoseMatrix() const{return this->inv_Bind_Pose_;}

	/// <summary>
	/// �e�̃{�[���ԍ����擾�B
	/// </summary>
	/// <returns></returns>
	int GetParentBoneNumber() const{return this->parent_Bone_Number_;}

	/// <summary>
	/// �{�[���ԍ����擾�B
	/// </summary>
	/// <returns></returns>
	int GetId() const{return this->bone_Id_;}

	/// <summary>
	/// �q���擾
	/// </summary>
	/// <returns>this bone children</returns>
	std::list<Bone*>& GetChildren(){return this->bone_children_;}
	const Matrix& GetOffsetLocalMatrix() const{return this->offset_Local_Matrix_;}

	//�{�[���̖��O �擾
	const wchar_t* GetName() const { return this->bone_Name_.c_str(); }

#pragma endregion

public:
#pragma region set method
	/// <summary>
	/// ���[�J���s��(�e�̍��W�n�ł̍s��)��ݒ�B
	/// </summary>
	void SetLocalMatrix(const Matrix& m){this->local_Matrix_ = m;}

	/// <summary>
	/// ���[���h�s���ݒ�B
	/// </summary>
	void SetWorldMatrix(const Matrix& m){this->world_Matrix_ = m;}

#pragma endregion

private:
	//�{�[���̖��O
	std::wstring	bone_Name_;
	//�e�̃{�[���ԍ�
	int				parent_Bone_Number_;
	//�{�[���ԍ�
	int				bone_Id_;

	//�o�C���h�|�[�Y
	Matrix			bind_Pose_;
	//�o�C���h�|�[�Y�̋t�s��
	Matrix			inv_Bind_Pose_;
	//���[�J���s��
	Matrix			local_Matrix_;
	//���[���h�s��
	Matrix			world_Matrix_;
	//���̃{�[���̃��[���h��Ԃł̈ʒu�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���
	Matrix			offset_Local_Matrix_;

	//�ʒu
	Vector3			positoin_;
	//���̃{�[���̊g�嗦�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���
	Vector3			scale_;
	//���̃{�[���̉�]�B�Ō��CalcWorldTRS�����s�����Ƃ��̌��ʂ��i�[����Ă���
	Quaternion		rotation_;

	//�q���̃��X�g�B
	std::list<Bone*>	bone_children_;
};