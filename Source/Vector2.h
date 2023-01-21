#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class Vector2
{
public:
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	Vector2(float x, float y):
		x(x),
		y(y)
	{
	}
	explicit Vector2():
		x(0),
		y(0)
	{
	}

	/// <summary>
	/// ���`���
	/// </summary>
	/// <param name="f">��ԗ�</param>
	/// <param name="v0">��ԊJ�n�x�N�g��</param>
	/// <param name="v1">��ԃx�N�g��</param>
	void Lerp(const float f, const Vector2& v0, const Vector2& v1)
	{
		this->x = v0.x + (v1.x - v0.x) * f;
		this->y = v0.y + (v1.y - v0.y) * f;
	}

	/// <summary>
	/// ���K��
	/// </summary>
	void Normalize()
	{
		XMVECTOR xmv = XMLoadFloat2(&vec);
		xmv = XMVector2Normalize(xmv);
		XMStoreFloat2(&this->vec, xmv);
	}

public:
#pragma region Set Method
	void Set(const float x, const float y)
	{
		this->x = x;
		this->y = y;
	}
	
#pragma endregion


public:
	//������Z�q
	Vector2& operator=(const Vector2& v)
	{
		this->vec = v.vec;
		return *this;
	}

public:
	union {
		DirectX::XMFLOAT2 vec;
		struct { float x, y; };
		float v[2];
	};
};

