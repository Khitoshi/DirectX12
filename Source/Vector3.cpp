#include "Vector3.h"

Vector3::Vector3() :
    vec(XMFLOAT3(0, 0, 0))
{
}

Vector3::Vector3(const float x,const float y,const float z) :
    vec(XMFLOAT3(x, y, z))
{
}

//
Vector3::Vector3(const XMFLOAT3 f):
    vec(f)
{
}

//���^���
void Vector3::Lerp(const float t, const Vector3& v0, const Vector3& v1)
{
    DirectX::XMVECTOR v = DirectX::XMVectorLerp(
        DirectX::XMLoadFloat3(&v0.vec),
        DirectX::XMLoadFloat3(&v1.vec),
        t);
    DirectX::XMStoreFloat3(&this->vec, v);
}

//�x�N�g���ɐݒ�
void Vector3::Set(const float x, const float y, const float z)
{
    //this->vec = XMFLOAT3(x, y, z);
    this->vec.x = x;
    this->vec.y = y;
    this->vec.z = z;
}

//�x�N�g���ɐݒu
void Vector3::Set(const Vector3 v)
{
    this->Set(v.vec.x, v.vec.y, v.vec.z);
}

//�x�N�g���̉��Z
void Vector3::Add(const Vector3& v)
{
    XMVECTOR xmv0 = XMLoadFloat3(&vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v.vec);
    XMVECTOR xmvr = XMVectorAdd(xmv0, xmv1);
    XMStoreFloat3(&this->vec, xmvr);
}

//�x�N�g���̉��Z
void Vector3::Add(const Vector3& v0, const Vector3 v1)
{
    XMVECTOR xmv0 = XMLoadFloat3(&v0.vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v1.vec);
    XMVECTOR xmvr = XMVectorAdd(xmv0, xmv1);
    XMStoreFloat3(&this->vec, xmvr);
}

//�x�N�g���̌��Z
void Vector3::Subtract(const Vector3& v)
{
    XMVECTOR xmv0 = XMLoadFloat3(&this->vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v.vec);
    XMVECTOR xmvr = XMVectorSubtract(xmv0, xmv1);
    XMStoreFloat3(&this->vec, xmvr);
}

//�x�N�g���̌��Z
void Vector3::Subtract(const Vector3& v0, const Vector3& v1)
{
    XMVECTOR xmv0 = XMLoadFloat3(&v0.vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v1.vec);
    XMVECTOR xmvr = XMVectorSubtract(xmv0, xmv1);
    XMStoreFloat3(&this->vec, xmvr);
}

//���ς��v�Z
float Vector3::Dot(const Vector3& v)
{
    XMVECTOR xmv0 = XMLoadFloat3(&this->vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v.vec);
    return XMVector3Dot(xmv0, xmv1).m128_f32[0];
}

//�O�ς��v�Z
void Vector3::Cross(const Vector3& v)
{
    DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&vec);
    DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v.vec);
    DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
    DirectX::XMStoreFloat3(&this->vec, xmvr);
}

//�O�ς̌v�Z
void Vector3::Cross(const Vector3& v0, const Vector3& v1)
{
    DirectX::XMVECTOR xmv0 = DirectX::XMLoadFloat3(&v0.vec);
    DirectX::XMVECTOR xmv1 = DirectX::XMLoadFloat3(&v1.vec);
    DirectX::XMVECTOR xmvr = DirectX::XMVector3Cross(xmv0, xmv1);
    DirectX::XMStoreFloat3(&this->vec, xmvr);
}

//�x�N�g���̒������擾
float Vector3::Length()const
{
    XMVECTOR xmv = XMLoadFloat3(&this->vec);
    return XMVector3Length(xmv).m128_f32[0];
}

//�x�N�g���̒����̓����擾
float Vector3::LengthSq() const
{
    XMVECTOR xmv = XMLoadFloat3(&vec);
    return XMVector3LengthSq(xmv).m128_f32[0];
}

//�x�N�g�����X�J���[�Ŋg��
void Vector3::Scale(const float s)
{
    XMVECTOR xmv = XMLoadFloat3(&this->vec);
    xmv = XMVectorScale(xmv, s);
    XMStoreFloat3(&this->vec, xmv);
}

//�x�N�g���̐��K��
void Vector3::Normalize()
{
    XMVECTOR xmv = XMLoadFloat3(&this->vec);
    xmv = XMVector3Normalize(xmv);
    XMStoreFloat3(&this->vec, xmv);
}

//�x�N�g�����X�J���[�ŏ��Z
void Vector3::Div(const float d)
{
    const float scale = 1.0f / d;
    this->Scale(scale);
}

//�x�N�g���ɍő�l��ݒ�
void Vector3::Max(const Vector3 v)
{
    XMVECTOR xmv0 = XMLoadFloat3(&this->vec);
    XMVECTOR xmv1 = XMLoadFloat3(&v.vec);
    XMStoreFloat3(&this->vec, XMVectorMax(xmv0, xmv1));
}

//�x�N�g���ɍŏ��l��ݒ�
void Vector3::Min(const Vector3& v)
{
    const XMVECTOR xmv0 = XMLoadFloat3(&this->vec);
    const XMVECTOR xmv1 = XMLoadFloat3(&v.vec);
    XMStoreFloat3(&this->vec, XMVectorMin(xmv0, xmv1));
}
