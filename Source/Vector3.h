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
	/// <param name="v"></param>
	float Dot(const Vector3& v);

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
