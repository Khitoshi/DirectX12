#include "Skeleton.h"

//#include <comutil.h>
#include <stdlib.h>

#pragma warning(disable : 4996)


//デフォルト コンストラクタ
Skeleton::Skeleton():
    tks_file_(),
    bones_(BONE_MAX),
    bone_Matrixs_(),

    is_Inited_(false),
    is_Play_Animation_(false)
{
}

//デフォルト デストラクタ
Skeleton::~Skeleton()
{
}

//初期化
void Skeleton::Init(const char* tksFilePath)
{
    //.tks file 読み込み
    this->tks_file_.Load(tksFilePath);

    //ボーン行列の構築
    this->BuildBoneMatrices();
}

//ボーン行列の構築
void Skeleton::BuildBoneMatrices()
{
    this->tks_file_.QueryBone([&](TksFile::SBone& tks_bone) {
        //バインドポーズ
        Matrix bind_pose_matrix;
        memcpy(bind_pose_matrix.m[0], &tks_bone.bind_Pose[0], sizeof(tks_bone.bind_Pose[0]));
        memcpy(bind_pose_matrix.m[1], &tks_bone.bind_Pose[1], sizeof(tks_bone.bind_Pose[1]));
        memcpy(bind_pose_matrix.m[2], &tks_bone.bind_Pose[2], sizeof(tks_bone.bind_Pose[2]));
        memcpy(bind_pose_matrix.m[3], &tks_bone.bind_Pose[3], sizeof(tks_bone.bind_Pose[3]));

        //バインドポーズの逆行列
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
        //マルチバイト文字列をワイド文字列に変換
        //mbstowcs_s(&value, bone_name, static_cast<size_t>(256), tks_bone.name.get(), _TRUNCATE);
        mbstowcs(bone_name, tks_bone.name.get(), 256);
        //ボーン 生成
        std::unique_ptr<Bone> bone = std::make_unique<Bone>(
            bone_name,
            bind_pose_matrix,
            inv_bind_pose_matrix,
            tks_bone.parent_No,
            tks_bone.id
            );

        //登録
        this->bones_.push_back(move(bone));
        });

    for (auto& bone : this->bones_)
    {
        if (bone->GetParentBoneNumber() != -1) {
            //ボーン追加
            this->bones_.at(bone->GetParentBoneNumber())->AddChild(bone.get());

            //ローカルマトリクスを計算
            const Matrix& parent_matrix = this->bones_.at(bone->GetParentBoneNumber())->GetInvBindPoseMatrix();
            Matrix local_matrix;
            local_matrix = bone->GetBindPoseMatrix() * parent_matrix;
            bone->SetLocalMatrix(local_matrix);
        }
        else
        {
            //ローカル行列 設定
            bone->SetLocalMatrix(bone->GetBindPoseMatrix());
        }
    }
    
    //ボーンマトリックス生成
    this->bone_Matrixs_ = std::make_unique<Matrix[]>(this->bones_.size());
    //初期化したので初期化済みフラグを建てる
    this->is_Inited_ = true;
}

int Skeleton::FindBoneID(const wchar_t* boneName) const
{
    
    //検索
    for (int i = 0; i < (int)this->bones_.size(); i++) {
        if (wcscmp(this->bones_[i]->GetName(), boneName) == 0) {
            return i;
        }
    }
    //見つからなかった。
    return -1;
    
}

//更新
void Skeleton::Update(const Matrix& matrix_World)
{
    //アニメーション更新
    AnimationUpdate(matrix_World);

    //ボーン行列を計算。
    int bone_no = 0;
    for (auto& bonePtr : this->bones_) {
        Matrix matrix_bone;
        matrix_bone = bonePtr->GetInvBindPoseMatrix() * bonePtr->GetWorldMatrix();
        this->bone_Matrixs_[bone_no] = matrix_bone;
        bone_no++;
    }
}

//ボーンのワールド行列の更新関数
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
        //ボーン行列をルートボーンの空間からワールド空間を構築していく
        for (auto& bone : this->bones_)
        {
            Matrix bone_world;
            Matrix local_matrix = bone->GetLocalMatrix();
            //親の行列とローカル行列を乗算して，ワールド行列を計算する
            bone_world = local_matrix * matrix_world;
            bone->SetWorldMatrix(bone_world);
        }
    }
    else
    {
        //アニメーションが流し込まれていると、ボーン行列がルートボーン空間に
        //変換されているが、流されていないと親の骨の座標系のままなので、
        //ルートボーン空間→ワールド空間への変換を行う。
        for (auto& bone : this->bones_)
        {
            //存在しない場合は飛ばす
            if (bone->GetParentBoneNumber() != -1)continue;
            //ルート
            UpdateBoneWorldMatrix(*bone, matrix_world);
        }
    }
}
