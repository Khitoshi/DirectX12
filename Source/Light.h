#pragma once

#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Light
{
private:
	struct DirectionLight
	{
		//����
		Vector3 direction;
		//�F
		Vector3 color;
	};

public:
	Light();
	~Light();

public:
	/// <summary>
	/// ���_�̈ʒu��ݒ�	
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetEyePosition(const float x, const float y, const float z) { this->eye_Position_.Set(x, y, z); }

	/// <summary>
	/// �\����DirectionLight��ݒ�
	/// </summary>
	/// <param name="direction"></param>
	void SetDirectionLight(const DirectionLight direction) { this->direction_Light_ = direction; }

	/// <summary>
	/// �\����DirectionLight�̕�����ݒ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetDirectionLightDirection(const float x, const float y, const float z) { this->direction_Light_.direction.Set(x, y, z); }
	
	/// <summary>
	/// �\����DirectionLight�̐F��ݒ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetDirectionLightColor(const float x, const float y, const float z) { this->direction_Light_.color.Set(x, y, z); }

public:
	/// <summary>
	/// ���_�̈ʒu���擾
	/// </summary>
	/// <returns>Vector3 eye_position</returns>
	const Vector3 GetEyePosition()const { return this->eye_Position_; }

	/// <summary>
	/// �\����DirectionLight���擾
	/// </summary>
	/// <returns>DirectionLight direction</returns>
	const DirectionLight GetDirectionLight()const { return this->direction_Light_; }

	/// <summary>
	/// �\����DirectionLight�̕������擾
	/// </summary>
	/// <returns>Direction Direction.direction </returns>
	const Vector3 GetDirectionLightDirection()const { return this->direction_Light_.direction; }

	/// <summary>
	/// �\����DirectionLight�̐F���擾
	/// </summary>
	/// <returns>Vector3 Direction.color</returns>
	const Vector3 GetDirectionLightColor()const { return this->direction_Light_.direction; }

//private:
	//���_�̈ʒu
	Vector3 eye_Position_;
	//�f�B���N�V�������C�g
	DirectionLight direction_Light_;
};