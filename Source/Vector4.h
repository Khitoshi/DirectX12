#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
/// <summary>
/// 4�����x�N�g���N���X
/// </summary>
class Vector4
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Vector4(float x, float y, float z, float w)
	{
		Set(x, y, z, w);
	}

	/// <summary>
	/// 3�����̃x�N�g���f�[�^�������Ŏ󂯎��R���X�g���N�^
	/// </summary>
	/// <remarks>
	/// w�ɂ�1.0���i�[����܂��B
	/// </remarks>
	Vector4(const Vector3& v)
	{
		Set(v);
	}

	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	/// <remarks>
	/// x,y,z��0.0f�Aw��1.0f�ŏ���������܂��B
	/// </remarks>
	explicit Vector4()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	/// <summary>
	/// �x�N�g���̃R�s�[�B
	/// </summary>
	template<class T>
	void CopyTo(T& dst) const
	{
		dst.x = x;
		dst.y = y;
		dst.z = z;
		dst.w = w;
	}

public:
	/// <summary>
	/// XMVECTOR�ւ̈Öق̌^�ϊ��B
	/// </summary>
	/// <returns></returns>
	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&vec);
	}

	/// <summary>
	/// ������Z�q�B
	/// </summary>
	/// <remarks>
	/// �x�N�g���N���X�ɉ��L�̂悤�ȑ�����Z�̋@�\��񋟂��܂��B
	/// Vector4 v0 = {10.0f, 20.0f, 30.0f, 10.0f};
	/// Vector4 v1;
	/// v1 = v0;
	/// </remarks>
	Vector4& operator=(const Vector4& _v)
	{
		vec = _v.vec;
		return *this;
	}

	/// <summary>
	/// �x�N�g���̊e�v�f��ݒ�B
	/// </summary>
	void Set(float _x, float _y, float _z, float _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	/// <summary>
	/// �x�N�g���𐳋K���B
	/// </summary>
	void Normalize()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		xmv = DirectX::XMVector4Normalize(xmv);
		DirectX::XMStoreFloat4(&vec, xmv);
	}

	/// <summary>
	/// �x�N�g����ݒ�B
	/// </summary>
	/// <param name="_v"></param>
	void Set(const Vector4& _v)
	{
		*this = _v;
	}

	/// <summary>
	/// 3�����x�N�g���f�[�^�����ƂɁA�x�N�g����ݒ�B
	/// </summary>
	/// <remarks>
	/// w�ɂ�1.0���ݒ肳��܂��B
	/// </remarks>
	void Set(const Vector3& _v)
	{
		this->x = _v.x;
		this->y = _v.y;
		this->z = _v.z;
		this->w = 1.0f;
	}

	/// <summary>
	/// �x�N�g�������Z�B
	/// </summary>
	/// <remarks>
	/// this += _v;
	/// </remarks>
	void Add(const Vector4& _v)
	{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}

	/// <summary>
	/// �x�N�g���̉��Z�B
	/// </summary>
	/// <remarks>
	/// this = v0 + v1;
	/// </remarks>
	void Add(const Vector4& v0, const Vector4& v1)
	{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorAdd(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}

	/// <summary>
	/// �x�N�g���̌��Z�B
	/// </summary>
	/// <remarks>
	/// this -= _v;
	/// </remarks>
	void Subtract(const Vector4& _v)
	{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}

	/// <summary>
	/// �x�N�g���̌��Z�B
	/// </summary>
	/// <remarks>
	/// this = v0 - v1;
	/// </remarks>
	void Subtract(const Vector4& v0, const Vector4& v1)
	{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&v0.vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&v1.vec);
		DirectX::XMVECTOR xmvr = DirectX::XMVectorSubtract(xmv0, xmv1);
		DirectX::XMStoreFloat4(&vec, xmvr);
	}

	/// <summary>
	/// ���ς��v�Z�B
	/// </summary>
	/// <remarks>
	/// float d = this->x * _v.x + this->y * _v.y + this->z * _v.z + this->w * _v.w;
	/// return d;
	/// </remarks>
	float Dot(const Vector4& _v) const
	{
		DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat4(&vec);
		DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat4(&_v.vec);
		return DirectX::XMVector4Dot(xmv0, xmv1).m128_f32[0];
	}

	/// <summary>
	/// �x�N�g���̒������擾
	/// </summary>
	float Length()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4Length(xmv).m128_f32[0];
	}

	/// <summary>
	/// �x�N�g���̒����̓����擾�B
	/// </summary>
	float LengthSq()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4LengthSq(xmv).m128_f32[0];
	}

	/// <summary>
	/// �x�N�g�����X�J���[�Ŋg��B
	/// </summary>
	/// <remarks>
	/// ���L�̂悤�ȏ������s���Ă��܂��B
	/// this->x *= s;
	/// this->y *= s;
	/// this->z *= s;
	/// this->w *= s;
	/// </remarks>
	void Scale(float s)
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		xmv = DirectX::XMVectorScale(xmv, s);
		DirectX::XMStoreFloat4(&vec, xmv);
	}

public:
#pragma region Get Colors
	/// <summary>
	/// ���F �擾
	/// </summary>
	/// <returns>Get color is White</returns>
	static const Vector4 GetWhite() {};

	/// <summary>
	/// ���F �擾
	/// </summary>
	/// <returns>Get color is Black</returns>
	static const Vector4 GetBlack() {};

	/// <summary>
	/// ���F �擾
	/// </summary>
	/// <returns>Get color is Yellow</returns>
	static const Vector4 GetYellow() {};
	
	/// <summary>
	/// �D�F �擾
	/// </summary>
	/// <returns>Get color is Gray</returns>
	static const Vector4 GetGray() {};

#pragma endregion

public:
	union {
		DirectX::XMFLOAT4 vec;
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float v[4];
	};
};


/// <summary>
/// �x�N�g�����m�̌��Z�B
/// </summary>
static inline Vector4 operator-(const Vector4& v0, const Vector4& v1)
{
	Vector4 result;
	result.Subtract(v0, v1);
	return result;
}


/// <summary>
/// �x�N�g�����m�̉��Z�B
/// </summary>
static inline Vector4 operator+(const Vector4& v0, const Vector4& v1)
{
	Vector4 result;
	result.Add(v0, v1);
	return result;
}


/// <summary>
/// �x�N�g���ƃX�J���[�̏�Z�B
/// </summary>
static inline Vector4 operator*(const Vector4& v, float s)
{
	Vector4 result;
	result = v;
	result.Scale(s);
	return result;
}


/// <summary>
/// ���ς��v�Z�B
/// </summary>
static inline float Dot(const Vector4& v0, const Vector4& v1)
{
	return v0.Dot(v1);
}