#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;

class Quaternion;

class Camera
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// �r���[�s��A�v���W�F�N�V�����s����X�V����B
	/// </summary>
	void Update();

	/// <summary>
	/// �����_�����_�Ƃ��ăJ��������]������B
	/// </summary>
	/// <param name="qRot">��]������N�H�[�^�j�I��</param>
	void RotateOriginTarget(const Quaternion& qRot);

public:

public:
	/// <summary>
	/// �ʒu���擾
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3& GetPosition() { return position_; }

	/// <summary>
	/// �^�[�Q�b�g���擾	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3& GetTarget() { return target_; }

private:
	//�J�����ʒu
	Vector3 position_;
	//�J�������~�_
	Vector3 target_;
};
