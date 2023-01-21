#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Vector3
{
private:
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// 全てx,y,zの要素が0で初期化されます
	/// </summary>
	explicit Vector3();

	/// <summary>
	/// コピー コンストラクタ
	/// 指定の値で初期化されます.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	explicit Vector3(const float x, const	float y, const float z);

	/// <summary>
	/// コピー コンストラクタ
	/// 指定の値で初期化されます
	/// </summary>
	/// <param name="f"></param>
	explicit Vector3(const XMFLOAT3 f);

	/// <summary>
	/// 線型補間
	/// </summary>
	/// <param name="t">補間率</param>
	/// <param name="v0">補間開始ベクトル</param>
	/// <param name="v1">補間終了ベクトル</param>
	void Lerp(const float t, const Vector3& v0, const Vector3& v1);

	/// <summary>
	/// ベクトルに設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void Set(const float x, const float y, const float z);
	
	/// <summary>
	/// ベクトルに設定
	/// </summary>
	/// <param name="v"></param>
	void Set(const Vector3 v);

	/// <summary>
	/// ベクトルに設定
	/// </summary>
	/// <typeparam name="TVector"></typeparam>
	/// <param name="_v"></param>
	template<class T>
	void Set(T& _v) { Set(_v.x, _v.y, _v.z); }

	/// <summary>
	/// ベクトルの加算
	/// </summary>
	/// <remarks>
	/// this += v;
	/// </remarks>
	/// <param name="vec">加算されるベクトル</param>
	void Add(const Vector3& v);

	/// <summary>
	/// ベクトルの加算
	/// </summary>
	/// <remarks>
	/// this = v0 + v1;
	/// </remarks>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	void Add(const Vector3& v0, const Vector3 v1);

	/// <summary>
	/// ベクトルの減算
	/// </summary>
	/// <remarks>
	/// this -= v;
	/// </remarks>
	/// <param name="v">減算するベクトル</param>
	void Subtract(const Vector3& v);

	/// <summary>
	/// ベクトルの減算
	/// </summary>
	/// <remarks>
	/// this = v0 - v1;
	/// </remarks>
	/// <param name="v0"></param>
	/// <param name="v1"></param>
	void Subtract(const Vector3& v0, const Vector3& v1);

	/// <summary>
	/// 内積を計算
	/// </summary>
	/// <remarks>
	/// float d = this->x * v.x + this->y * v.y + this->z * v.z;
	/// return d;
	/// </remarks>
	/// <param name="v"></param>
	float Dot(const Vector3& v)const;

	/// <summary>
	/// 外積を計算
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
	/// 外積の計算
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
	/// ベクトルの長さを取得
	/// </summary>
	/// <returns>
	/// this Length
	/// </returns>
	float Length()const;

	/// <summary>
	/// ベクトルの長さの二乗を取得
	/// </summary>
	/// <returns>
	/// this Length**
	/// </returns>
	float  LengthSq()const;

	/// <summary>
	/// ベクトルをスカラーで拡大
	/// </summary>
	/// <remarks>
	/// this->x *= s
	/// this->y *= s
	/// this->z *= s
	/// </remarks>
	/// <param name="s">スカラー値</param>
	void Scale(const float s);

	/// <summary>
	/// ベクトルの正規化
	/// </summary>
	void Normalize();

	/// <summary>
	/// ベクトルをスカラーで除算
	/// </summary>
	/// <remarks>
	/// this->x / = d
	/// this->y / = d
	/// this->z / = d
	/// </remarks>
	/// <param name="d"></param>
	void Div(const float d);

	/// <summary>
	/// ベクトルに最大値を設定
	/// </summary>
	/// <remarks>
	/// this->x = max(this->x,v.x);
	/// this->y = max(this->y,v.y);
	/// this->z = max(this->z,v.z);
	/// </remarks>
	/// <param name="v"></param>
	void Max(const Vector3 v);

	/// <summary>
	/// ベクトルに最小値を設定
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

#pragma region Operator
	/// <summary>
	/// 代入演算子
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
	/// 加算代入演算子
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
	/// 減算代入演算子
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
	/// 乗算代入演算子
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
	/// 除算演算子
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

#pragma endregion

	

public:
#pragma region Get static method
	//const Vector2 g_vec2Zero = { 0.0f, 0.0f };
	const static Vector3 GetVec3Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
	const static Vector3 GetVec3Right() { return Vector3(1.0f, 0.0f, 0.0f); }
	const static Vector3 GetVec3Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
	const static Vector3 GetVec3Up() { return Vector3(0.0f, 1.0f, 0.0f); }
	const static Vector3 GetVec3Down() { return Vector3(0.0f, -1.0f, 0.0f); }
	const static Vector3 GetVec3Front() { return Vector3(0.0f, 0.0f, 1.0f); }
	const static Vector3 GetVec3Back() { return Vector3(0.0f, 0.0f, -1.0f); }
	const static Vector3 GetVec3AxisX() { return Vector3(1.0f, 0.0f, 0.0f); }
	const static Vector3 GetVec3AxisY() { return Vector3(0.0f, 1.0f, 0.0f); }
	const static Vector3 GetVec3AxisZ() { return Vector3(0.0f, 0.0f, 1.0f); }
	const static Vector3 GetVec3One() { return Vector3(1.0f, 1.0f, 1.0f); }

#pragma endregion


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

/// <summary>
/// ベクトル同士の加算。
/// </summary>
static inline Vector3 operator+(const Vector3& v0, const Vector3& v1)
{
	Vector3 result;
	result.Add(v0, v1);
	return result;
}
/// <summary>
/// ベクトルとスカラーの乗算。
/// </summary>
static inline Vector3 operator*(const Vector3& v, float s)
{
	Vector3 result;
	result = v;
	result.Scale(s);
	return result;
}

/// <summary>
/// ベクトルとスカラーの除算。
/// </summary>
static inline Vector3 operator/(const Vector3& v, float s)
{
	Vector3 result;
	result = v;
	result.Div(s);
	return result;
}

/// <summary>
/// ベクトル同士の減算。
/// </summary>
static inline Vector3 operator-(const Vector3& v0, const Vector3& v1)
{
	Vector3 result;
	result.Subtract(v0, v1);
	return result;
}

/// <summary>
/// 外積を計算。
/// </summary>
static inline Vector3 Cross(const Vector3& v0, const Vector3& v1)
{
	Vector3 result;
	result.Cross(v0, v1);
	return result;
}


/// <summary>
/// 内積を計算。
/// </summary>
static inline float Dot(const Vector3& v0, const Vector3& v1)
{
	return v0.Dot(v1);
}