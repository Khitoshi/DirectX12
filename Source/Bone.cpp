#include "Bone.h"

//�R���X�g���N�^
Bone::Bone(
    const wchar_t* boneName,
    const Matrix& bindPose,
    const Matrix& invBindPose,
    int parentBoneNo,
    int boneId) :
    bone_Name_(boneName),
    parent_Bone_Index_(parentBoneNo),
    bone_Id_(boneId),

    bind_Pose_(bindPose),
    inv_Bind_Pose_(invBindPose),
    local_Matrix_(),
    world_Matrix_(),
    offset_Local_Matrix_(),

    //positoin_(Vector3(0, 0, 0)),
    positoin_(),
    //scale_(Vector3(0, 0, 0)),
    scale_(),
    rotation_(),

    bone_children_()
{
}

//�f�X�g���N�^
Bone::~Bone()
{
}

//���̃{�[���̃��[���h��Ԃł̈ʒu�Ɖ�]�ƃX�P�[�����v�Z����
void Bone::CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale)
{
    //���[���h�s�� �擾
    Matrix world = this->world_Matrix_;

    //�s�񂩂�g�嗦 �擾
    scale.x = world.v[0].Length();
    scale.y = world.v[1].Length();
    scale.z = world.v[2].Length();

    //�V���� �X�P�[���ɍX�V
    this->scale_ = scale;

    //���s�ړ��ʂ��擾,�ݒ�
    trans.Set(world.v[3]);
    this->positoin_ = trans;

    //�s�񂩂�g�嗦�ƕ��s�ړ��ʂ��������ĉ�]�ʂ��擾
    world.v[0].Normalize();
    world.v[1].Normalize();
    world.v[2].Normalize();
    world.v[3].Set(0.0f, 0.0f, 0.0f, 0.0f);
    rot.SetRotation(world);
    rotation_ = rot;
}
