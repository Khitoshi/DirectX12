#pragma once

#include "Quaternion.h"

class Math
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	Math();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~Math();

	/// <summary>
	/// 角度をDegree単位からRadian単位に変換する。
	/// </summary>
	/// <param name="deg">Degree単位の角度。</param>
	/// <returns>Radian単位の角度。</returns>
	constexpr static inline float DegToRad(float deg){return deg * (PI() / 180.0f);}
	
	/// <summary>
	/// 角度をRadian単位からDegree単位に変換する。
	/// </summary>
	/// <param name="rad">Radian単位の角度。</param>
	/// <returns>Degree単位の角度。</returns>
	constexpr static inline float RadToDeg(float rad){return rad / (PI() / 180.0f);}

	/// <summary>
	/// t0とt1の間を線形補完。
	/// </summary>
	/// <param name="rate">補間率は0.0～1.0</param>
	/// <param name="t0">補間開始の値。</param>
	/// <param name="t1">補間終了の値。</param>
	/// <returns>補間された値。</returns>
	template<class T> static inline T Lerp(float rate, T t0, T t1)
	{
		T ret;
		ret.Lerp(rate, t0, t1);
		return ret;
	}

	/// <summary>
	/// t0とt1の間を線形補完。
	/// </summary>
	/// <param name="rate">補間率は0.0～1.0</param>
	/// <param name="t0">補間開始の値。</param>
	/// <param name="t1">補間終了の値。</param>
	/// <returns>補間された値。</returns>
	template<> static inline float Lerp(float rate, float t0, float t1)
	{
		return t0 + (t1 - t0) * rate;
	}

public:
	//円周率
	const static constexpr float PI() { return 3.14159265358979323846f; }

	//円周率の２乗
	const static constexpr float PI2() { return PI() * 2.0f; }
};


//整数型のベクトルクラス。
__declspec(align(16)) class Vector4i {
public:
	union {
		struct { int x, y, z, w; };
		int v[4];
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
/// ベクトル同士の減算。
/// </summary>
static inline Vector4 operator-(const Vector4& v0, const Vector4& v1)
{
	Vector4 result;
	result.Subtract(v0, v1);
	return result;
}


/// <summary>
/// 内積を計算。
/// </summary>
static inline float Dot(const Vector4& v0, const Vector4& v1)
{
	return v0.Dot(v1);
}


/// <summary>
/// クォータニオン同士の乗算。
/// </summary>
static inline Quaternion operator*(const Quaternion& q1, const Quaternion q2)
{
	Quaternion qRet;
	qRet.Multiply(q2, q1);
	return qRet;
}