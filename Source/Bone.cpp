#include "Bone.h"

//コンストラクタ
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

//デストラクタ
Bone::~Bone()
{
}

//このボーンのワールド空間での位置と回転とスケールを計算する
void Bone::CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale)
{
    //ワールド行列 取得
    Matrix world = this->world_Matrix_;

    //行列から拡大率 取得
    scale.x = world.v[0].Length();
    scale.y = world.v[1].Length();
    scale.z = world.v[2].Length();

    //新しい スケールに更新
    this->scale_ = scale;

    //平行移動量を取得,設定
    trans.Set(world.v[3]);
    this->positoin_ = trans;

    //行列から拡大率と平行移動量を譲許して回転量を取得
    world.v[0].Normalize();
    world.v[1].Normalize();
    world.v[2].Normalize();
    world.v[3].Set(0.0f, 0.0f, 0.0f, 0.0f);
    rot.SetRotation(world);
    rotation_ = rot;
}
