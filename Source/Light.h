#pragma once

#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

/// <summary>
/// �f�B���N�V�������C�g�\����
/// </summary>
struct DirectionLight
{
	Vector3 direction;  //���C�g�̕���
	float pad0;
	Vector3 color;      //���C�g�̃J���[
	float pad1;
};

/// <summary>
/// ���C�g�\����
/// </summary>
struct Light
{
	DirectionLight directionLight;  // �f�B���N�V�������C�g
	Vector3 eyePos;                 // ���_�̈ʒu
};

/*
class Light
{
private:
	struct DirectionLight
	{
		//����
		Vector3 direction;
		float pad0;
		//�F
		Vector3 color;
		float pad1;
	};

public:
	Light();
	~Light();

public:
public:
	/// <summary>
	/// ���_�̈ʒu���擾
	/// </summary>
	/// <returns>Vector3 eye_position</returns>
	Vector3& GetEyePosition() { return this->eye_Position_; }

	/// <summary>
	/// �\����DirectionLight���擾
	/// </summary>
	/// <returns>DirectionLight direction</returns>
	DirectionLight& GetDirectionLight() { return this->direction_Light_; }

//private:
	//���_�̈ʒu
	Vector3 eye_Position_;
	//�f�B���N�V�������C�g
	DirectionLight direction_Light_;
};
*/