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
