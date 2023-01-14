#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
public:
	/// <summary>
	/// �ʒu��ݒ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetPosition(const float x, const float y, const float z) { position_.Set(x, y, z); }

	/// <summary>
	/// �^�[�Q�b�g��ݒ�	
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetTarget(const float x, const float y, const float z) { this->target_.Set(x, y, z); }

public:
	/// <summary>
	/// �ʒu���擾
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3 GetPosition()const { return position_; }

	/// <summary>
	/// �^�[�Q�b�g���擾	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3 GetTarget()const { return target_; }

private:
	//�J�����ʒu
	Vector3 position_;
	//�J�������~�_
	Vector3 target_;
};
