#pragma once
#include <DirectXMath.h>

#include "Matrix.h"

class Math;

/// <summary>
/// �N�I�[�^�j�I��
/// </summary>
class Quaternion : public Vector4
{
public:
	//static const Quaternion Identity;		//!<�P�ʃN�H�[�^�j�I���B

	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	Quaternion();
	

	/// <summary>
	/// �R�s�[ �R���X�g���N�^�B
	/// </summary>
	Quaternion(float x, float y, float z, float w);


	/// <summary>
	/// ���ʐ��`�⊮
	/// </summary>
	/// <param name="t">�⊮��</param>
	/// <param name="q1">�J�n�N�H�[�^�j�I���B</param>
	/// <param name="q2">�I���N�H�[�^�j�I���B</param>
	void Slerp(float t, Quaternion q1, Quaternion q2);

	/// <summary>
	/// Y������̉�]�����Z�B
	/// </summary>
	/// <returns>���Z�����]�p�x�B���W�A���P�ʁB</returns>
	void AddRotationY(float angle);

	/// <summary>
	/// �N�H�[�^�j�I�����m�̏�Z
	/// </summary>
	/// <param name="rot"></param>
	void Multiply(const Quaternion& rot);

	/// <summary>
	/// �N�H�[�^�j�I�����m�̏�Z�B
	/// </summary>
	/// <param name="rot0"></param>
	/// <param name="rot1"></param>
	void Multiply(const Quaternion& rot0, const Quaternion& rot1);

	/// <summary>
	/// �x�N�g���ɃN�H�[�^�j�I����K�p����B
	/// </summary>
	void Apply(Vector4& _v) const;

	/// <summary>
	/// �x�N�g���ɃN�H�[�^�j�I����K�p����B
	/// </summary>
	void Apply(Vector3& _v) const;

public://operator
	/// <summary>
	/// �N�H�[�^�j�I���̑����Z���Z�q
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
	/// �P�ʃN�H�[�^�j�I��
	/// </summary>
	/// <returns></returns>
	static Quaternion GetIdentity() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }
#pragma endregion

public://set method
#pragma region set method
	/// <summary>
	/// X������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationX(float angle) { SetRotation(Vector3::GetVec3AxisX(), angle); }

	/// <summary>
	/// X������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegX(float angle) { SetRotationDeg(Vector3::GetVec3AxisX(), angle); }

	/// <summary>
	/// Y������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationY(float angle) { SetRotation(Vector3::GetVec3AxisY(), angle); };

	/// <summary>
	/// Y������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegY(float angle) { SetRotationDeg(Vector3::GetVec3AxisY(), angle); }

	/// <summary>
	/// Z������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis"></param>
	/// <param name="angle"></param>
	void SetRotationZ(float angle) { SetRotation(Vector3::GetVec3AxisZ(), angle); }

	/// <summary>
	/// Z������̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDegZ(float angle) { SetRotationDeg(Vector3::GetVec3AxisZ(), angle); }

	/// <summary>
	/// �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x�B�P�ʃ��W�A���B</param>
	void SetRotation(const Vector3& axis, float angle);

	/// <summary>
	/// �C�ӂ̎�����̉�]�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="axis">��]��</param>
	/// <param name="angle">��]�p�x�B�P��Degree</param>
	void SetRotationDeg(const Vector3& axis, float angle);

	/// <summary>
	/// �s�񂩂�N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="m">�s��</param>
	void SetRotation(const Matrix& m)
	{
		DirectX::XMStoreFloat4(
			&vec,
			DirectX::XMQuaternionRotationMatrix(m)
		);
	}

	/// <summary>
	/// from�x�N�g������to�x�N�g���ɉ�]������N�H�[�^�j�I�����쐬�B
	/// </summary>
	/// <param name="from">��]�O�̃x�N�g��</param>
	/// <param name="to">��]��̃x�N�g��</param>
	void SetRotation(Vector3 from, Vector3 to)
	{
		from.Normalize();
		to.Normalize();
		auto t = ::Dot(from, to);
		Vector3 rotAxis;
		if (t > 0.998f) {
			//�قړ��������Ȃ̂ŒP�ʃN�H�[�^�j�I���ɂ���B
			*this = Quaternion::GetIdentity();
		}
		else if (t < -0.998f) {
			//�قڋt�����Ȃ̂ŁA
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