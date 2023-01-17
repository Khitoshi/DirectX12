#include "Quaternion.h"

#include "Math.h"

//デフォルト コンストラクタ
Quaternion::Quaternion()
{
	//初期化
	Set(0.0f, 0.0f, 0.0f, 1.0f);
	//x = y = z = 0.0f;
	//w = 1.0f;
}


//コピー コンストラクタ
Quaternion::Quaternion(float x, float y, float z, float w) :
	Vector4(x, y, z, w)
{
}

//球面線形補完
void Quaternion::Slerp(float t, Quaternion q1, Quaternion q2)
{
	DirectX::XMVECTOR xmv = DirectX::XMQuaternionSlerp(
		DirectX::XMLoadFloat4(&q1.vec),
		DirectX::XMLoadFloat4(&q2.vec),
		t
	);
	DirectX::XMStoreFloat4(&this->vec, xmv);
}

//Y軸周りの回転を加算。
void Quaternion::AddRotationY(float angle)
{
	Quaternion addRot;
	addRot.SetRotation(Vector3::GetVec3AxisY(), angle);
	*this *= addRot;
}

//クォータニオン同士の乗算
void Quaternion::Multiply(const Quaternion& rot)
{
	float pw, px, py, pz;
	float qw, qx, qy, qz;

	qw = this->w; qx = this->x; qy = this->y; qz = this->z;
	pw = rot.w; px = rot.x; py = rot.y; pz = rot.z;

	w = pw * qw - px * qx - py * qy - pz * qz;
	x = pw * qx + px * qw + py * qz - pz * qy;
	y = pw * qy - px * qz + py * qw + pz * qx;
	z = pw * qz + px * qy - py * qx + pz * qw;
}

//クォータニオン同士の乗算
void Quaternion::Multiply(const Quaternion& rot0, const Quaternion& rot1)
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

//ベクトルにクォータニオンを適用する
void Quaternion::Apply(Vector4& _v) const
{
	DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
	DirectX::XMStoreFloat4(&_v.vec, xmv);
}

void Quaternion::Apply(Vector3& _v) const
{
	DirectX::XMVECTOR xmv = DirectX::XMVector3Rotate(_v, *this);
	DirectX::XMStoreFloat3(&_v.vec, xmv);
}

//任意の軸周りの回転クォータニオンを作成。
void Quaternion::SetRotation(const Vector3& axis, float angle)
{
	float s;
	float halfAngle = angle * 0.5f;
	s = sinf(halfAngle);
	w = cosf(halfAngle);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}

//任意の軸周りの回転クォータニオンを作成。
void Quaternion::SetRotationDeg(const Vector3& axis, float angle)
{
	float s;
	float halfAngle = Math::DegToRad(angle) * 0.5f;
	s = sinf(halfAngle);
	w = cosf(halfAngle);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}