#include "Skeleton.h"

//#include <comutil.h>
#include <stdlib.h>

#pragma warning(disable : 4996)


//�f�t�H���g �R���X�g���N�^
Skeleton::Skeleton():
    tks_file_(),
    bones_(BONE_MAX),
    bone_Matrixs_(),

    is_Inited_(false),
    is_Play_Animation_(false)
{
}

//�f�t�H���g �f�X�g���N�^
Skeleton::~Skeleton()
{
}

//������
void Skeleton::Init(const char* tksFilePath)
{
    //.tks file �ǂݍ���
    this->tks_file_.Load(tksFilePath);

    //�{�[���s��̍\�z
    this->BuildBoneMatrices();
}

//�{�[���s��̍\�z
void Skeleton::BuildBoneMatrices()
{
    this->tks_file_.QueryBone([&](TksFile::SBone& tks_bone) {
        //�o�C���h�|�[�Y
        Matrix bind_pose_matrix;
        memcpy(bind_pose_matrix.m[0], &tks_bone.bind_Pose[0], sizeof(tks_bone.bind_Pose[0]));
        memcpy(bind_pose_matrix.m[1], &tks_bone.bind_Pose[1], sizeof(tks_bone.bind_Pose[1]));
        memcpy(bind_pose_matrix.m[2], &tks_bone.bind_Pose[2], sizeof(tks_bone.bind_Pose[2]));
        memcpy(bind_pose_matrix.m[3], &tks_bone.bind_Pose[3], sizeof(tks_bone.bind_Pose[3]));

        //�o�C���h�|�[�Y�̋t�s��
        Matrix inv_bind_pose_matrix;
        memcpy(inv_bind_pose_matrix.m[0], &tks_bone.inv_Bind_Pose[0], sizeof(tks_bone.inv_Bind_Pose[0]));
        memcpy(inv_bind_pose_matrix.m[1], &tks_bone.inv_Bind_Pose[1], sizeof(tks_bone.inv_Bind_Pose[1]));
        memcpy(inv_bind_pose_matrix.m[2], &tks_bone.inv_Bind_Pose[2], sizeof(tks_bone.inv_Bind_Pose[2]));
        memcpy(inv_bind_pose_matrix.m[3], &tks_bone.inv_Bind_Pose[3], sizeof(tks_bone.inv_Bind_Pose[3]));
        inv_bind_pose_matrix.m[0][3] = 0.0f;
        inv_bind_pose_matrix.m[1][3] = 0.0f;
        inv_bind_pose_matrix.m[2][3] = 0.0f;
        inv_bind_pose_matrix.m[3][3] = 1.0f;

        wchar_t bone_name[256];
        size_t value;
        //�}���`�o�C�g����������C�h������ɕϊ�
        //mbstowcs_s(&value, bone_name, static_cast<size_t>(256), tks_bone.name.get(), _TRUNCATE);
        mbstowcs(bone_name, tks_bone.name.get(), 256);
        //�{�[�� ����
        std::unique_ptr<Bone> bone = std::make_unique<Bone>(
            bone_name,
            bind_pose_matrix,
            inv_bind_pose_matrix,
            tks_bone.parent_No,
            tks_bone.id
            );

        //�o�^
        this->bones_.push_back(move(bone));
        });

    for (auto& bone : this->bones_)
    {
        if (bone->GetParentBoneNumber() != -1) {
            //�{�[���ǉ�
            this->bones_.at(bone->GetParentBoneNumber())->AddChild(bone.get());

            //���[�J���}�g���N�X���v�Z
            const Matrix& parent_matrix = this->bones_.at(bone->GetParentBoneNumber())->GetInvBindPoseMatrix();
            Matrix local_matrix;
            local_matrix = bone->GetBindPoseMatrix() * parent_matrix;
            bone->SetLocalMatrix(local_matrix);
        }
        else
        {
            //���[�J���s�� �ݒ�
            bone->SetLocalMatrix(bone->GetBindPoseMatrix());
        }
    }
    
    //�{�[���}�g���b�N�X����
    this->bone_Matrixs_ = std::make_unique<Matrix[]>(this->bones_.size());
    //�����������̂ŏ������ς݃t���O�����Ă�
    this->is_Inited_ = true;
}

int Skeleton::FindBoneID(const wchar_t* boneName) const
{
    
    //����
    for (int i = 0; i < (int)this->bones_.size(); i++) {
        if (wcscmp(this->bones_[i]->GetName(), boneName) == 0) {
            return i;
        }
    }
    //������Ȃ������B
    return -1;
    
}

//�X�V
void Skeleton::Update(const Matrix& matrix_World)
{
    //�A�j���[�V�����X�V
    AnimationUpdate(matrix_World);

    //�{�[���s����v�Z�B
    int bone_no = 0;
    for (auto& bonePtr : this->bones_) {
        Matrix matrix_bone;
        matrix_bone = bonePtr->GetInvBindPoseMatrix() * bonePtr->GetWorldMatrix();
        this->bone_Matrixs_[bone_no] = matrix_bone;
        bone_no++;
    }
}

//�{�[���̃��[���h�s��̍X�V�֐�
void Skeleton::UpdateBoneWorldMatrix(Bone& bone, const Matrix& parentMatrix)
{
    Matrix bone_world;
    Matrix local_matrix = bone.GetLocalMatrix();
    bone_world = local_matrix * parentMatrix;

    bone.SetWorldMatrix(bone_world);
    for (auto childBone : bone.GetChildren()) {
        UpdateBoneWorldMatrix(*childBone, bone_world);
    }
}

void Skeleton::AnimationUpdate(const Matrix& matrix_world)
{
    if (this->is_Play_Animation_)
    {
        //�{�[���s������[�g�{�[���̋�Ԃ��烏�[���h��Ԃ��\�z���Ă���
        for (auto& bone : this->bones_)
        {
            Matrix bone_world;
            Matrix local_matrix = bone->GetLocalMatrix();
            //�e�̍s��ƃ��[�J���s�����Z���āC���[���h�s����v�Z����
            bone_world = local_matrix * matrix_world;
            bone->SetWorldMatrix(bone_world);
        }
    }
    else
    {
        //�A�j���[�V�������������܂�Ă���ƁA�{�[���s�񂪃��[�g�{�[����Ԃ�
        //�ϊ�����Ă��邪�A������Ă��Ȃ��Ɛe�̍��̍��W�n�̂܂܂Ȃ̂ŁA
        //���[�g�{�[����ԁ����[���h��Ԃւ̕ϊ����s���B
        for (auto& bone : this->bones_)
        {
            //���݂��Ȃ��ꍇ�͔�΂�
            if (bone->GetParentBoneNumber() != -1)continue;
            //���[�g
            UpdateBoneWorldMatrix(*bone, matrix_world);
        }
    }
}
