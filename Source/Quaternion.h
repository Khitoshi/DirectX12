#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Math.h"
/// <summary>
/// クオータニオン
/// </summary>
class Quaternion : public Vector4
{
public:
	//static const Quaternion Identity;		//!<単位クォータニオン。
	Quaternion()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	Quaternion(float x, float y, float z, float w) :
		Vector4(x, y, z, w)
	{
	}
	/// <summary>
	/// X軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationX(float angle)
	{
		SetRotation(Vector3::Vec3AxisX(), angle);
	}
	/// <summary>
	/// X軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegX(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisX(), angle);
	}
	/// <summary>
	/// Y軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationY(float angle)
	{
		SetRotation(Vector3::Vec3AxisY(), angle);
	}
	/// <summary>
	/// Y軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegY(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisY(), angle);
	}

	/// <summary>
	/// Z軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationZ(float angle)
	{
		SetRotation(Vector3::Vec3AxisZ(), angle);
	}
	/// <summary>
	/// Z軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDegZ(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisZ(), angle);
	}



	/// <summary>
	/// 任意の軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度。単位ラジアン。</param>
	void SetRotation(const Vector3& axis, float angle)
	{
		float s;
		float halfAngle = angle * 0.5f;
		s = sinf(halfAngle);
		w = cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	/// <summary>
	/// 任意の軸周りの回転クォータニオンを作成。
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度。単位Degree</param>
	void SetRotationDeg(const Vector3& axis, float angle)
	{
		float s;
		float halfAngle = Math::DegToRad(angle) * 0.5f;
		s = sinf(halfAngle);
		w = cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	/// <summary>
	/// 行列からクォータニオンを作成。
	/// </summary>
	/// <param name="m">行列</param>
	void SetRotation(const Matrix& m);
	/// <summary>
	/// fromベクトルからtoベクトルに回転させるクォータニオンを作成。
	/// </summary>
	/// <param name="from">回転前のベクトル</param>
	/// <param name="to">回転後のベクトル</param>
	void SetRotation(Vector3 from, Vector3 to);
	/// <summary>
	/// 球面線形補完
	/// </summary>
	/// <param name="t">補完率</param>
	/// <param name="q1">開始クォータニオン。</param>
	/// <param name="q2">終了クォータニオン。</param>
	void Slerp(float t, Quaternion q1, Quaternion q2)
	{
		DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
			DirectX::XMLoadFloat4(&q1.vec),
			DirectX::XMLoadFloat4(&q2.vec),
			t
		);
		DirectX::XMStoreFloat4(&vec, xmv);
	}
	/// <summary>
	/// Y軸周りの回転を加算。
	/// </summary>
	/// <returns>加算する回転角度。ラジアン単位。</returns>
	void AddRotationY(float angle)
	{
		Quaternion addRot;
		addRot.SetRotation(Vector3::Vec3AxisY(), angle);
		*this *= addRot;
	}
	/// <summary>
	/// クォータニオン同士の乗算
	/// </summary>
	/// <param name="rot"></param>
	void Multiply(const Quaternion& rot)
	{
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		qw = w; qx = x; qy = y; qz = z;
		pw = rot.w; px = rot.x; py = rot.y; pz = rot.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;

	}
	/// <summary>
	/// クォータニオン同士の乗算。
	/// </summary>
	/// <param name="rot0"></param>
	/// <param name="rot1"></param>
	void Multiply(const Quaternion& rot0, const Quaternion& rot1)
	{
		float pw, px, py, pz;
		float qw, qx, qy, qz;

		qw = rot0.w; qx = rot0.x; qy = rot0.y; qz = rot0.z;
		pw = rot1.w; px = rot1.x; py = rot1.y; pz = rot1.z;

		w = pw * qw - px * qx - py * qy - pz * qz;
		x = pw * qx + px * qw + py * qz - pz * qy;
		y = pw * qy - px * qz + py * qw + pz * qx;
		z = pw * qz + px * qy - py * qx + pz * qw;
	}
	/// <summary>
	/// クォータニオンの代入乗算演算子
	/// </summary>
	const Quaternion& operator*=(const Quaternion& rot0)
	{
		Multiply(rot0, *this);
		return *this;
	}
	/// <summary>
	/// ベクトルにクォータニオンを適用する。
	/// </summary>
	void Apply(Vector4& _v) const
	{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat4(&_v.vec, xmv);
	}
	/// <summary>
	/// ベクトルにクォータニオンを適用する。
	/// </summary>
	void Apply(Vector3& _v) const
	{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat3(&_v.vec, xmv);
	}
public:
};