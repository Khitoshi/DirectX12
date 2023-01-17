#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include "Math.h"
/// <summary>
/// �N�I�[�^�j�I��
/// </summary>
class Quaternion : public Vector4
{
public:
	//static const Quaternion Identity;		//!<�P�ʃN�H�[�^�j�I���B
	Quaternion()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Quaternion(float x, float y, float z, float w) :
		Vector4(x, y, z, w)
	{
	}
	/// <summary>
	/// X������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationX(float angle)
	{
		SetRotation(Vector3::Vec3AxisX(), angle);
	}
	/// <summary>
	/// X������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegX(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisX(), angle);
	}
	/// <summary>
	/// Y������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationY(float angle)
	{
		SetRotation(Vector3::Vec3AxisY(), angle);
	}
	/// <summary>
	/// Y������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegY(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisY(), angle);
	}

	/// <summary>
	/// Z������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationZ(float angle)
	{
		SetRotation(Vector3::Vec3AxisZ(), angle);
	}
	/// <summary>
	/// Z������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegZ(float angle)
	{
		SetRotationDeg(Vector3::Vec3AxisZ(), angle);
	}



	/// <summary>
	/// �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x�B�P�ʃ��W�A���B</param>
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
	/// �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
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
	/// �s�񂩂�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="m">�s��</param>
	void SetRotation(const Matrix& m);
	/// <summary>
	/// from�x�N�g������to�x�N�g���ɉ�]������N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="from">��]�O�̃x�N�g��</param>
	/// <param name="to">��]��̃x�N�g��</param>
	void SetRotation(Vector3 from, Vector3 to);
	/// <summary>
	/// ���ʐ��`�⊮
	/// </summary>
	/// <param name="t">�⊮��</param>
	/// <param name="q1">�J�n�N�H�[�^�j�I���B</param>
	/// <param name="q2">�I���N�H�[�^�j�I���B</param>
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
	/// Y������̉�]�����Z�B
	/// </summary>
	/// <returns>���Z�����]�p�x�B���W�A���P�ʁB</returns>
	void AddRotationY(float angle)
	{
		Quaternion addRot;
		addRot.SetRotation(Vector3::Vec3AxisY(), angle);
		*this *= addRot;
	}
	/// <summary>
	/// �N�H�[�^�j�I�����m�̏�Z
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
	/// �N�H�[�^�j�I�����m�̏�Z�B
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
	/// �N�H�[�^�j�I���̑����Z���Z�q
	/// </summary>
	const Quaternion& operator*=(const Quaternion& rot0)
	{
		Multiply(rot0, *this);
		return *this;
	}
	/// <summary>
	/// �x�N�g���ɃN�H�[�^�j�I����K�p����B
	/// </summary>
	void Apply(Vector4& _v) const
	{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat4(&_v.vec, xmv);
	}
	/// <summary>
	/// �x�N�g���ɃN�H�[�^�j�I����K�p����B
	/// </summary>
	void Apply(Vector3& _v) const
	{
		DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
		DirectX::XMStoreFloat3(&_v.vec, xmv);
	}
public:
};