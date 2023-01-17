#pragma once
#include <DirectXMath.h>

#include "Matrix.h"

class Math;

/// <summary>
/// クオータニオン
/// </summary>
class Quaternion : public Vector4
{
public:
	//static const Quaternion Identity;		//!<単位クォータニオン。

	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	Quaternion();
	

	/// <summary>
	/// コピー コンストラクタ。
	/// </summary>
	Quaternion(float x, float y, float z, float w);


	/// <summary>
	/// 球面線形補完
	/// </summary>
	/// <param name="t">補完率</param>
	/// <param name="q1">開始クォータニオン。</param>
	/// <param name="q2">終了クォータニオン。</param>
	void Slerp(float t, Quaternion q1, Quaternion q2);

	/// <summary>
	/// Y軸周りの回転を加算。
	/// </summary>
	/// <returns>加算する回転角度。ラジアン単位。</returns>
	void AddRotationY(float angle);

	/// <summary>
	/// クォータニオン同士の乗算
	/// </summary>
	/// <param name="rot"></param>
	void Multiply(const Quaternion& rot);

	/// <summary>
	/// クォータニオン同士の乗算。
	/// </summary>
	/// <param name="rot0"></param>
	/// <param name="rot1"></param>
	void Multiply(const Quaternion& rot0, const Quaternion& rot1);

	/// <summary>
	/// ベクトルにクォータニオンを適用する。
	/// </summary>
	void Apply(Vector4& _v) const;

	/// <summary>
	/// ベクトルにクォータニオンを適用する。
	/// </summary>
	void Apply(Vector3& _v) const;

public://operator
	/// <summary>
	/// クォータニオンの代入乗算演算子
	/// </summary>
	const Quaternion& operator*=(const Quaternion& rot0)
	{
		Multiply(rot0, *this);
		return *this;
	}

public://get method 
#pragma region get method

#pragma endregion

#pragma region get static method
	/// <summary>
	/// 単位クォータニオン
	/// </summary>
	/// <returns></returns>
	static Quaternion GetIdentity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }
#pragma endregion

public://set method
#pragma region set method
	/// <summary>
	/// X軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationX(float angle) { SetRotation(Vector3::GetVec3AxisX(), angle); }

	/// <summary>
	/// X軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegX(float angle) { SetRotationDeg(Vector3::GetVec3AxisX(), angle); }

	/// <summary>
	/// Y軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationY(float angle) { SetRotation(Vector3::GetVec3AxisY(), angle); };

	/// <summary>
	/// Y軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegY(float angle) { SetRotationDeg(Vector3::GetVec3AxisY(), angle); }

	/// <summary>
	/// Z軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationZ(float angle) { SetRotation(Vector3::GetVec3AxisZ(), angle); }

	/// <summary>
	/// Z軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegZ(float angle) { SetRotationDeg(Vector3::GetVec3AxisZ(), angle); }

	/// <summary>
	/// 任意の軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度。単位ラジアン。</param>
	void SetRotation(const Vector3& axis, float angle);

	/// <summary>
	/// 任意の軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDeg(const Vector3& axis, float angle);

	/// <summary>
	/// 行列からクォータニオンを作成。
	/// </summary>
	/// <param name="m">行列</param>
	void SetRotation(const Matrix& m)
	{
		DirectX::XMStoreFloat4(
			&vec,
			DirectX::XMQuaternionRotationMatrix(m)
		);
	}

	/// <summary>
	/// fromベクトルからtoベクトルに回転させるクォータニオンを作成。
	/// </summary>
	/// <param name="from">回転前のベクトル</param>
	/// <param name="to">回転後のベクトル</param>
	void SetRotation(Vector3 from, Vector3 to)
	{
		from.Normalize();
		to.Normalize();
		auto t = ::Dot(from, to);
		Vector3 rotAxis;
		if (t > 0.998f) {
			//ほぼ同じ向きなので単位クォータニオンにする。
			*this = Quaternion::GetIdentity();
		}
		else if (t < -0.998f) {
			//ほぼ逆向きなので、
			if (fabsf(to.x) < 1.0f) {
				//
				rotAxis = Cross(Vector3::GetVec3AxisX(), to);
			}
			else {
				rotAxis = Cross(Vector3::GetVec3AxisY(), to);
			}
		}
		else {
			rotAxis = Cross(from, to);
		}
		rotAxis.Normalize();
		SetRotation(rotAxis, acosf(t));

	}

#pragma endregion

};