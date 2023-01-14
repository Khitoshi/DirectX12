#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Vector3
{
private:
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// �S��x,y,z�̗v�f��0�ŏ���������܂�
	/// </summary>
	explicit Vector3();

	/// <summary>
	/// �R�s�[ �R���X�g���N�^
	/// �w��̒l�ŏ���������܂�.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	explicit Vector3(const float x, const	float y, const float z);

	/// <summary>
	/// �R�s�[ �R���X�g���N�^
	/// �w��̒l�ŏ���������܂�
	/// </summary>
	/// <param name="f"></param>
	explicit Vector3(const XMFLOAT3 f);

	/// <summary>
	/// ���^���
	/// </summary>
	/// <param name="t">��ԗ�</param>
	/// <param name="v0">��ԊJ�n�x�N�g��</param>
	/// <param name="v1">��ԏI���x�N�g��</param>
	void Lerp(const float t, const Vector3& v0, const Vector3& v1);

	/// <summary>
	/// �x�N�g���ɐݒ�
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void Set(const float x, const float y, const float z);
	
	/// <summary>
	/// �x�N�g���ɐݒ�
	/// </summary>
	/// <param name="v"></param>
	void Set(const Vector3 v);

	/// <summary>
	/// �x�N�g���̉��Z
	/// </summary>
	/// <remarks>
	/// this += v;
	/// </remarks>
	/// <param name="vec">���Z�����x�N�g��</param>
	void Add(const Vector3& v);

	/// <summary>
	/// �x�N�g���̉��Z
	/// </summary>
	/// <remarks>
	/// this = v0 + v1;
	/// </remarks>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	void Add(const Vector3& v0, const Vector3 v1);

	/// <summary>
	/// �x�N�g���̌��Z
	/// </summary>
	/// <remarks>
	/// this -= v;
	/// </remarks>
	/// <param name="v">���Z����x�N�g��</param>
	void Subtract(const Vector3& v);

	/// <summary>
	/// �x�N�g���̌��Z
	/// </summary>
	/// <remarks>
	/// this = v0 - v1;
	/// </remarks>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	void Subtract(const Vector3& v0, const Vector3& v1);

	/// <summary>
	/// ���ς��v�Z
	/// </summary>
	/// <remarks>
	/// float d = this->x * v.x + this->y * v.y + this->z * v.z;
	/// return d;
	/// <param name="v"></param>
	float Dot(const Vector3& v);

	/// <summary>
	/// �O�ς��v�Z
	/// </summary>
	/// <remarks>
	/// Vector3 v;
	/// v.x = this->y * _v.z - this->z * _v.y;
	/// v.y = thiz->z * _v.x - this->x * _v.z;
	/// v.z = this->x * _v.y - this->y * _v.x;
	/// this = v;	
	/// </remarks>
	/// <param name="v"></param>
	void Cross(const Vector3& v);

	/// <summary>
	/// �O�ς̌v�Z
	/// </summary>
	/// <remarks>
	/// Vector3 v;
	/// v.x = v0->y * v1.z - v0z * v1.y;
	/// v.y = v0->z * v1.x - v0x * v1.z;
	/// v.z = v0->x * v1.y - v0y * v1.x;
	/// this = v;	
	/// </remarks>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	void Cross(const Vector3& v0, const Vector3& v1);

	/// <summary>
	/// �x�N�g���̒������擾
	/// </summary>
	/// <returns>
	/// this Length
	/// </returns>
	float Length()const;

	/// <summary>
	/// �x�N�g���̒����̓����擾
	/// </summary>
	/// <returns>
	/// this Length**
	/// </returns>
	float  LengthSq()const;

	/// <summary>
	/// �x�N�g�����X�J���[�Ŋg��
	/// </summary>
	/// <remarks>
	/// this->x *= s
	/// this->y *= s
	/// this->z *= s
	/// </remarks>
	/// <param name="s">�X�J���[�l</param>
	void Scale(const float s);

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	void Normalize();

	/// <summary>
	/// �x�N�g�����X�J���[�ŏ��Z
	/// </summary>
	/// <remarks>
	/// this->x / = d
	/// this->y / = d
	/// this->z / = d
	/// </remarks>
	/// <param name="d"></param>
	void Div(const float d);

	/// <summary>
	/// �x�N�g���ɍő�l��ݒ�
	/// </summary>
	/// <remarks>
	/// this->x = max(this->x,v.x);
	/// this->y = max(this->y,v.y);
	/// this->z = max(this->z,v.z);
	/// </remarks>
	/// <param name="v"></param>
	void Max(const Vector3 v);

	/// <summary>
	/// �x�N�g���ɍŏ��l��ݒ�
	/// </summary>
	/// <remarks>
	/// this->x = min(this->x,v.x)
	/// this->y = min(this->y,v.y)
	/// this->z = min(this->z,v.z)
	/// </remarks>
	/// <param name="v"></param>
	void Min(const Vector3& v);



public:

	operator XMVECTOR()const
	{
		return XMLoadFloat3(&vec);
	}

	/// <summary>
	/// ������Z�q
	/// </summary>
	/// <remarks>
	/// this = v;
	/// </remarks>
	/// <param name="v"></param>
	Vector3& operator=(const Vector3& v)
	{
		this->vec = v.vec;
		return*this;
	}

	/// <summary>
	/// ���Z������Z�q
	/// </summary>
	/// <remarks>
	/// this.x += v.x;
	/// this.y += v.y;
	/// this.z += v.z;
	/// </remarks>
	/// <param name="v"></param>
	const Vector3& operator+=(const Vector3& v)
	{
		this->Add(v);
		return *this;
	}

	/// <summary>
	/// ���Z������Z�q
	/// </summary>
	/// <remarks>
	/// this->x -= v.x
	/// this->y -= v.y
	/// this->z -= v.z
	/// </remarks>
	/// <param name="v"></param>
	const Vector3& operator-=(const Vector3& v)
	{
		this->Subtract(v);
		return *this;
	}

	/// <summary>
	/// ��Z������Z�q
	/// </summary>
	/// <remarks>
	/// this->x *= s;
	/// this->y *= s;
	/// this->z *= s;
	/// </remarks>
	/// <param name="s"></param>
	const Vector3& operator*=(const float s)
	{
		this->Scale(s);
		return *this;
	}

	/// <summary>
	/// ���Z���Z�q
	/// </summary>
	/// <remarks>
	/// this->x /= s;
	/// this->y /= s;
	/// this->z /= s;
	/// </remarks>
	/// <param name="s"></param>
	const Vector3& operator/=(const float s)
	{
		this->Div(s);
		return *this;
	}

public:
	union
	{
		XMFLOAT3 vec;
		float v[3];
		struct
		{
			float x, y, z;
		};
	};
};
