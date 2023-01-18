#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
/// <summary>
/// 4次元ベクトルクラス
/// </summary>
class Vector4
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Vector4(float x, float y, float z, float w)
	{
		Set(x, y, z, w);
	}

	/// <summary>
	/// 3次元のベクトルデータを引数で受け取るコンストラクタ
	/// </summary>
	/// <remarks>
	/// wには1.0が格納されます。
	/// </remarks>
	Vector4(const Vector3& v)
	{
		Set(v);
	}

	/// <summary>
	/// コンストラクタ。
	/// </summary>
	/// <remarks>
	/// x,y,zが0.0f、wは1.0fで初期化されます。
	/// </remarks>
	explicit Vector4()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	/// <summary>
	/// ベクトルのコピー。
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
	/// XMVECTORへの暗黙の型変換。
	/// </summary>
	/// <returns></returns>
	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&vec);
	}

	/// <summary>
	/// 代入演算子。
	/// </summary>
	/// <remarks>
	/// ベクトルクラスに下記のような代入演算の機能を提供します。
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
	/// ベクトルの各要素を設定。
	/// </summary>
	void Set(float _x, float _y, float _z, float _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	/// <summary>
	/// ベクトルを正規化。
	/// </summary>
	void Normalize()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		xmv = DirectX::XMVector4Normalize(xmv);
		DirectX::XMStoreFloat4(&vec, xmv);
	}

	/// <summary>
	/// ベクトルを設定。
	/// </summary>
	/// <param name="_v"></param>
	void Set(const Vector4& _v)
	{
		*this = _v;
	}

	/// <summary>
	/// 3次元ベクトルデータをもとに、ベクトルを設定。
	/// </summary>
	/// <remarks>
	/// wには1.0が設定されます。
	/// </remarks>
	void Set(const Vector3& _v)
	{
		this->x = _v.x;
		this->y = _v.y;
		this->z = _v.z;
		this->w = 1.0f;
	}

	/// <summary>
	/// ベクトルを加算。
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
	/// ベクトルの加算。
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
	/// ベクトルの減算。
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
	/// ベクトルの減算。
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
	/// 内積を計算。
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
	/// ベクトルの長さを取得
	/// </summary>
	float Length()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4Length(xmv).m128_f32[0];
	}

	/// <summary>
	/// ベクトルの長さの二乗を取得。
	/// </summary>
	float LengthSq()
	{
		DirectX::XMVECTOR xmv = DirectX::XMLoadFloat4(&vec);
		return DirectX::XMVector4LengthSq(xmv).m128_f32[0];
	}

	/// <summary>
	/// ベクトルをスカラーで拡大。
	/// </summary>
	/// <remarks>
	/// 下記のような処理が行われています。
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
	/// 白色 取得
	/// </summary>
	/// <returns>Get color is White</returns>
	static const Vector4 GetWhite() {};

	/// <summary>
	/// 黒色 取得
	/// </summary>
	/// <returns>Get color is Black</returns>
	static const Vector4 GetBlack() {};

	/// <summary>
	/// 黄色 取得
	/// </summary>
	/// <returns>Get color is Yellow</returns>
	static const Vector4 GetYellow() {};
	
	/// <summary>
	/// 灰色 取得
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
/// ベクトル同士の減算。
/// </summary>
static inline Vector4 operator-(const Vector4& v0, const Vector4& v1)
{
	Vector4 result;
	result.Subtract(v0, v1);
	return result;
}


/// <summary>
/// ベクトル同士の加算。
/// </summary>
static inline Vector4 operator+(const Vector4& v0, const Vector4& v1)
{
	Vector4 result;
	result.Add(v0, v1);
	return result;
}


/// <summary>
/// ベクトルとスカラーの乗算。
/// </summary>
static inline Vector4 operator*(const Vector4& v, float s)
{
	Vector4 result;
	result = v;
	result.Scale(s);
	return result;
}


/// <summary>
/// 内積を計算。
/// </summary>
static inline float Dot(const Vector4& v0, const Vector4& v1)
{
	return v0.Dot(v1);
}