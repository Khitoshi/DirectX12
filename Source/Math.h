#pragma once
#include <DirectXMath.h>

class Math
{
public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	Math();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~Math();

	/// <summary>
	/// �p�x��Degree�P�ʂ���Radian�P�ʂɕϊ�����B
	/// </summary>
	/// <param name="deg">Degree�P�ʂ̊p�x�B</param>
	/// <returns>Radian�P�ʂ̊p�x�B</returns>
	constexpr static inline float DegToRad(float deg){return deg * (PI() / 180.0f);}
	
	/// <summary>
	/// �p�x��Radian�P�ʂ���Degree�P�ʂɕϊ�����B
	/// </summary>
	/// <param name="rad">Radian�P�ʂ̊p�x�B</param>
	/// <returns>Degree�P�ʂ̊p�x�B</returns>
	constexpr static inline float RadToDeg(float rad){return rad / (PI() / 180.0f);}

	/// <summary>
	/// t0��t1�̊Ԃ���`�⊮�B
	/// </summary>
	/// <param name="rate">��ԗ���0.0�`1.0</param>
	/// <param name="t0">��ԊJ�n�̒l�B</param>
	/// <param name="t1">��ԏI���̒l�B</param>
	/// <returns>��Ԃ��ꂽ�l�B</returns>
	template<class T> static inline T Lerp(float rate, T t0, T t1)
	{
		T ret;
		ret.Lerp(rate, t0, t1);
		return ret;
	}

	/// <summary>
	/// t0��t1�̊Ԃ���`�⊮�B
	/// </summary>
	/// <param name="rate">��ԗ���0.0�`1.0</param>
	/// <param name="t0">��ԊJ�n�̒l�B</param>
	/// <param name="t1">��ԏI���̒l�B</param>
	/// <returns>��Ԃ��ꂽ�l�B</returns>
	template<> static inline float Lerp(float rate, float t0, float t1)
	{
		return t0 + (t1 - t0) * rate;
	}

public:
	//�~����
	const static constexpr float PI() { return 3.14159265358979323846f; }

	//�~�����̂Q��
	const static constexpr float PI2() { return PI() * 2.0f; }
};
